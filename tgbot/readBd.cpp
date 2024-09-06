// Это файл для чтения содержания из БД, для просмотра содержимого
// ----------------------------------------
// Комилируеться в ручную с помощью g++ readBd.cpp -lsqlite3

#include <iostream>
#include <sqlite3.h>
#include <string>

// Имя базы данных
const std::string kDbName = "gamebot.db";

// Функция обратного вызова для обработки результатов запроса
int Callback(void*, int argc, char** argv, char** az_col_name) {
  for (int i = 0; i < argc; i++) {
    std::cout << az_col_name[i] << " = " << (argv[i] ? argv[i] : "NULL") << " | ";
  }
  std::cout << std::endl;
  return 0;
}

void DumpDatabase() {
  sqlite3* db = nullptr;
  char* err_msg = nullptr;

  // Открытие базы данных
  if (sqlite3_open(kDbName.c_str(), &db) != SQLITE_OK) {
    std::cerr << "Не удаётся открыть базу данных: " << sqlite3_errmsg(db) << std::endl;
    return;
  }

  // SQL запрос для получения всех данных из таблицы Users
  const char* query = "SELECT * FROM Users;";

  // Выполнение запроса
  if (sqlite3_exec(db, query, Callback, nullptr, &err_msg) != SQLITE_OK) {
    std::cerr << "SQL ошибка: " << err_msg << std::endl;
    sqlite3_free(err_msg);
  }
  sqlite3_close(db);
}

int main() {
  DumpDatabase();
  return 0;
}