import asyncio
import threading
from telegram import InlineKeyboardButton, InlineKeyboardMarkup, Update
from telegram.ext import ApplicationBuilder, CommandHandler, CallbackQueryHandler, MessageHandler, filters

# Словарь для хранения пользователей и их викторин
users = {}
quiz_games = {}

# Класс для игры викторина
class QuizGame:
    def __init__(self, questions):
        self.questions = questions
        self.current_question_index = 0

    def get_current_question(self):
        return self.questions[self.current_question_index]

    def check_answer(self, selected_option):
        current_question = self.get_current_question()
        return selected_option == current_question['answer']

    def next_question(self):
        self.current_question_index += 1
        if self.current_question_index >= len(self.questions):
            return None  # Все вопросы закончились
        return self.get_current_question()

# Вопросы для викторины
questions = [
    {"question": "Что такое Python?", "options": ["Язык программирования", "Змея", "Вид гадюки"], "answer": 0},
    {"question": "Какой метод используется для добавления элементов в список?", "options": ["push", "append", "add"], "answer": 1},
    {"question": "Какая функция используется для вывода на экран в Python?", "options": ["echo", "print", "write"], "answer": 1},
    {"question": "Какое расширение у Python файлов?", "options": [".py", ".txt", ".exe"], "answer": 0},
    {"question": "Какое ключевое слово используется для создания функции в Python?", "options": ["def", "func", "lambda"], "answer": 0},
]

# Обработчик команды /start
async def start(update: Update, context) -> None:
    user_id = update.message.from_user.id
    username = update.message.from_user.username

    if username:
        users[username] = user_id
        await update.message.reply_text(f"Привет, {username}! Викторина начинается.")
    else:
        await update.message.reply_text("У вас нет имени пользователя. Придумайте его, чтобы я мог отправлять вам сообщения.")
    
    # Начинаем игру сразу после приветствия
    quiz_games[user_id] = QuizGame(questions)
    await send_question(update, user_id)

# Отправляем текущий вопрос пользователю
async def send_question(update: Update, user_id):
    game = quiz_games.get(user_id)

    if game:
        question_data = game.get_current_question()
        if question_data:
            keyboard = [[InlineKeyboardButton(option, callback_data=str(index)) for index, option in enumerate(question_data['options'])]]
            reply_markup = InlineKeyboardMarkup(keyboard)

            await update.message.reply_text(question_data['question'], reply_markup=reply_markup)
        else:
            await update.message.reply_text("Викторина завершена! Спасибо за участие.")
    else:
        await update.message.reply_text("Игра не найдена.")

# Обработчик ответа на викторину
async def answer(update: Update, context) -> None:
    query = update.callback_query
    user_id = query.from_user.id
    selected_option = int(query.data)

    # Получаем текущую викторину для пользователя
    game = quiz_games.get(user_id)
    
    if game:
        if game.check_answer(selected_option):
            await query.edit_message_text("Правильно!")
        else:
            await query.edit_message_text("Неправильно! Попробуйте еще раз.")
        
        # Переход к следующему вопросу
        next_question = game.next_question()
        if next_question:
            await send_question(query, user_id)
        else:
            await query.message.reply_text("Викторина завершена!")
    else:
        await query.edit_message_text("Викторина не найдена.")

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
    application.add_handler(CallbackQueryHandler(answer))  # Обработчик ответов на викторину

    # Запускаем поток для отправки сообщений из консоли
    threading.Thread(target=send_message_from_console, args=(application,), daemon=True).start()

    # Запускаем бота с использованием метода run_polling
    application.run_polling()

if __name__ == "__main__":
    main()
