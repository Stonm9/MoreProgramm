// Банк
// Можно добавляють и средства на счет пользователя если его нету создаеться новый пользователь
#include <iostream>
#include <unordered_map>
#include <string>
class Bank {
  std::unordered_map<std::string, int> accounts_;

 public:
  void Put(const std::string& name, int amount) {
    accounts_[name] += amount;
  }
  std::pair<bool, int> Get(const std::string& name) const {

    const auto it = accounts_.find(name);
    return it != accounts_.end() ? std::pair(true, it->second) : std::pair(false, 0);
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  Bank bank;
  size_t count = 0;
  std::cin >> count;
  for (size_t i = 0; i < count; i++) {
    int command = 0;
    std::string name;
    std::cin >> command >> name;
    if (command == 1) {
      int amount = 0;
      std::cin >> amount;
      bank.Put(name, amount);
    } else if (command == 2) {
      const auto res = bank.Get(name);
      if (res.first) {
        std::cout << res.second << '\n';
      } else {
        std::cout << "ERROR\n";
      }
    }
  }

  return 0;
}