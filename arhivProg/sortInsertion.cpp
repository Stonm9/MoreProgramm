// Сортировка вставками
#include <iostream>
bool compare(int& mas, int& mas2) {
  return mas < mas2;
}
int* Sort(int* mas, int count) {
  for (int i = 0; i < count - 1; i++) {
    for (int j = i + 1; j > 0 && compare(mas[j], mas[j - 1]); j--) {
      std::swap(mas[j - 1], mas[j]);
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
  // Insertion Sort
  Sort(mas, count);

  // Print
  for (int i = 0; i < count; ++i) {
    std::cout << mas[i] << '\n';
  }
  delete[] mas;
}