// Слияние двух отсортированных массивов в один отсортированный массив.
#include <iostream>
bool compare (int& mas1, int& mas2){
  return mas1 < mas2;
}
int* Sort(int* mas1, size_t size1, int* mas2, size_t size2, int* masMerge) {
  size_t Left = 0;
  size_t Right = 0;
  // Проверяем и сливаем
  while (Left < size1 && Right < size2) {
    if (compare(mas1[Left],mas2[Right])) {
      masMerge[Left + Right] = mas1[Left];
      ++Left;
    } else {
      masMerge[Left + Right] = mas2[Right];
      ++Right;
    }
  }
  // Сливаем остатки
  while (Left < size1) {
    masMerge[Left + Right] = mas1[Left];
    ++Left;
  }
  while (Right < size2) {
    masMerge[Left + Right] = mas2[Right];
    ++Right;
  }
  return masMerge;
}
int main() {
  size_t size1 = 0;
  size_t size2 = 0;
  // Получаем размер и заполняем первый массив
  std::cin >> size1;
  int* mas1 = new int[size1];
  for (size_t i = 0; i < size1; i++) {
    std::cin >> mas1[i];
  }
  // Получаем размер и заполняем второй массив
  std::cin >> size2;
  int* mas2 = new int[size2];
  for (size_t i = 0; i < size2; i++) {
    std::cin >> mas2[i];
  }
  // Слияние
  int* masMerge = new int[size1 + size2];
  Sort(mas1, size1, mas2, size2, masMerge);
  // Вывод на экран
  for (size_t i = 0; i < size1 + size2; i++) {
    std::cout << masMerge[i] << '\n';
  }
  delete[] masMerge;
  delete[] mas2;
  delete[] mas1;
  return 0;
}