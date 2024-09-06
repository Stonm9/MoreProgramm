// Встреча - просто поиск в неотсортированном массиве повторений
#include <algorithm>
#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  size_t count = 0;
  std::cin >> count;
  std::vector<int> v(count);
  for (auto& x : v) {
    std::cin >> x;
  }
  int x = 0;
  std::cin >> x;
  std::cout << std::count(v.begin(), v.end(), x) << '\n';
  return 0;
}