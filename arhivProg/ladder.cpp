// Этот код решает задачу нахождения максимальной суммы подотрезка массива, где элементы подотрезка могут быть выбраны так, чтобы не находиться рядом друг с другом.
#include <iostream>
int Max(int first, int second){
    return first > second ? first : second;
}
int Sum(int* mas, int n) {
  int prev = 0;
  int next = mas[0];
  int buffer = 0;
  for (int i = 1; i < n; ++i) {
    buffer = Max(prev + mas[i], next + mas[i]);
    prev = next;
    next = buffer;
  }
  return next;
}

int main() {
  int n = 0;
  std::cin >> n;
  int* mas = new int[n];
  for (int i = 0; i < n; i++) {
    std::cin >> mas[i];
  }
  std::cout << Sum(mas, n);
  delete[] mas;
  return 0;
}