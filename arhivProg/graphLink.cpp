// Алгоритм поиска в ширину (BFS) для нахождения кратчайших расстояний от стартовой вершины до всех остальных вершин в неориентированном графе.
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
    edges[from][to] = 1;
    edges[to][from] = 1;  // Для неориентированного графа
  }

  std::vector<int> BFS(int start) {
    std::vector<int> distance(N, -1);
    std::queue<int> queue;

    distance[start] = 0;
    queue.push(start);

    while (!queue.empty()) {
      int current = queue.front();
      queue.pop();

      for (int neighbor = 0; neighbor < N; ++neighbor) {
        if (edges[current][neighbor] == 1 && distance[neighbor] == -1) {
          queue.push(neighbor);
          distance[neighbor] = distance[current] + 1;
        }
      }
    }

    return distance;
  }
};

int main() {
  int vertex, M;
  std::cin >> vertex >> M;
  Graph list(vertex);

  for (int i = 0; i < vertex; ++i) {
    for (int j = 0; j < vertex; ++j) {
      int value;
      std::cin >> value;
      if (value == 1) {
        list.addEdge(i, j);
      }
    }
  }

  std::vector<int> distances = list.BFS(0);
  for (int distance : distances) {
    std::cout << distance << " ";
  }
  std::cout << std::endl;

  return 0;
}