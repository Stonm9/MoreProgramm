// База данных
// ----------------------------------------
#include <sqlite3.h>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "database.h"

Database::Database(const std::string& db_name) {
  if (sqlite3_open(db_name.c_str(), &db_) != SQLITE_OK) {
    throw std::runtime_error("Не удаётся открыть базу данных");
  }
  CreateTable();
}

Database::~Database() {
  sqlite3_close(db_);
}

void Database::CreateTable() {
  const char* create_table_query =
      "CREATE TABLE IF NOT EXISTS Users("
      "chat_id INT PRIMARY KEY, "
      "money INT, stationery INT, vegetables INT, textiles INT, fertilizers INT, "
      "wood INT, scrap_metal INT, medicines INT, furniture INT, computers INT, "
      "cars INT, trips INT, city INT);";

  char* err_msg = nullptr;
  if (sqlite3_exec(db_, create_table_query, nullptr, nullptr, &err_msg) != SQLITE_OK) {
    std::string error = "Ошибка SQL: ";
    error += err_msg;
    sqlite3_free(err_msg);
    throw std::runtime_error(error);
  }
}

void Database::AddUser(int64_t chat_id) {
  const char* insert_query =
      "INSERT INTO Users (chat_id, money, stationery, vegetables, textiles, fertilizers, "
      "wood, scrap_metal, medicines, furniture, computers, cars, trips, city) "
      "VALUES (?, 50000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);";

  sqlite3_stmt* stmt = nullptr;
  sqlite3_prepare_v2(db_, insert_query, -1, &stmt, nullptr);
  sqlite3_bind_int64(stmt, 1, chat_id);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    throw std::runtime_error("Не удаётся добавить пользователя");
  }

  sqlite3_finalize(stmt);
}

UserData Database::GetUserData(int64_t chat_id) {
  const char* select_query = "SELECT * FROM Users WHERE chat_id = ?;";
  sqlite3_stmt* stmt = nullptr;
  sqlite3_prepare_v2(db_, select_query, -1, &stmt, nullptr);
  sqlite3_bind_int64(stmt, 1, chat_id);

  UserData user_data;
  if (sqlite3_step(stmt) == SQLITE_ROW) {
    user_data.money = sqlite3_column_int(stmt, 1);
    user_data.stationery = sqlite3_column_int(stmt, 2);
    user_data.vegetables = sqlite3_column_int(stmt, 3);
    user_data.textiles = sqlite3_column_int(stmt, 4);
    user_data.fertilizers = sqlite3_column_int(stmt, 5);
    user_data.wood = sqlite3_column_int(stmt, 6);
    user_data.scrap_metal = sqlite3_column_int(stmt, 7);
    user_data.medicines = sqlite3_column_int(stmt, 8);
    user_data.furniture = sqlite3_column_int(stmt, 9);
    user_data.computers = sqlite3_column_int(stmt, 10);
    user_data.cars = sqlite3_column_int(stmt, 11);
    user_data.trips = sqlite3_column_int(stmt, 12);
    user_data.city = sqlite3_column_int(stmt, 13);
  } else {
    AddUser(chat_id);
    user_data = GetUserData(chat_id);
  }

  sqlite3_finalize(stmt);
  return user_data;
}

void Database::UpdateUserData(int64_t chat_id, const UserData& user_data) {
  const char* update_query =
      "UPDATE Users SET money = ?, stationery = ?, vegetables = ?, textiles = ?, fertilizers = ?, "
      "wood = ?, scrap_metal = ?, medicines = ?, furniture = ?, computers = ?, cars = ?, trips = ?, city = ? "
      "WHERE chat_id = ?;";

  sqlite3_stmt* stmt = nullptr;
  sqlite3_prepare_v2(db_, update_query, -1, &stmt, nullptr);

  sqlite3_bind_int(stmt, 1, user_data.money);
  sqlite3_bind_int(stmt, 2, user_data.stationery);
  sqlite3_bind_int(stmt, 3, user_data.vegetables);
  sqlite3_bind_int(stmt, 4, user_data.textiles);
  sqlite3_bind_int(stmt, 5, user_data.fertilizers);
  sqlite3_bind_int(stmt, 6, user_data.wood);
  sqlite3_bind_int(stmt, 7, user_data.scrap_metal);
  sqlite3_bind_int(stmt, 8, user_data.medicines);
  sqlite3_bind_int(stmt, 9, user_data.furniture);
  sqlite3_bind_int(stmt, 10, user_data.computers);
  sqlite3_bind_int(stmt, 11, user_data.cars);
  sqlite3_bind_int(stmt, 12, user_data.trips);
  sqlite3_bind_int(stmt, 13, user_data.city);
  sqlite3_bind_int64(stmt, 14, chat_id);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    throw std::runtime_error("Не удаётся обновить данные пользователя");
  }

  sqlite3_finalize(stmt);
}
