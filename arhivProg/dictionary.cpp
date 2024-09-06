// Словарь
#include <iostream>
#include <unordered_map>
#include <string>

int main() {
  // Быстрый ввод
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::unordered_map<std::string, std::string> words;
  size_t count_word = 0;
  std::cin >> count_word;
  for (size_t i = 0; i < count_word; i++) {
    std::string first;
    std::string second;
    std::cin >> first >> second;
    words[first] = second;
    words[second] = first;
    // words.emplace(first, second);
    // words.emplace(std::move(second), std::move(first));
  }
  size_t input = 0;
  std::cin >> input;
  for (size_t i = 0; i < input; i++) {
    std::string word;
    std::cin >> word;
    std::cout << words[word] << '\n';
  }
  return 0;
}
