// Поиск в отсортированном массиве одинакового количества элементов
#include <algorithm>
#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  size_t count = 0;
  std::cin >> count;
  std::vector<int> colors(count);
  for (int& x : colors) {
    std::cin >> x;
  }

  size_t com = 0;
  std::cin >> com;
  for (size_t i = 0; i < com; i++) {
    int quert = 0;
    std::cin >> quert;
    const auto range = std::equal_range(colors.begin(), colors.end(), quert);
    std::cout << range.second - range.first << '\n';
  }
  return 0;
}