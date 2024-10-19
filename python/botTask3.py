import asyncio
from pybit.unified_trading import HTTP
from telegram import Update
from telegram.ext import ApplicationBuilder, CommandHandler

# Словарь для хранения пользователей и их данных
users = {}

# Инициализация сессии с биржей (testnet=True для тестирования)
session = HTTP(testnet=True)

# Доступные активы
assets = ["BTCUSD", "ETHUSD", "LTCUSD", "XRPUSD", "DOTUSD", "ADAUSD", "MANAUSD", "EOSUSD", "SOLUSD"]

# Начальный капитал
initial_balance = 100

# Обработчик команды /start
async def start(update: Update, context) -> None:
    if update.message is None:
        return

    user_id = update.message.from_user.id
    username = update.message.from_user.username

    if username:
        if username not in users:
            users[username] = {
                'user_id': user_id,
                'username': username,
                'balance': initial_balance,
                'selected_asset': None,
                'buy_price': None,
                'assets': {},
                'buy_coefficient': 1.001,  # Коэффициент покупки по умолчанию
                'drop_coefficient': 0.999,  # Коэффициент продажи по умолчанию
                'is_trading': False,  # Флаг для контроля торговли
            }
        print("Start bot...")
        
        await update.message.reply_text(f"Привет, {username}!\n"
                                        f"Ваш текущий баланс: {users[username]['balance']} долларов.\n"
                                        f"Доступные активы: {', '.join(assets)}\n"
                                        f"Выбери актив командой /select <имя актива>.\n"
                                        f"Справка по командам /help")
    else:
        await update.message.reply_text("У вас нет имени пользователя. Придумайте его, чтобы я мог отправлять вам сообщения.")

# Обработчик команды /select
async def select_asset(update: Update, context) -> None:
    if update.message is None:
        return

    username = update.message.from_user.username
    if not username:
        await update.message.reply_text("У вас нет имени пользователя.")
        return

    if len(context.args) == 0:
        await update.message.reply_text("Укажите актив. Пример: /select BTCUSD")
        return

    asset = context.args[0].upper()
    if asset not in assets:
        await update.message.reply_text(f"Неправильный актив. Доступные активы: {', '.join(assets)}")
        return

    user_data = users[username]
    user_data['selected_asset'] = asset
    user_data['buy_price'] = None  # Сбросить цену покупки
    user_data['is_trading'] = True  # Установить флаг торговли в True

    await update.message.reply_text(f"Вы выбрали актив {asset}.")

    # Запуск автоматической торговли для выбранного актива
    asyncio.create_task(trade_user_asset(user_data))

# Обработчик команды /stop для остановки торговли
async def stop_trading(update: Update, context) -> None:
    if update.message is None:
        return

    username = update.message.from_user.username
    if not username:
        await update.message.reply_text("У вас нет имени пользователя.")
        return

    user_data = users[username]
    user_data['is_trading'] = False  # Установить флаг торговли в False

    await update.message.reply_text("Торговля остановлена.")

# Обработчик команды /setbuy для установки коэффициента покупки
async def set_buy_coefficient(update: Update, context) -> None:
    if update.message is None:
        return

    username = update.message.from_user.username
    if not username:
        await update.message.reply_text("У вас нет имени пользователя.")
        return

    if len(context.args) == 0:
        await update.message.reply_text("Укажите коэффициент покупки. Пример: /setbuy 1.005")
        return

    try:
        coefficient = float(context.args[0])
        users[username]['buy_coefficient'] = coefficient
        await update.message.reply_text(f"Коэффициент покупки установлен на {coefficient}.")
    except ValueError:
        await update.message.reply_text("Неправильный коэффициент. Пожалуйста, укажите число.")

# Обработчик команды /setdrop для установки коэффициента продажи
async def set_drop_coefficient(update: Update, context) -> None:
    if update.message is None:
        return

    username = update.message.from_user.username
    if not username:
        await update.message.reply_text("У вас нет имени пользователя.")
        return

    if len(context.args) == 0:
        await update.message.reply_text("Укажите коэффициент продажи. Пример: /setdrop 0.995")
        return

    try:
        coefficient = float(context.args[0])
        users[username]['drop_coefficient'] = coefficient
        await update.message.reply_text(f"Коэффициент продажи установлен на {coefficient}.")
    except ValueError:
        await update.message.reply_text("Неправильный коэффициент. Пожалуйста, укажите число.")

# Функция автоматической торговли
async def trade_user_asset(user_data):
    asset = user_data['selected_asset']
    buy_coefficient = user_data['buy_coefficient']  # Используем установленный коэффициент покупки
    drop_coefficient = user_data['drop_coefficient']  # Используем установленный коэффициент продажи

    while user_data['is_trading']:  # Проверяем флаг торговли
        response = session.get_tickers(category="inverse", symbol=asset)
        current_price = float(response['result']['list'][0]['indexPrice'])

        # Определяем target_sell_price и drop_sell_price
        target_sell_price = None
        drop_sell_price = None

        if user_data['buy_price'] is None:  # Условия покупки
            user_data['buy_price'] = current_price  # Устанавливаем цену покупки
            amount_to_invest = 10  # Сумма для покупки
            amount_purchased = amount_to_invest / current_price  # Рассчитываем количество актива
            user_data['balance'] -= amount_to_invest  # Уменьшаем баланс
            user_data['assets'][asset] = user_data['assets'].get(asset, 0) + amount_purchased  # Добавляем актив

            # Устанавливаем цены для продажи после покупки
            target_sell_price = user_data['buy_price'] * buy_coefficient  # Продажа при росте
            drop_sell_price = user_data['buy_price'] * drop_coefficient  # Продажа при падении

            await send_message(application, user_data['username'], f"Вы купили {amount_purchased:.4f} {asset} по цене {current_price} на 10 долларов.\n"
                                                                    f"Остаток баланса: {user_data['balance']} долларов.\n"
                                                                    f"Ваши активы: {user_data['assets']}\n"
                                                                    f"Цена, продажи при росте: {target_sell_price:.4f}\n"
                                                                    f"Цена, продажи при падении: {drop_sell_price:.4f}\n"
                                                                    f"Торговый бот запущен...")

        else:  # Условия продажи
            target_sell_price = user_data['buy_price'] * buy_coefficient  # Продажа при росте
            drop_sell_price = user_data['buy_price'] * drop_coefficient  # Продажа при падении

            if current_price >= target_sell_price:  # Продажа при росте
                amount_sold = user_data['assets'][asset]  # Количество проданного актива
                user_data['balance'] += amount_sold * current_price  # Добавляем деньги за проданные активы
                del user_data['assets'][asset]  # Удаляем актив из портфеля
                await send_message(application, user_data['username'], f"Вы продали {amount_sold:.4f} {asset} по цене {current_price} (рост).\n"
                                                                        f"Ваш баланс: {user_data['balance']} долларов.\n"
                                                                        f"Ваши активы: {user_data['assets']}\n"
                                                                        f"Цена, продажи при росте: {target_sell_price:.4f}\n"
                                                                        f"Цена, продажи при падении: {drop_sell_price:.4f}\n"
                                                                        f"Торговый бот запущен...")
                user_data['buy_price'] = None  # Сброс цены покупки для следующей сделки

            elif current_price <= drop_sell_price:  # Продажа при падении
                amount_sold = user_data['assets'][asset]  # Количество проданного актива
                user_data['balance'] += amount_sold * current_price  # Добавляем деньги за проданные активы
                del user_data['assets'][asset]  # Удаляем актив из портфеля
                await send_message(application, user_data['username'], f"Вы продали {amount_sold:.4f} {asset} по цене {current_price} (падение).\n"
                                                                        f"Ваш баланс: {user_data['balance']} долларов.\n"
                                                                        f"Ваши активы: {user_data['assets']}")
                user_data['buy_price'] = None  # Сброс цены покупки для следующей сделки
        
        # Печатаем информацию о ценах
        print('Цена покупки: ' + str(user_data['buy_price']))
        print('Текущая цена: ' + str(current_price))
        print('Цена, по которой произойдет продажа при росте: ' + (str(target_sell_price) if target_sell_price is not None else "Не установлена"))
        print('Цена, по которой произойдет продажа при падении: ' + (str(drop_sell_price) if drop_sell_price is not None else "Не установлена"))
        

        await send_message(application, user_data['username'], f"Текущая цена {asset}: {current_price:.4f} долларов.")

        await asyncio.sleep(10)  # Ждем перед следующей проверкой

# Функция отправки сообщения пользователю
async def send_message(application, username, text):
    user_id = users[username]['user_id']
    await application.bot.send_message(chat_id=user_id, text=text)

# Обработчик команды /price для показа текущей цены актива
async def show_price(update: Update, context) -> None:
    if update.message is None:
        return

    username = update.message.from_user.username
    if not username:
        await update.message.reply_text("У вас нет имени пользователя.")
        return

    if len(context.args) == 0:
        await update.message.reply_text("Укажите актив. Пример: /price BTCUSD")
        return

    asset = context.args[0].upper()
    if asset not in assets:
        await update.message.reply_text(f"Неправильный актив. Доступные активы: {', '.join(assets)}")
        return

    response = session.get_tickers(category="inverse", symbol=asset)
    current_price = float(response['result']['list'][0]['indexPrice'])
    await update.message.reply_text(f"Текущая цена {asset}: {current_price} долларов.")

# Обработчик команды /balance
async def balance(update: Update, context) -> None:
    if update.message is None:
        return

    username = update.message.from_user.username
    if not username:
        await update.message.reply_text("У вас нет имени пользователя.")
        return

    user_data = users[username]
    assets_info = ', '.join([f"{asset}: {amount:.15f}" for asset, amount in user_data['assets'].items()])
    await update.message.reply_text(f"Ваш баланс: {user_data['balance']} долларов.\n"
                                     f"Ваши активы: {assets_info if assets_info else 'Нет активов'}.")
# Обработчик команды /help
async def help_command(update: Update, context) -> None:
    help_text = (
        "/start - Начать работу с ботом и получить начальный баланс.\n"
        "/select <актив> - Выбрать актив для торговли (например, /select BTCUSD).\n"
        "/setbuy <коэффициент> - Установить коэффициент для покупки (например, /setbuy 1.005).\n"
        "/setdrop <коэффициент> - Установить коэффициент для продажи (например, /setdrop 0.995).\n"
        "/balance - Показать текущий баланс и список активов.\n"
        "/price <актив> - Показать текущую цену выбранного актива (например, /price BTCUSD).\n"
        "/stop - Остановить автоматическую торговлю.\n"
        "/help - Показать описание всех команд."
    )
    await update.message.reply_text(help_text)

# Основная функция запуска бота
def main():
    global application
    print("The program is working now, please launch the bot in telegram!")
    # Читаем токен из файла key
    with open("key", "r") as file:
        bot_token = file.read().strip()  # Читаем токен и удаляем лишние пробелы

    application = ApplicationBuilder().token(bot_token).build()

    # Добавляем обработчики команд и сообщений
    application.add_handler(CommandHandler("start", start))
    application.add_handler(CommandHandler("select", select_asset))
    application.add_handler(CommandHandler("setbuy", set_buy_coefficient))
    application.add_handler(CommandHandler("setdrop", set_drop_coefficient))
    application.add_handler(CommandHandler("balance", balance))
    application.add_handler(CommandHandler("price", show_price))  # Добавляем команду /price
    application.add_handler(CommandHandler("stop", stop_trading))
    application.add_handler(CommandHandler("help", help_command)) 

    # Запускаем бота
    application.run_polling()

if __name__ == "__main__":
    main()
