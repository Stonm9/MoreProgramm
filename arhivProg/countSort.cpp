// countSort - сортировка подсчетом
#include <iostream>
void Print(int* mas, size_t size) {
  for (size_t i = 0; i < size; i++) {
    std::cout << mas[i] << '\n';
  }
}
void CountSort(int* mas, size_t size) {
  int max = mas[0];
  // Ищем максимум в массиве
  for (size_t i = 0; i < size; i++) {
    if (mas[i] > max) {
      max = mas[i];
    }
  }
  int* count = new int[max + 1]{0};
  // Считаем значения в массиве
  for (size_t i = 0; i < size; i++) {
    count[mas[i]]++;
  }
  // Сортируем массив
  int countMas = 0;
  for (size_t i = 0; i < max + 1; i++) {
    if (count[i] != 0) {
      for (size_t j = 0; j < count[i]; j++) {
        mas[countMas++] = i;
      }
    }
  }
  delete[] count;
}
int main() {
  size_t size = 0;
  std::cin >> size;
  int* mas = new int[size];
  for (size_t i = 0; i < size; i++) {
    std::cin >> mas[i];
  }
  CountSort(mas, size);
  Print(mas, size);
  delete[] mas;
  return 0;
}