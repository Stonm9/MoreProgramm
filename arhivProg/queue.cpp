// Реализация очереди
#include <iostream>
#include <cstring>
#include <utility>
struct Node {
  std::string name;
  Node* next;
};
class Stack {
  Node* first = nullptr;
  Node* last = nullptr;

 public:
  Stack() {
  }
  ~Stack() {
    while (first != nullptr) {
      Node* bufer = first->next;
      delete first;
      first = bufer;
    }
    while (last != nullptr) {
      Node* bufer = last->next;
      delete last;
      last = bufer;
    }
  }
  void Push(std::string name) {
    first = new Node{name, first};
  }
  std::string Top() {
    return last->name;
  }
  bool isEmpty() {
    return (first == nullptr) && (last == nullptr);
  }
  void Pop() {
    if (last == nullptr) {
      if (first != nullptr) {
        while (first != nullptr) {
          last = new Node{first->name, last};
          delete std::exchange(first, first->next);
        }
      } else {
        std::cout << "Все вылечены!\n";
      }
      std::cout << Top() << '\n';
      delete std::exchange(last, last->next);
    } else {
      std::cout << Top() << '\n';
      delete std::exchange(last, last->next);
    }
  }
};
int main() {
  std::string input;
  Stack queue;

  while (true) {
    std::getline(std::cin, input);
    if (input == "Смена закончилась!") {
      return 0;
    }
    if (input == "новичок:") {
      std::cin >> input;
      queue.Push(input);
    } else if (input == "следующий") {
      if (queue.isEmpty()) {
        std::cout << "Все вылечены!";
        return 0;
      } else {
        queue.Pop();
      }
    }
  };
  return 0;
}
