// Реализация размена денег основываясь на номинале купюр
#include <iostream>
void Exchenge(const int size, int* money, int* count, int summa) {
  while (summa != 0) {
    for (int i = 0; i < size; i++) {
      if (summa >= money[i]) {
        count[i]++;
        summa = summa - money[i];
        break;
      }
    }
  }
}
int main() {
  int summa = 0;
  const int size = 11;
  int money[size]{5000, 2000, 1000, 500, 200, 100, 50, 10, 5, 2, 1};
  int count[size]{0};

  std::cin >> summa;
  Exchenge(size, money, count, summa);
  for (int i = size - 1; i >= 0; i--) {
    std::cout << money[i] << "руб - " << count[i];
    if (i != 0) {
      std::cout << ", ";
    }
  }
  return 0;
}