// Реализация структуры данных "max-heap" с сортировкой.
#include <iostream>
#include <cstring>
#include <vector>

class Heap {
  std::vector<int> dynamcik;

 public:
  void Push(int number) {
    dynamcik.push_back(number);
    SiftUp(dynamcik.size() - 1);
  }

  void MakeHeap(int* arr, int size) {
    for (size_t i = 0; i < size; i++) {
      Push(arr[i]);
    }
    for (int i = dynamcik.size() / 2 - 1; i >= 0; i--) {
      SiftDown(i);
    }
  }

  int GetMax() {
    return dynamcik[0];
  }

  void SiftUp(int index) {
    int parent = (index - 1) / 2;
    while (index > 0 && dynamcik[parent] < dynamcik[index]) {
      std::swap(dynamcik[parent], dynamcik[index]);
      index = parent;
      parent = (index - 1) / 2;
    }
  }

  void SiftDown(int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int Max = index;

    if (left < dynamcik.size() && dynamcik[left] > dynamcik[Max]) {
      Max = left;
    }

    if (right < dynamcik.size() && dynamcik[right] > dynamcik[Max]) {
      Max = right;
    }

    if (Max != index) {
      std::swap(dynamcik[index], dynamcik[Max]);
      SiftDown(Max);
    }
  }

  void PopMax() {
    if (dynamcik.empty()) {
      return;
    }
    std::swap(dynamcik[0], dynamcik[dynamcik.size() - 1]);
    dynamcik.pop_back();
    if (!dynamcik.empty()) {
      SiftDown(0);
    }
  }
  int PopMin() {
    int min = dynamcik[0];
    std::swap(dynamcik[0], dynamcik[dynamcik.size() - 1]);
    dynamcik.pop_back();
    SiftDown(0);
    return min;
  }
  void HeapSort() {
    int size = dynamcik.size();
    for (int i = 0; i < size; i++) {
      std::cout << PopMin() << "\n";
    }
  }
  void Print() {
    for (size_t i = 0; i < dynamcik.size(); i++) {
      std::cout << dynamcik[i] << " ";
    }
    std::cout << '\n';
  }

  bool isEmpty() {
    return dynamcik.empty();
  }
};

int main() {
  int size = 10;
  Heap heap;
  std::cin >> size;
  int* arr = new int[size]; //{66, -92, -90, 24, -62, 53, -11, 11, -25, -28};
  for (size_t i = 0; i < size; i++) {
    std::cin >> arr[i];
  }
  heap.MakeHeap(arr, size);
  heap.HeapSort();
  delete[] arr;
}
