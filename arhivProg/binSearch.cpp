// Бинарный поиск
#include <iostream>
int BinSearch(int* arr, int begin, int end, int key) {
  while (begin != end) {
    int mid = begin + (end - begin) / 2;
    if (arr[mid] > key) {
      end = mid;
    } else if (arr[mid] < key) {
      begin = mid + 1;
    } else {
      return mid + 1;
    }
  }
  return -1;
}
int main() {
  size_t size_n = 0;
  size_t size_x = 0;
  std::cin >> size_n;
  int* n = new int[size_n];
  for (size_t i = 0; i < size_n; i++)
  {
          std::cin >> n[i];
  }
	std::cin >> size_x;
  int* x = new int[size_x];
  for (size_t i = 0; i < size_x; i++)
  {
          std::cin >> x[i];
  }
	for (size_t i = 0; i < size_x; i++)
	{
		std::cout << BinSearch(n, 0, size_n, x[i]) << " ";	
	}
	
  delete[] n;
  delete[] x;
  return 0;
}