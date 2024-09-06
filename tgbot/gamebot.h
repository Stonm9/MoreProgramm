// Этот файл содержит игровую механику для телеграмм бота
// Игра, в которой пользователь может путешествовать по городам,
// покупать и продавать товары, цель игры — накопить 1 000 000 000.
// ----------------------------------------
// Комилируеться ручную с помощью g++ -fsanitize=address main.cpp database.cpp -o telegram_bot --std=c++14
// -I/usr/local/include -lTgBot -lboost_system -lssl -lcrypto -lpthread -lsqlite3
// Курс ФПМИ МФТИ "Разработка на С++" студент Петров Денис
#ifndef GAMEBOT_H
#define GAMEBOT_H

#include <tgbot/tgbot.h>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>
#include "token.h"     // Храним токен
#include "database.h"  // Подключаем файл базы данных
#include <thread>
#include <chrono>

// Цены на товары в разных городах
const std::unordered_map<int, std::vector<int>> kCityPrices = {
    {0, {22, 60, 350, 180, 500, 720, 1100, 2200, 15000, 480000}},  // Москва
    {1, {18, 45, 380, 200, 450, 600, 1600, 3000, 16000, 650000}},  // Санкт-Петербург
    {2, {15, 50, 300, 220, 400, 800, 1250, 2100, 13000, 500000}},  // Калининград
    {3, {20, 55, 410, 170, 530, 780, 1400, 2800, 14000, 610000}}   // Казань
};

// Стоимость билетов между городами
const int kTicketPrices[4][4] = {
    {0, 2500, 8000, 4500},  // Москва -> другие города
    {2500, 0, 5500, 3500},  // Санкт-Петербург -> другие города
    {8000, 5500, 0, 6000},  // Калининград -> другие города
    {4500, 3500, 6000, 0}   // Казань -> другие города
};

class GameBot {
 protected:
  TgBot::Bot bot_;
  std::unordered_map<int64_t, int32_t> lastMessageMap_;  // Карта для хранения данных о последнем сообщении
  std::unordered_map<int64_t, std::string> lastMessageTextMap_;  // Карта хранения текста последнего сообщения
  Database db_;                                                  // Сама БД

  TgBot::InlineKeyboardMarkup::Ptr CreateInlineKeyboard() {
    TgBot::InlineKeyboardMarkup::Ptr inline_keyboard(new TgBot::InlineKeyboardMarkup);

    TgBot::InlineKeyboardButton::Ptr my_inventory_button = std::make_shared<TgBot::InlineKeyboardButton>();
    my_inventory_button->text = "Мои товары";
    my_inventory_button->callbackData = "my_inventory";

    TgBot::InlineKeyboardButton::Ptr show_goods_button = std::make_shared<TgBot::InlineKeyboardButton>();
    show_goods_button->text = "Товары на рынке";
    show_goods_button->callbackData = "buyButton";

    TgBot::InlineKeyboardButton::Ptr to_moscow_button = std::make_shared<TgBot::InlineKeyboardButton>();
    to_moscow_button->text = "Едем в Москву";
    to_moscow_button->callbackData = "toMoscowButton";

    TgBot::InlineKeyboardButton::Ptr to_spb_button = std::make_shared<TgBot::InlineKeyboardButton>();
    to_spb_button->text = "Едем в Санкт-Петербург";
    to_spb_button->callbackData = "toSPBButton";

    TgBot::InlineKeyboardButton::Ptr to_kaliningrad_button = std::make_shared<TgBot::InlineKeyboardButton>();
    to_kaliningrad_button->text = "Едем в Калининград";
    to_kaliningrad_button->callbackData = "toKaliningradButton";

    TgBot::InlineKeyboardButton::Ptr to_kazan_button = std::make_shared<TgBot::InlineKeyboardButton>();
    to_kazan_button->text = "Едем в Казань";
    to_kazan_button->callbackData = "toKazanButton";

    TgBot::InlineKeyboardButton::Ptr price_button = std::make_shared<TgBot::InlineKeyboardButton>();
    price_button->text = "Стоимость проезда";
    price_button->callbackData = "priceButton";

    TgBot::InlineKeyboardButton::Ptr help_button = std::make_shared<TgBot::InlineKeyboardButton>();
    help_button->text = "Помощь";
    help_button->callbackData = "helpButton";

    inline_keyboard->inlineKeyboard.push_back({my_inventory_button, to_moscow_button});
    inline_keyboard->inlineKeyboard.push_back({show_goods_button, to_spb_button});
    inline_keyboard->inlineKeyboard.push_back({price_button, to_kaliningrad_button});
    inline_keyboard->inlineKeyboard.push_back({help_button, to_kazan_button});

    return inline_keyboard;
  }
  std::string GetCityName(int city_id) {
    switch (city_id) {
      case 0:
        return "Москва";
      case 1:
        return "Санкт-Петербург";
      case 2:
        return "Калининград";
      case 3:
        return "Казань";
      default:
        return "Неизвестно";
    }
  }

  void SendVictoryMessage(int64_t chat_id, int trips) {
    std::string message = "Поздравляю! Вы набрали 1 000 000 000 руб.\n";
    message += "Для этого вам потребовалось " + std::to_string(trips) + " поездок.\n";
    message += "Если вы хотите начать заново, наберите команду /reset.";

    TgBot::InlineKeyboardMarkup::Ptr inline_keyboard = CreateInlineKeyboard();
    SendOrEditMessage(bot_, lastMessageMap_, lastMessageTextMap_, chat_id, message, nullptr);
  }

  void SendOrEditMessage(TgBot::Bot& bot, std::unordered_map<int64_t, int32_t>& last_message_map,
                         std::unordered_map<int64_t, std::string>& last_message_text_map, int64_t chat_id,
                         const std::string& send_message, TgBot::InlineKeyboardMarkup::Ptr inline_keyboard) {
    auto& api = bot.getApi();

    if (last_message_map.find(chat_id) != last_message_map.end()) {
      auto last_message_id = last_message_map[chat_id];
      // Проверяем, есть ли у нас сохраненный текст для последнего сообщения
      if (last_message_text_map.find(last_message_id) != last_message_text_map.end()) {
        if (last_message_text_map[last_message_id] == send_message) {
          std::cout << "Содержание сообщения не изменилось. Нет необходимости редактировать." << std::endl;
          return;
        }
      }
      std::cout << "Редактирование предыдущего сообщения с id: " << last_message_id << std::endl;
      api.editMessageText(send_message, chat_id, last_message_id, "", "", nullptr, inline_keyboard);
      // Обновляем сохраненный текст
      last_message_text_map[last_message_id] = send_message;
    } else {
      auto message = api.sendMessage(chat_id, send_message, nullptr, nullptr, inline_keyboard);
      last_message_map[chat_id] = message->messageId;
      last_message_text_map[message->messageId] = send_message;  // Сохраняем текст сообщения
      std::cout << "Отправлено сообщение с подтверждением сброса данных с id: " << message->messageId << std::endl;
    }
  }

  void SendAndDeleteMessage(TgBot::Bot& bot, int64_t chat_id, const std::string& message) {
    auto& api = bot.getApi();
    auto sent_message = api.sendMessage(chat_id, message);
    int32_t message_id = sent_message->messageId;

    // Устанавливаем задержку на 0.1 секунды
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Удаляем сообщение
    try {
      api.deleteMessage(chat_id, message_id);
    } catch (const TgBot::TgException& e) {
      std::cerr << "Ошибка при удалении сообщения: " << e.what() << std::endl;
    }
  }
  void ActionStartCommand(TgBot::Message::Ptr msg_ptr) {
    lastMessageMap_.clear();  // Очищаем карту
    const auto chat_id = msg_ptr->chat->id;
    const std::string message =
        "Добро пожаловать в игру Traded!\n\n"
        "Правила игры:\n"
        "1. Вы можете ездить между городами и покупать/продавать товары.\n"
        "2. Изначально ваш баланс составляет 50,000 руб.\n"
        "3. Цель игры - заработать 1,000,000,000 руб.\n\n"
        "Удачи и приятной игры!";
    // std::cout << "Start\n";
    TgBot::InlineKeyboardMarkup::Ptr inline_keyboard = CreateInlineKeyboard();
    SendOrEditMessage(bot_, lastMessageMap_, lastMessageTextMap_, chat_id, message, inline_keyboard);
  }
  void ActionResetCommand(TgBot::Message::Ptr msg_ptr) {
    lastMessageMap_.clear();  // очищаем карту
    const auto chat_id = msg_ptr->chat->id;
    const std::string message = "Ваши данные были сброшены. Баланс: 50,000 руб, город: Москва.";
    // Сбрасываем данные пользователя в базе данных
    UserData reset_data;
    reset_data.money = 50000;
    reset_data.city = 0;
    reset_data.stationery = 0;
    reset_data.vegetables = 0;
    reset_data.textiles = 0;
    reset_data.fertilizers = 0;
    reset_data.wood = 0;
    reset_data.scrap_metal = 0;
    reset_data.medicines = 0;
    reset_data.furniture = 0;
    reset_data.computers = 0;
    reset_data.cars = 0;
    reset_data.trips = 0;

    db_.UpdateUserData(chat_id, reset_data);

    TgBot::InlineKeyboardMarkup::Ptr inline_keyboard = CreateInlineKeyboard();
    SendOrEditMessage(bot_, lastMessageMap_, lastMessageTextMap_, chat_id, message, inline_keyboard);
  }
  void ActionHelpCommand(TgBot::Message::Ptr msg_ptr) {
    const auto chat_id = msg_ptr->chat->id;
    const std::string message =
        "Цель игры заработать 1 000 000 000 руб.\n"
        "---------------------------------------------------------\n"
        "/start - запуск бота\n"
        "/reset - начать игру заново\n"
        "---------------------------------------------------------\n"
        "Покупка и продажа товаров введите в чате\n"
        "простое сообщение без ковычек, пример:\n"
        "---------------------------------------------------------\n"
        "\"купить 3 10\" - где 3 это номер товара 10 количество\n"
        "\"продать 3 10\" - где 3 это номер товара 10 количество\n";
    TgBot::InlineKeyboardMarkup::Ptr inline_keyboard = CreateInlineKeyboard();
    SendOrEditMessage(bot_, lastMessageMap_, lastMessageTextMap_, chat_id, message, inline_keyboard);
  }
  void ActionHelloCommand(TgBot::Message::Ptr msg_ptr) {
    auto& api = bot_.getApi();
    const auto chat_id = msg_ptr->chat->id;
    api.sendMessage(chat_id, "Здравствуйте, " + msg_ptr->chat->username);
  }
  void ActionUnknownCommand(TgBot::Message::Ptr msg_ptr) {
    auto& api = bot_.getApi();
    const auto chat_id = msg_ptr->chat->id;
    api.sendMessage(chat_id, "Неизвестная команда: " + msg_ptr->text);
  }
  void ActionCallbackQuery(TgBot::CallbackQuery::Ptr query) {
    const std::string data = query->data;
    const int64_t chat_id = query->message->chat->id;

    std::string message;
    int destination_city = -1;  // Переменная для хранения выбранного города

    if (data == "my_inventory") {
      ActionMyInventory(chat_id);
      return;
    }
    if (data == "buyButton") {
      ActionBuyGoods(chat_id);  // Вызываем метод для отображения цен на товары
      return;
    }
    if (data == "toMoscowButton") {
      message = "Вы выбрали 'Едем в Москву'. Вы направляетесь в Москву.";
      destination_city = 0;
    } else if (data == "toSPBButton") {
      message = "Вы выбрали 'Едем в Санкт-Петербург'. Вы направляетесь в Санкт-Петербург.";
      destination_city = 1;
    } else if (data == "toKaliningradButton") {
      message = "Вы выбрали 'Едем в Калининград'. Вы направляетесь в Калининград.";
      destination_city = 2;
    } else if (data == "toKazanButton") {
      message = "Вы выбрали 'Едем в Казань'. Вы направляетесь в Казань.";
      destination_city = 3;
    } else if (data == "priceButton") {
      UserData user_data = db_.GetUserData(chat_id);
      std::string current_city = GetCityName(user_data.city);
      message += "Текущий город: " + current_city + "\n";
      message += "---------------------\n";
      message += "Ваши деньги: " + std::to_string(user_data.money) + "\n";
      message += "---------------------\n";
      message += "Стоимость проезда между городами:\n";
      message += "Москва - Санкт-Петербург: " + std::to_string(kTicketPrices[0][1]) + "\n";
      message += "Москва - Калининград: " + std::to_string(kTicketPrices[0][2]) + "\n";
      message += "Москва - Казань: " + std::to_string(kTicketPrices[0][3]) + "\n";
      message += "Санкт-Петербург - Москва: " + std::to_string(kTicketPrices[1][0]) + "\n";
      message += "Санкт-Петербург - Калининград: " + std::to_string(kTicketPrices[1][2]) + "\n";
      message += "Санкт-Петербург - Казань: " + std::to_string(kTicketPrices[1][3]) + "\n";
      message += "Калининград - Москва: " + std::to_string(kTicketPrices[2][0]) + "\n";
      message += "Калининград - Санкт-Петербург: " + std::to_string(kTicketPrices[2][1]) + "\n";
      message += "Калининград - Казань: " + std::to_string(kTicketPrices[2][3]) + "\n";
      message += "Казань - Москва: " + std::to_string(kTicketPrices[3][0]) + "\n";
      message += "Казань - Санкт-Петербург: " + std::to_string(kTicketPrices[3][1]) + "\n";
      message += "Казань - Калининград: " + std::to_string(kTicketPrices[3][2]) + "\n";
    } else if (data == "helpButton") {
      ActionHelpCommand(query->message);
      return;
    }

    if (destination_city != -1) {
      UserData user_data = db_.GetUserData(chat_id);

      if (user_data.city == destination_city) {
        message = "Вы уже находитесь в " + GetCityName(destination_city) +
                  ".\nВаш баланс: " + std::to_string(user_data.money);
      } else {
        int ticket_cost = kTicketPrices[user_data.city][destination_city];

        if (user_data.money >= ticket_cost) {
          user_data.money -= ticket_cost;
          user_data.city = destination_city;
          user_data.trips += 1;

          db_.UpdateUserData(chat_id, user_data);

          message = "Вы выбрали 'Едем в " + GetCityName(destination_city) +
                    "'.\nПереезд выполнен.\nВаш новый баланс: " + std::to_string(user_data.money) +
                    "\nВы можете зайти на рынок, чтобы посмотреть товары в городе.";

        } else {
          message = "У вас недостаточно средств для поездки.\nВаш баланс: " + std::to_string(user_data.money);
        }
      }
    }
    TgBot::InlineKeyboardMarkup::Ptr inline_keyboard = CreateInlineKeyboard();
    SendOrEditMessage(bot_, lastMessageMap_, lastMessageTextMap_, chat_id, message, inline_keyboard);
  }
  void ActionAnyMessage(TgBot::Message::Ptr msg_ptr) {
    const auto chat_id = msg_ptr->chat->id;
    const int message_id = msg_ptr->messageId;
    const std::string text = msg_ptr->text;
    if (text == "/start") {  // Для обхода дублирования команд
      // std::cout << "Start из any\n";
      return;
    }
    // Проверка на команду "купить" или "продать" с параметрами
    if (text.rfind("купить ", 0) == 0 || text.rfind("Купить ", 0) == 0) {
      std::string command = text.substr(text.find(' ') + 1);  // удаляем "купить " или "Купить "
      std::istringstream iss(command);
      int item_number = 0;
      int quantity = 0;
      if (iss >> item_number >> quantity) {
        std::cout << "Команда покупки обработана: " << text << std::endl;
        ActionTransaction(chat_id, item_number, quantity, true);
      } else {
        std::cout << "Ошибка формата команды покупки: " << text << std::endl;
      }
    } else if (text.rfind("продать ", 0) == 0 || text.rfind("Продать ", 0) == 0) {
      std::string command = text.substr(text.find(' ') + 1);  // удаляем "продать " или "Продать "
      std::istringstream iss(command);
      int item_number = 0;
      int quantity = 0;
      if (iss >> item_number >> quantity) {
        std::cout << "Команда продажи обработана: " << text << std::endl;
        ActionTransaction(chat_id, item_number, quantity, false);
      } else {
        std::cout << "Ошибка формата команды продажи: " << text << std::endl;
      }
    }
    // Удаляем исходное сообщение
    auto& api = bot_.getApi();
    try {
      api.deleteMessage(chat_id, message_id);
    } catch (const TgBot::TgException& e) {
      std::cerr << "Ошибка при удалении сообщения: " << e.what() << std::endl;
    }
    std::cout << "Получено сообщение: " << text << std::endl;
    // Проверка достижения цели
    UserData user_data = db_.GetUserData(chat_id);
    if (user_data.money >= 1000000000) {
      SendVictoryMessage(chat_id, user_data.trips);
    } else {
      ActionBuyGoods(chat_id);
    }
  }
  void ActionTransaction(int64_t chat_id, int item_number, int quantity, bool is_buying) {
    UserData user_data = db_.GetUserData(chat_id);

    if (kCityPrices.find(user_data.city) == kCityPrices.end() || item_number <= 0 || item_number > 10) {
      std::cout << "Некорректный номер товара: " << item_number << std::endl;
      return;
    }

    const auto& prices = kCityPrices.at(user_data.city);
    int price = prices[item_number - 1];
    int total_cost = price * quantity;

    std::unordered_map<int, std::function<int&(UserData&)>> item_quantity_map = {
        {1, [](UserData& data) -> int& { return data.stationery; }},
        {2, [](UserData& data) -> int& { return data.vegetables; }},
        {3, [](UserData& data) -> int& { return data.textiles; }},
        {4, [](UserData& data) -> int& { return data.fertilizers; }},
        {5, [](UserData& data) -> int& { return data.wood; }},
        {6, [](UserData& data) -> int& { return data.scrap_metal; }},
        {7, [](UserData& data) -> int& { return data.medicines; }},
        {8, [](UserData& data) -> int& { return data.furniture; }},
        {9, [](UserData& data) -> int& { return data.computers; }},
        {10, [](UserData& data) -> int& { return data.cars; }}};

    auto& item_quantity = item_quantity_map[item_number](user_data);

    if (is_buying) {
      if (user_data.money < total_cost) {
        SendAndDeleteMessage(bot_, chat_id, "Недостаточно денег для покупки.");
        std::cout << "Недостаточно денег для покупки. Баланс: " << user_data.money << std::endl;
        return;
      }
      user_data.money -= total_cost;
      item_quantity += quantity;
      SendAndDeleteMessage(bot_, chat_id, "Покупка выполнена.");
      std::cout << "Покупка выполнена. Остаток средств: " << user_data.money << std::endl;
    } else {
      if (quantity > item_quantity) {
        SendAndDeleteMessage(bot_, chat_id, "Недостаточно товара для продажи.");
        std::cout << "Недостаточно товара для продажи. Имеется: " << item_quantity << std::endl;
        return;
      }
      user_data.money += total_cost;
      item_quantity -= quantity;
      SendAndDeleteMessage(bot_, chat_id, "Продажа выполнена.");
      std::cout << "Продажа выполнена. Новый баланс: " << user_data.money << std::endl;
    }

    db_.UpdateUserData(chat_id, user_data);
  }
  void ActionBuyGoods(int64_t chat_id) {
    UserData user_data = db_.GetUserData(chat_id);  // Получаем данные пользователя из базы данных

    // Получаем текущий город пользователя
    int city_id = user_data.city;
    std::string current_city = GetCityName(user_data.city);
    std::string message = "Цены на товары в вашем текущем городе: " + current_city + "\n";

    // Сообщение с ценами на товары
    if (kCityPrices.find(city_id) != kCityPrices.end()) {
      const auto& prices = kCityPrices.at(city_id);
      message += "---------------------\n";
      message += "Ваши деньги: " + std::to_string(user_data.money) + "\n";
      message += "---------------------\n";
      message += "1. Канцтовары: " + std::to_string(prices[0]) + " руб. " +
                 "| У вас: " + std::to_string(user_data.stationery) + " шт.\n";
      message += "2. Овощи: " + std::to_string(prices[1]) + " руб. " +
                 "| У вас: " + std::to_string(user_data.vegetables) + " шт.\n";
      message += "3. Текстиль: " + std::to_string(prices[2]) + " руб. " +
                 "| У вас: " + std::to_string(user_data.textiles) + " шт.\n";
      message += "4. Удобрения: " + std::to_string(prices[3]) + " руб. " +
                 "| У вас: " + std::to_string(user_data.fertilizers) + " шт.\n";
      message += "5. Древесина: " + std::to_string(prices[4]) + " руб. " +
                 "| У вас: " + std::to_string(user_data.wood) + " шт.\n";
      message += "6. Металлолом: " + std::to_string(prices[5]) + " руб. " +
                 "| У вас: " + std::to_string(user_data.scrap_metal) + " шт.\n";
      message += "7. Лекарства: " + std::to_string(prices[6]) + " руб. " +
                 "| У вас: " + std::to_string(user_data.medicines) + " шт.\n";
      message += "8. Мебель: " + std::to_string(prices[7]) + " руб. " +
                 "| У вас: " + std::to_string(user_data.furniture) + " шт.\n";
      message += "9. Компьютеры: " + std::to_string(prices[8]) + " руб. " +
                 "| У вас: " + std::to_string(user_data.computers) + " шт.\n";
      message += "10. Автомобили: " + std::to_string(prices[9]) + " руб. " +
                 "| У вас: " + std::to_string(user_data.cars) + " шт.\n";
      message += "---------------------\n";
      message += "Для покупки или продажи товара введите в чат:\n";
      message += "Купить номер товара количество\n";
      message += "Пример: купить 3 10\n";
      message += "Пример: продать 7 15\n";
    } else {
      message += "Не удалось получить цены на товары для вашего города.";
    }

    TgBot::InlineKeyboardMarkup::Ptr inline_keyboard = CreateInlineKeyboard();
    SendOrEditMessage(bot_, lastMessageMap_, lastMessageTextMap_, chat_id, message, inline_keyboard);
  }

  void ActionMyInventory(int64_t chat_id) {
    UserData user_data = db_.GetUserData(chat_id);  // Получаем данные пользователя из базы данны
    std::string message = "Ваши товары:\n";
    message += "Город: " + GetCityName(user_data.city) + "\n";
    message += "---------------------\n";
    message += "Ваши деньги: " + std::to_string(user_data.money) + "\n";
    message += "---------------------\n";
    message += "1. Канцтовары: " + std::to_string(user_data.stationery) + " шт." + "\n";
    message += "2. Овощи: " + std::to_string(user_data.vegetables) + " шт." + "\n";
    message += "3. Текстиль: " + std::to_string(user_data.textiles) + " шт." + "\n";
    message += "4. Удобрения: " + std::to_string(user_data.fertilizers) + " шт." + "\n";
    message += "5. Древесина: " + std::to_string(user_data.wood) + " шт." + "\n";
    message += "6. Металлолом: " + std::to_string(user_data.scrap_metal) + " шт." + "\n";
    message += "7. Лекарства: " + std::to_string(user_data.medicines) + " шт." + "\n";
    message += "8. Мебель: " + std::to_string(user_data.furniture) + " шт." + "\n";
    message += "9. Компьютеры: " + std::to_string(user_data.computers) + " шт." + "\n";
    message += "10. Автомобили: " + std::to_string(user_data.cars) + " шт." + "\n";
    message += "Поездки: " + std::to_string(user_data.trips) + " шт." + "\n";

    TgBot::InlineKeyboardMarkup::Ptr inline_keyboard = CreateInlineKeyboard();
    SendOrEditMessage(bot_, lastMessageMap_, lastMessageTextMap_, chat_id, message, inline_keyboard);
  }

 public:
  explicit GameBot(const std::string& api_token) : bot_(api_token), db_("gamebot.db") {
    bot_.getEvents().onCommand("start", [this](TgBot::Message::Ptr msg_ptr) { ActionStartCommand(msg_ptr); });
    bot_.getEvents().onCommand("help", [this](TgBot::Message::Ptr msg_ptr) { ActionHelpCommand(msg_ptr); });
    bot_.getEvents().onCommand("hello", [this](TgBot::Message::Ptr msg_ptr) { ActionHelloCommand(msg_ptr); });
    bot_.getEvents().onCommand("reset", [this](TgBot::Message::Ptr msg_ptr) { ActionResetCommand(msg_ptr); });
    bot_.getEvents().onUnknownCommand([this](TgBot::Message::Ptr msg_ptr) { ActionUnknownCommand(msg_ptr); });
    bot_.getEvents().onCallbackQuery([this](TgBot::CallbackQuery::Ptr query) { ActionCallbackQuery(query); });
    bot_.getEvents().onAnyMessage([this](TgBot::Message::Ptr msg_ptr) { ActionAnyMessage(msg_ptr); });
  }

  void Run() {
    auto& api = bot_.getApi();
    std::cout << "Программа работает..." << std::endl;
    api.deleteWebhook();
    TgBot::TgLongPoll long_poll(bot_);

    while (true) {
      try {
        long_poll.start();
      } catch (const TgBot::TgException& err) {
        std::cerr << "Ошибка TgBot: " << err.what() << '\n';
        continue;
      }
    }
  }
};
#endif
