// База данных, заголовочный файл
// ----------------------------------------
#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <stdexcept>
#include <string>

struct UserData {
  int money;
  int stationery;
  int vegetables;
  int textiles;
  int fertilizers;
  int wood;
  int scrap_metal;
  int medicines;
  int furniture;
  int computers;
  int cars;
  int trips;
  int city;
};

class Database {
 public:
  Database(const std::string& db_name);  // NOLINT
  ~Database();

  void CreateTable();
  void AddUser(int64_t chat_id);
  UserData GetUserData(int64_t chat_id);
  void UpdateUserData(int64_t chat_id, const UserData& user_data);

 private:
  sqlite3* db_;
};

#endif
