// База данных
#include <iostream>
#include <string>
#include <map>
class Database {
  std::map<std::string, std::map<std::string, size_t>> clients_;

 public:
  void Add(const std::string& name, const std::string& good, size_t amount) {
    clients_[name][good] += amount;
  }
  void Print() const {
    for (const auto& client : clients_) {
      std::cout << client.first << ":\n";
      for (const auto& item : client.second) {
        std::cout << item.first << ' ' << item.second << '\n';
      }
    }
  }
};
int main() {
  std::string name;
  std::string good;
  size_t amount = 0;
  Database base;
  while (true) {
    if (!(std::cin >> name >> good >> amount)) {
      break;
    }
    base.Add(name, good, amount);
  }
  base.Print();
  return 0;
}