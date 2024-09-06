// Задача про рюкзак
#include <iostream>

int Max_Weight(int** arr, int* weight_gold, size_t amount, int weight_backpack) {
  // Строим таблицу
  for (size_t i = 1; i < amount; i++) {
    for (size_t j = 1; j <= weight_backpack; j++) {
      if (weight_gold[i] > j) {
        arr[i][j] = arr[i - 1][j];
      } else {
        arr[i][j] = std::max(arr[i - 1][j], weight_gold[i] + arr[i - 1][j - weight_gold[i]]);
      }
    }
  }
  return arr[amount - 1][weight_backpack];
}

int main() {
  int weight_backpack = 0;  // Вместимость рюкзака
  size_t amount = 0;        // Количество слитков
  // Читаем вводные данные
  std::cin >> weight_backpack;
  std::cin >> amount;
  int* weight_gold = new int[++amount]{0};
  for (size_t i = 1; i < amount; i++) {
    std::cin >> weight_gold[i];
  }
  int** arr = new int*[amount];
  for (size_t i = 0; i < amount; i++) {
    arr[i] = new int[weight_backpack + 1]{0};
  }
  std::cout << Max_Weight(arr, weight_gold, amount, weight_backpack) << '\n';
  // Печать таблицы
  /*for (size_t i = 0; i < amount; i++) {
    for (size_t j = 0; j <= weight_backpack; j++) {
      std::cout << arr[i][j] << " ";
    }
    std::cout << '\n';
  }*/

  // Удаляем массивы
  for (size_t i = 0; i < amount; i++) {
    delete[] arr[i];
  }
  delete[] arr;
  delete[] weight_gold;
}