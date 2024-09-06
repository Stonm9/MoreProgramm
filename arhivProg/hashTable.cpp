// Hash таблица
#include <iostream>
#include <cstring>
#include <cmath>
#include <utility>
template <class T>
class List {
  class Node {
   public:
    T value_ = 0;
    Node* next_ = nullptr;
    Node(T value) : value_(value), next_(nullptr) {
    }
  };
  Node* root_ = nullptr;
  
 public:
  bool Find(T element) {
    
    Node* runner = root_;
    
    while (runner != nullptr) {
      if (runner->value_ == element) {
        return true;
      } else {
        runner = runner->next_;
      }
    }
    return false;
  }
  void push_back(T element) {
    if (root_ == nullptr) {
      root_ = new Node(element);
      return;
    }
    Node* runner = root_;
    while (runner->next_ != nullptr) {
      runner = runner->next_;
    }
    runner->next_ = new Node(element);
  }
  bool pop(std::string element) {
    if (root_ == nullptr) {
      return false;
    }
    // Если элемент находится в корне списка
    if (root_->value_ == element) {
      Node* temp = root_;
      root_ = root_->next_;
      delete temp;
      return true;
    }
    Node* runner = root_;
    Node* previous = nullptr;
    // Поиск элемента в списке
    while (runner != nullptr && runner->value_ != element) {
      previous = runner;
      runner = runner->next_;
    }
    if (runner == nullptr) {
      return false;
    }
    previous->next_ = runner->next_;
    delete runner;
    return true;
  }
  void pop_back() {
    if (root_ == nullptr) {
      std::cout << "error, nullptr\n";
      return;
    }
    if (root_->next_ == nullptr) {
      delete root_;
      root_ = nullptr;
      return;
    }
    Node* runner = root_;
    while (runner->next_->next_ != nullptr) {
      runner = runner->next_;
    }
    delete runner->next_;
    runner->next_ = nullptr;
  }

  void top() {
    if (root_ == nullptr) {
      std::cout << "error, nullptr\n";
      return;
    }
    Node* runner = root_;
    while (runner->next_ != nullptr) {
      runner = runner->next_;
    }
    std::cout << runner->value_ << "\n";
  }
  void is_empty() {
    if (root_ == nullptr) {
      std::cout << "Пуст\n";
    } else {
      std::cout << "Не пуст\n";
    }
  }
  ~List() {
    while (root_ != nullptr) {
      Node* temp = root_;
      root_ = root_->next_;
      delete temp;
    }
  }
};

class HashTable {
  List<std::string> list[10000];

 public:
  bool Find(std::string data) {
    int hash = HashFunction(data);
    return list[hash].Find(data);
    ;
  }
  void Push(std::string data) {
    int hash = HashFunction(data);
    if (list[hash].Find(data) == true) {
      return;
    } else {
      list[hash].push_back(data);
    }
  }
  bool Pop(std::string data) {
    int hash = HashFunction(data);
    return list[hash].pop(data);
  }
  int HashFunction(std::string data) {
    int h = 0;
    for (int i = 0; i < data.size(); i++) {
      int simple = std::pow(5, i);
      simple = simple % data.size();
      h += int(data[i]) * simple;
    }
    return h;
  }
};
int main() {
  HashTable hash;
  std::string command;
  int request = 0;
  std::string data;
  std::cin >> request;
  for (size_t i = 0; i < request; i++) {
    std::cin >> command;
    if (command == "search" && std::cin >> data) {
      if (hash.Find(data)) {
        std::cout << "TRUE\n";
      } else {
        std::cout << "FALSE\n";
      }
    } else if (command == "pop" && std::cin >> data) {
      if (hash.Pop(data)) {
        std::cout << "TRUE\n";
      } else {
        std::cout << "FALSE\n";
      }
    } else if (command == "push" && std::cin >> data) {
      hash.Push(data);
    }
  }

  return 0;
}