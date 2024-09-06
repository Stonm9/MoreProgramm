// const int count = 10;
// int mas[count]{-82,17,-77,73,-74,-84,58,61,-5,-43};
// Сортировка выбором
#include <iostream>

bool compare(int& mas, int& mas2) {
  return mas < mas2;
}
int* Sort(int* mas, int count) {
  for (int i = 0; i < count; ++i) {
    for (int j = i; j < count; ++j) {
      if (compare(mas[j], mas[i])) {
        std::swap(mas[j], mas[i]);
      }
    }
  }
  return mas;
}
int main() {
  int count = 0;
  std::cin >> count;
  int* mas = new int[count];
  // Input
  for (int i = 0; i < count; i++) {
    std::cin >> mas[i];
  }
  // Sort
  Sort(mas, count);
  // Print
  for (int i = 0; i < count; ++i) {
    std::cout << mas[i] << '\n';
  }
  delete[] mas;
}