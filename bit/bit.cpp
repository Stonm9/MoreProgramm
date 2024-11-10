// Двоичное представление числа с помощью bitset
#include <iostream>
#include <string>
#include <bitset>

int main(){
  unsigned int number = 0b10101;
  std::string tmp;
  std::bitset<32> bit = number;
  tmp = bit.to_string();
  std::cout << number << '\n';
  std::cout << bit << '\n';
  std::cout << tmp << '\n';
  return 0;
}