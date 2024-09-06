// Этот код решает задачу по выбору максимального количества предметов с ограничением по общему весу. 
#include <iostream>
int main() {
  int weghit = 0;
  size_t size = 0;
  int count = 0;
  std::cin >> weghit;
  std::cin >> size;
  int* arr = new int[size]{};
  for (size_t i = 0; i < size; i++)
  {
    std::cin >> arr[i];
  }
  
  for (size_t i = 0; i < size; i++) {
    if (weghit >= arr[i]) {
      weghit = weghit - arr[i];
      count++;
    }
  }
  std::cout << count;
  delete[] arr;
}