// Поиск ребер в неореентированной матице
#include <iostream>
#include <vector>
int Edges(const std::vector<std::vector<int>>& matrix) {
  int edges = 0;
  int n = matrix.size();
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      if (matrix[i][j] != 0) {
        edges++;
      }
    }
  }
  return edges;
}
int main() {
  int vertex = 0;
  std::cin >> vertex;
  std::vector<std::vector<int>> matrix(vertex, std::vector<int>(vertex));
  for (int i = 0; i < vertex; ++i) {
    for (int j = 0; j < vertex; ++j) {
      std::cin >> matrix[i][j];
    }
  }
	std::cout << Edges(matrix) << '\n';
}