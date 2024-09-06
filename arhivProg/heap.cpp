// Реализация структуры данных "max-heap"
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
  int size = 0;
  size_t request = 0;
  Heap heap;
  std::string input;
  int value = 0;
  
  std::cin >> size;
  int* arr = new int[size]; //{-6, -8, -9, -1, 1, -1, 5, 8, 1, -1};
  for (size_t i = 0; i < size; i++)
  {
    std::cin >> arr[i];
  }
  heap.MakeHeap(arr, size);
  std::cin >> request;
  for (size_t i = 0; i < request; i++) {
    std::cin >> input;
    if (input == "Push" && std::cin >> value) {
      heap.Push(value);
    } else if (input == "PopMax") {
      heap.PopMax();
    } else if (input == "IsEmpty") {
      if (heap.isEmpty()) {
        std::cout << "TRUE" << '\n';
      } else {
        std::cout << "FALSE" << '\n';
      }
    } else if (input == "GetMax") {
      if (heap.isEmpty()) {
        std::cout << "None" << '\n';
      } else {
        std::cout << heap.GetMax() << '\n';
      }
    }
  }

  delete[] arr;
}
