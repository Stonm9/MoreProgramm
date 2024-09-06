// Проект собираться как в ручную так и с помощью Cmake
// cmake ..
// make telegram_bot
// make telegram_bot_test
// make telegram_bot_readBd
// ----------------------------------------
// Комилируеться в ручную с помощью g++ -fsanitize=address main.cpp database.cpp -o telegram_bot --std=c++14
// -I/usr/local/include -lTgBot -lboost_system -lssl -lcrypto -lpthread -lsqlite3 Курс ФПМИ МФТИ "Разработка на С++"
// студент Петров Денис Николаевич, финальный проект.
#include "gamebot.h"
#include "token.h"

int main() {
  GameBot bot(kEy);
  bot.Run();
  return 0;
}