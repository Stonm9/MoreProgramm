// По верхней границе
// Set это множество, хранит данные в отсортированном виде
#include <iostream>
#include <set>
class Set {
  std::set<int> s_;

 public:
  void Add(int x) {
    s_.insert(x);
  }
  int Next(int x) const {
    const auto it = s_.lower_bound(x);
    return it != s_.end() ? *it : -1;
  }
};
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  Set numbers;
  size_t count = 0;
  std::cin >> count;
  int prev = 0;
  for (size_t i = 0; i < count; i++) {
    char command = '0';
    int x = 0;
    std::cin >> command >> x;
    if (command == '+') {
      numbers.Add((x + prev) % 1'000'000'000);
      prev = 0;
    } else if (command == '?') {
      prev = numbers.Next(x);
      std::cout << numbers.Next(x) << '\n';
    }
  }
  return 0;
}