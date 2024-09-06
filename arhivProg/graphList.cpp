// Список смежности (для работы с графом)
#include <iostream>
#include <vector>
const int kComEdge = 1;
const int kComVertex = 2;
template <class T>
void PringVector(const std::vector<T>& v) {
  for (const auto& x : v) {
    std::cout << x << ' ';
  }
  std::cout << '\n';
}
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  size_t count_vertex = 0;
  std::cin >> count_vertex;

  std::vector<std::vector<int>> graph(count_vertex);
  size_t count_operation = 0;
  std::cin >> count_operation;
  for (size_t i = 0; i < count_operation; i++) {
    int command = 0;
    std::cin >> command;
    if (command == kComEdge) {
      int first_number = 0;
      int second_number = 0;
      std::cin >> second_number >> first_number;
      graph[second_number - 1].push_back(first_number);
      graph[first_number - 1].push_back(second_number);
    } else if (command == kComVertex) {
      int vertex = 0;
      std::cin >> vertex;
      PringVector(graph[vertex - 1]);
    }
  }

  return 0;
}
