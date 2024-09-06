// Алгоритм поиска в ширину (BFS) для нахождения кратчайших путей от стартовой вершины до всех остальных вершин в графе.
#include <iostream>
#include <vector>
#include <queue>

class Graph {
 public:
  std::vector<std::vector<int>> edges;
  size_t N = 0;  // количество вершин

  Graph(int vertices) {
    N = vertices;
    edges.resize(N, std::vector<int>(N, 0));
  }

  void addEdge(int from, int to) {
    //  edges[to][from] = 1;
    edges[from][to] = 1;  // Для ориентированного графа
  }

  std::vector<int> BFS(int start) {
    std::vector<int> dist(N, -1);
    std::queue<int> queue;

    dist[start] = 0;
    queue.push(start);

    while (!queue.empty()) {
      int current = queue.front();
      queue.pop();

      for (int i = 0; i < N; i++) {
        if (edges[current][i] == 1 && dist[i] == -1) {
          queue.push(i);
          dist[i] = dist[current] + 1;
        }
      }
    }

    return dist;
  }
};

int main() {
  int vertex = 0;
  int start = 0;
  std::cin >> vertex >> start;
  Graph list(vertex);
  // Чтение матрицы
  for (int i = 0; i < vertex; ++i) {
    for (int j = 0; j < vertex; ++j) {
      int value;
      std::cin >> value;
      if (value == 1) {
        list.addEdge(i, j);
      }
    }
  }

  std::vector<int> distances = list.BFS(start - 1);
  for (size_t i = 0; i < distances.size(); ++i) {
    std::cout << distances[i] << " ";
  }
  std::cout << '\n';

  return 0;
}
