#include <iostream>
#include <vector>

class Graph {
 private:
  std::vector<std::vector<int>> edges;

 public:
  size_t N = 0;  // количество вершин

  Graph(int vertices) {
    N = vertices;
    edges.resize(vertices);
  }

  bool findEdge(int from, int to) {
    for (size_t i = 0; i < edges[from].size(); i++) {
      if (edges[from][i] == to) {
        return true;
      }
    }
    return false;
  }
  void removeEdge(int from, int to) {
    for (size_t i = 0; i < edges[from].size(); i++) {
      if (edges[from][i] == to) {
        edges[from].erase(edges[from].begin() + i);
        break;
      }
    }
  }
  void addEdge(int from, int to) {
    if (!findEdge(from, to)) {
      edges[from].push_back(to);
    }
  }
  int operator()(int index, int index2) {
    if (index >= 0 && index < N && index2 >= 0 && index2 < N && findEdge(index, index2)) {
      return 1;
    }
    return 0;
  }
  int getEdge(int index, int index2) {
    if (index >= 0 && index < N && index2 >= 0 && index2 < edges[index].size()) {
      return edges[index][index2];
    }
    return -1;
  }
};

int main() {
  int vertex = 0;
  std::cin >> vertex;
  Graph listGraph(vertex);

  for (int i = 0; i < vertex; i++) {
    int numEdges = 0;
    std::cin >> numEdges;
    for (int j = 0; j < numEdges; j++) {
      int to;
      std::cin >> to;
      listGraph.addEdge(i, to - 1);
    }
  }

  // Вывод матрицы
  for (int i = 0; i < vertex; i++) {
    for (int j = 0; j < vertex; j++) {
      std::cout << listGraph(i, j) << " ";
    }
    std::cout << '\n';
  }

  return 0;
}
