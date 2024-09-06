// Реализация алгоритма сортировка слиянием
#include <iostream>

bool compare(int& mas1, int& mas2) {
  return mas1 > mas2;
}
void Merge(int* mas, int beginIndex, int mid, int endIndex, int* buffer) {
  int left = beginIndex;
  int right = mid;
  int i = 0;
  while (left < mid && right < endIndex) {
    if (compare(mas[left], mas[right])) {
      buffer[i] = mas[left];
      i++;
      left++;
    } else {
      buffer[i] = mas[right];
      i++;
      right++;
    }
  }
  while (left < mid) {
    buffer[i] = mas[left];
    i++;
    left++;
  }
  while (right < endIndex) {
    buffer[i] = mas[right];
    i++;
    right++;
  }
  for (int id = beginIndex; id < endIndex; id++) {
    mas[id] = buffer[id - beginIndex];
  }
}
int* MergeSort(int* mas, int beginIndex, int endIndex, int* buffer) {
  if (endIndex - beginIndex <= 1) {
  } else {
    int mid = (beginIndex + endIndex) / 2;
    MergeSort(mas, beginIndex, mid, buffer);
    MergeSort(mas, mid, endIndex, buffer);
    Merge(mas, beginIndex, mid, endIndex, buffer);
  }
  return mas;
}
void Print(int* mas, size_t& size) {
  for (size_t i = 0; i < size; i++) {
    std::cout << mas[i] << '\n';
  }
}
int main() {
  size_t size = 0;
  std::cin >> size;
  int* mas = new int[size];     // Основной массив
  int* buffer = new int[size];  // Буферный массив
  for (size_t i = 0; i < size; i++) {
    std::cin >> mas[i];
  }

  MergeSort(mas, 0, size, buffer);
  Print(mas, size);
  delete[] mas;
  delete[] buffer;
  return 0;
}