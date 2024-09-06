// Реализация алгоритма Дейкстры для нахождения кратчайшего пути между двумя вершинами в ориентированном графе с ненулевыми весами рёбер.
#include <iostream>
#include <vector>
#include <queue>

const int INF = 1000000000;

class Graph {
 public:
  std::vector<std::vector<int>> adj;
  int num_vertices;

  Graph(int n) : num_vertices(n), adj(n, std::vector<int>(n, 0)) {
  }

  void addEdge(int from, int to, int weight) {
    adj[from][to] = weight;
  }

  std::vector<int> dijkstra(int start) {
    std::vector<int> dist(num_vertices, INF);
    std::vector<bool> visited(num_vertices, false);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
      int u = pq.top().second;
      pq.pop();

      if (visited[u])
        continue;
      visited[u] = true;

      for (int v = 0; v < num_vertices; ++v) {
        if (adj[u][v] > 0) {
          int weight = adj[u][v];
          if (dist[v] > dist[u] + weight) {
            dist[v] = dist[u] + weight;
            pq.push({dist[v], v});
          }
        }
      }
    }

    return dist;
  }
};

int main() {
  int n;
  int start;
  int finish;
  std::cin >> n >> start >> finish;

  Graph graph(n);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cin >> graph.adj[i][j];
    }
  }

  std::vector<int> paths = graph.dijkstra(start - 1);
  int paths_short = paths[finish - 1];
  if (paths_short == INF) {
    std::cout << "-1\n";
  } else {
    std::cout << paths_short << "\n";
  }

  return 0;
}
