// Тесты
// Обязательно поменйте chatId на свой реальный из tgbota иначе тесты работать не будут.
// ----------------------------------------
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "gamebot.h"
#include "token.h"
// Этот класc нужен исключительно для того чтобы получить доступ к protected полям и методам для теста
class GameBotTest : public GameBot {
 public:
  explicit GameBotTest(const std::string& token) : GameBot(token) {
  }
  using GameBot::ActionCallbackQuery;
  using GameBot::ActionMyInventory;
  using GameBot::ActionResetCommand;
  using GameBot::ActionStartCommand;
  using GameBot::ActionTransaction;
  using GameBot::CreateInlineKeyboard;
  using GameBot::db_;
  using GameBot::lastMessageMap_;
  using GameBot::lastMessageTextMap_;
};

const int64_t kChatId = 1063015615;  // Обяазательно поменять на свой chatId !!!

TEST_CASE("createInlineKeyboard") {
  GameBotTest bot(kEy);

  auto keyboard = bot.CreateInlineKeyboard();

  // Проверяем количество кнопок в клавиатуре
  REQUIRE(keyboard->inlineKeyboard.size() == 4);

  // Проверяем количество кнопок в каждом ряду
  REQUIRE(keyboard->inlineKeyboard[0].size() == 2);
  REQUIRE(keyboard->inlineKeyboard[1].size() == 2);
  REQUIRE(keyboard->inlineKeyboard[2].size() == 2);
  REQUIRE(keyboard->inlineKeyboard[3].size() == 2);
}

TEST_CASE("ActionTransaction_buy") {
  GameBotTest bot(kEy);

  // Создаем пользователя с балансом 50000
  UserData user_data = {50000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  bot.db_.UpdateUserData(kChatId, user_data);

  // Покупаем 2 товара с номером 1 по цене 22 за единицу
  bot.ActionTransaction(kChatId, 1, 2, true);

  // Проверяем, что баланс и количество товара изменились корректно
  auto updated_data = bot.db_.GetUserData(kChatId);
  REQUIRE(updated_data.money == 49956);   // 1500 - 2 * 22
  REQUIRE(updated_data.stationery == 2);  // Куплено 2 товара
}

TEST_CASE("ActionTransaction_sell") {
  GameBotTest bot(kEy);

  // Создаем пользователя с балансом 1000 и 2 товарами
  UserData user_data = {1000, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  bot.db_.UpdateUserData(kChatId, user_data);

  // Продаем 1 товар с номером 1 по цене 22 за единицу
  bot.ActionTransaction(kChatId, 1, 1, false);

  // Проверяем что баланс и количество товара изменились
  auto updated_data = bot.db_.GetUserData(kChatId);
  REQUIRE(updated_data.money == 1022);    // 1000 + 22
  REQUIRE(updated_data.stationery == 1);  // Продано 1 товар
}

TEST_CASE("ActionStartCommand") {
  GameBotTest bot(kEy);
  TgBot::Message::Ptr msg_ptr(new TgBot::Message);
  msg_ptr->chat = std::make_shared<TgBot::Chat>();
  msg_ptr->chat->id = kChatId;

  bot.ActionStartCommand(msg_ptr);

  // Проверка начального сообщения и данных пользователя
  auto updated_data = bot.db_.GetUserData(msg_ptr->chat->id);
  REQUIRE(updated_data.money == 1022);  // Баланс не должен изменяться
  REQUIRE(updated_data.city == 0);      // Город не должен поменяться
}

TEST_CASE("ActionResetCommand") {
  GameBotTest bot(kEy);
  TgBot::Message::Ptr msg_ptr(new TgBot::Message);
  msg_ptr->chat = std::make_shared<TgBot::Chat>();
  msg_ptr->chat->id = kChatId;

  // Создаем пользователя с балансом 1000 и 2 товарами и городом Казань
  UserData user_data = {1000, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3};
  bot.db_.UpdateUserData(msg_ptr->chat->id, user_data);

  bot.ActionResetCommand(msg_ptr);

  // Проверка сброшенных данных пользователя
  auto updated_data = bot.db_.GetUserData(msg_ptr->chat->id);
  REQUIRE(updated_data.money == 50000);   // Баланс после сброса
  REQUIRE(updated_data.city == 0);        // Город после сброса (Москва)
  REQUIRE(updated_data.stationery == 0);  // Товары после сброса
}

TEST_CASE("ActionCallbackQuery_toSPBButton") {
  // Едем в Санкт-Петербург
  GameBotTest bot(kEy);
  TgBot::CallbackQuery::Ptr query(new TgBot::CallbackQuery);
  query->message = std::make_shared<TgBot::Message>();
  query->message->chat = std::make_shared<TgBot::Chat>();
  query->message->chat->id = kChatId;
  query->data = "toSPBButton";

  bot.ActionCallbackQuery(query);

  // Проверка, что город пользователя изменился на Санкт-Петербург
  auto updated_data = bot.db_.GetUserData(query->message->chat->id);
  REQUIRE(updated_data.city == 1);
}
TEST_CASE("ActionCallbackQuery_toMoscow") {
  // Едем в Обратно в Москку
  GameBotTest bot(kEy);
  TgBot::CallbackQuery::Ptr query(new TgBot::CallbackQuery);
  query->message = std::make_shared<TgBot::Message>();
  query->message->chat = std::make_shared<TgBot::Chat>();
  query->message->chat->id = kChatId;
  query->data = "toMoscowButton";

  bot.ActionCallbackQuery(query);

  // Проверка, что город пользователя изменился на Москву
  auto updated_data = bot.db_.GetUserData(query->message->chat->id);
  REQUIRE(updated_data.city == 0);
}
TEST_CASE("ActionMyInventory") {
  GameBotTest bot(kEy);

  // Создаем пользователя с разными данными в городе Санкт-питербург
  UserData user_data = {1500, 5, 10, 2, 3, 1, 7, 8, 4, 6, 10, 15, 1};
  bot.db_.UpdateUserData(kChatId, user_data);

  // Вызываем метод для получения инвентаря
  bot.ActionMyInventory(kChatId);
  std::string expected_message =
      "Ваши товары:\n"
      "Город: Санкт-Петербург\n"
      "---------------------\n"
      "Ваши деньги: 1500\n"
      "---------------------\n"
      "1. Канцтовары: 5 шт.\n"
      "2. Овощи: 10 шт.\n"
      "3. Текстиль: 2 шт.\n"
      "4. Удобрения: 3 шт.\n"
      "5. Древесина: 1 шт.\n"
      "6. Металлолом: 7 шт.\n"
      "7. Лекарства: 8 шт.\n"
      "8. Мебель: 4 шт.\n"
      "9. Компьютеры: 6 шт.\n"
      "10. Автомобили: 10 шт.\n"
      "Поездки: 15 шт.\n";

  // Получаем ID последнего сообщения из lastMessageMap
  int64_t last_message_id = bot.lastMessageMap_[kChatId];

  // Проверяем, что текст сообщения правильный
  std::string sent_message = bot.lastMessageTextMap_[last_message_id];
  REQUIRE(sent_message == expected_message);
}