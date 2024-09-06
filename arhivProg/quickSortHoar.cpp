// QuickSort - Хоар
#include <iostream>
void Print(int* mas, size_t size) {
  for (size_t i = 0; i < size; i++) {
    std::cout << mas[i] << '\n';
  }
}
int Partition(int* mas, int left, int right) {
  int pivot = mas[left];
  int leftPart = left - 1;
  int rightPart = right + 1;
  while (true) {
    do {
      leftPart++;
    } while (mas[leftPart] > pivot);
    do {
      rightPart--;
    } while (mas[rightPart] < pivot);
    if (leftPart >= rightPart) {
      return rightPart;
    }
    std::swap(mas[leftPart], mas[rightPart]);
  }
}
void QuickSort(int* mas, int left, int right) {
  if (left < right) {
    int midIndex = Partition(mas, left, right);
    QuickSort(mas, left, midIndex);
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