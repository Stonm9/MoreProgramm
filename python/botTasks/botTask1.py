import asyncio
import threading
from telegram import Update
from telegram.ext import ApplicationBuilder, CommandHandler, MessageHandler, filters

# Словарь для хранения пользователей
users = {}

# Обработчик команды /start
async def start(update: Update, context) -> None:
    user_id = update.message.from_user.id
    username = update.message.from_user.username

    if username:
        users[username] = user_id
        await update.message.reply_text(f"Привет, {username}! Я теперь запомнил тебя.")
    else:
        await update.message.reply_text("У вас нет имени пользователя. Придумайте его, чтобы я мог отправлять вам сообщения.")

# Обработчик текстовых сообщений
async def handle_message(update: Update, context) -> None:
    message = update.message.text
    user_id = update.message.from_user.id
    username = update.message.from_user.username

    if username:
        users[username] = user_id
        print(f"Получено сообщение от {username}: {message}")
    else:
        print(f"Получено сообщение от пользователя без ника: {message}")

    await update.message.reply_text("Сообщение получено!")

# Функция отправки сообщения пользователю
def send_message_from_console(application):
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)  # Устанавливаем новый event loop для текущего потока

    while True:
        username = input("Введите имя пользователя для отправки сообщения: ")
        if username in users:
            user_id = users[username]
            message = input("Введите сообщение для отправки: ")

            # Отправляем сообщение в новом цикле событий
            loop.run_until_complete(application.bot.send_message(chat_id=user_id, text=message))
        else:
            print("Пользователь с таким ником не найден.")

# Основная функция запуска бота
def main():
    # Читаем токен из файла key
    with open("key", "r") as file:
        bot_token = file.read().strip()  # Читаем токен и удаляем лишние пробелы

    application = ApplicationBuilder().token(bot_token).build()

    # Добавляем обработчики команд и сообщений
    application.add_handler(CommandHandler("start", start))
    application.add_handler(MessageHandler(filters.TEXT & ~filters.COMMAND, handle_message))

    # Запускаем поток для отправки сообщений из консоли
    threading.Thread(target=send_message_from_console, args=(application,), daemon=True).start()

    # Запускаем бота с использованием метода run_polling
    application.run_polling()

if __name__ == "__main__":
    main()
