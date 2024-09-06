// Польская последовательность
#include <iostream>
#include <utility>

struct Node {
  int value;
  Node* next;
};
Node* top = nullptr;
Node* bufer = nullptr;
int numberA = 0;
int numberB = 0;

void LoadSteak() {
  numberA = top->value;
  delete std::exchange(top, top->next);
  numberB = top->value;
  delete std::exchange(top, top->next);
}
void Push(int input) {
  top = new Node{input, top};
}
void Print() {
  bufer = top;
  while (top != nullptr) {
    std::cout << top->value << '\n';
    top = top->next;
  }
  top = bufer;
}
int main() {

  int count = 0;
  char input[100];
  std::cin >> count;

  while (count > 0) {
    std::cin >> input;
    if (isdigit(input[0])) {  // проверка если число
      Push(std::atoi(input));
    } else {
      switch (input[0]) {
        case '+':
          LoadSteak();
          Push(numberA + numberB);
          break;
        case '-':
          LoadSteak();
          Push(numberB - numberA);
          break;
        case '*':
          LoadSteak();
          Push(numberA * numberB);
          break;
        case ':':
          LoadSteak();
          Push(numberB / numberA);
          break;
        default:
          break;
      }
    }
    --count;
  }
  Print();
  // Удаляем стек

  while (top != nullptr) {
    bufer = top->next;
    delete top;
    top = bufer;
  }
  return 0;
}