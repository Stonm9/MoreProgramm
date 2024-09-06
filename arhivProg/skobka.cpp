// Скобочная последовательность
#include <iostream>
#include <string>
#include <utility>
struct Node {
  char value;
  Node* next;
};
class Stack {
  Node* top = nullptr;
  char simbolOpen_ = ' ';
  char simbolClose_ = ' ';

 public:
  Stack(char simbolOpen, char simbolClose) : simbolOpen_(simbolOpen), simbolClose_(simbolClose) {
  }
  ~Stack() {
    while (top != nullptr) {
      Node* bufer = top->next;
      delete top;
      top = bufer;
    }
  }
  bool backet(std::string command) {
    for (size_t i = 0; i < command.size(); i++) {
      if (command[i] == simbolOpen_) {
        top = new Node{command[i], top};
      } else if (command[i] == simbolClose_) {
        if (top == nullptr) {
          return false;
        } else {
          delete std::exchange(top, top->next);
        }
      }
    }
    return (top == nullptr);
  }
};

int main() {
  std::string command;
  std::cin >> command;
  Stack round[3]{{'(', ')'}, {'{', '}'}, {'[', ']'}};
  for (size_t i = 0; i < 3; i++) {
    if (round[i].backet(command) == false) {
      std::cout << "INCORRECT\n";
      return 0;
    }
  }
  std::cout << "CORRECT\n";
  return 0;
}