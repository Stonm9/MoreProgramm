// Числа фибоначи
#include <iostream>
#include <cmath>
void Fibonachi(int& count) {
  int prev = 0;
  int current = 1;
  int next = prev + current;

  if (count == 1) {
    std::cout << 0 << '\n';
  }
  if (count == 2) {
    std::cout << "0 1" << '\n';
  }
  if (count > 2) {
    std::cout << "0 1 ";
    for (size_t i = 2; i < count; i++) {
      next = prev + current;
      std::cout << next << " ";
      prev = current;
      current = next;
    }
  }
}
int main() {
  int count = 0;
  std::cin >> count;
  Fibonachi(count);
  return 0;
}