// Пример создания компаратора 

#include <iostream>
#include <vector>
#include <algorithm>

struct Team {
  size_t n_sloved;
  size_t penalty;
  size_t id;
};

bool CompareACM(const Team& lhs, const Team& rhs) {
  if (lhs.n_sloved > rhs.n_sloved) {
    return true;
  }
  if (lhs.n_sloved == rhs.n_sloved) {
    if (lhs.penalty < rhs.penalty) {
      return true;
    }
    if (lhs.penalty == rhs.penalty) {
      return lhs.id < rhs.id;
    }
    return false;
  }
  return false;
}
int main() {
  size_t n = 5;
  std::cin >> n;
  std::vector<Team> teams(n);
  for (size_t i = 0; i < n; ++i) {
    std::cin >> teams[i].n_sloved >> teams[i].penalty;
    teams[i].id = i + 1;
  }
  std::sort(teams.begin(), teams.end(), CompareACM);
  for (const auto& team : teams) {
    std::cout << team.id << '\n';
  }

  return 0;
}
