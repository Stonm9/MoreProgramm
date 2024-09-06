#include <iostream>
#include <cstring>
class DynamicArray {
  size_t size_;      // Размер
  size_t capacity_;  // Емкость
  int* arr_;         // Массив
 public:

  DynamicArray() : size_(0), capacity_(0), arr_(nullptr) {
  }
  DynamicArray(int size) : size_(size), capacity_(size), arr_(new int[size]) {
  }
  ~DynamicArray() {
    delete[] arr_;
  }
  size_t Size() {
    return size_;
  }
  size_t Capacity() {
    return capacity_;
  }
  int& operator[](size_t index) {
    return arr_[index];
  }
  void Push_back(const int& element) {
    if (capacity_ == size_) {
      arr_ = Resize();
      arr_[size_++] = element;
    } else {
      arr_[size_++] = element;
    }
  }
  int Pop_back() {
    if (size_ == 0) {
      std::cout << "error, size 0" << '\n';
      return -1;
    } else {
      --size_;
      return arr_[size_];
    }
  }

 private:
  int* Resize() {
    if (capacity_ == 0) {
      capacity_ = 1;
    }
    capacity_ = capacity_ * 2;
    int* buffer = new int[capacity_];
    for (size_t i = 0; i < size_; i++) {
      buffer[i] = arr_[i];
    }
    delete[] arr_;
    arr_ = buffer;
    return arr_;
  }
};
int main() {
  DynamicArray arr(0);
  std::string input;
  int N = 0; 
  std::cin >> N;
  for (size_t i = 0; i < N; i++) {
    std::cin >> input;
    if (input == "push_back") {
      int number = 0;
      std::cin >> number;
      arr.Push_back(number);
    } else if (input == "pop_back") {
      std::cout << arr.Pop_back() << '\n';
    } else if (input == "size") {
      std::cout << arr.Size() << '\n';
    } else if (input == "index"){
      int index = 0;
      std::cin >> index;
      std::cout << arr[index] << '\n';
    }
  }
return 0;
}