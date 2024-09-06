// QuickSort - Ломуто
#include <iostream>
bool compare(int& left, int& pivot) {
  return left >= pivot;
}
void Print(int* mas, size_t size) {
  for (size_t i = 0; i < size; i++) {
    std::cout << mas[i] << '\n';
  }
}
int Partition(int* mas, int left, int right) {
  int pivot = mas[right];
  int i = (left - 1);
  for (int j = left; j <= right - 1; j++) {
        if (compare(mas[j],pivot)) {
      i++;
      std::swap(mas[i], mas[j]);
        }
  }
  std::swap(mas[i + 1], mas[right]);
  return (i + 1);
}
void QuickSort(int* mas, int left, int right) {
  if (left < right) {
    int midIndex = Partition(mas, left, right);
    QuickSort(mas, left, midIndex - 1);
    QuickSort(mas, midIndex + 1, right);
  }
}
int main() {
  size_t size = 0;
  std::cin >> size;
  int* mas = new int[size];
  for (size_t i = 0; i < size; i++) {
    std::cin >> mas[i];
  }
  QuickSort(mas, 0, size - 1);
  Print(mas, size);
  delete[] mas;
  return 0;
}