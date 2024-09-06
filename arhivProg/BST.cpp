// Бинарное Дерево Поиска
#include <iostream>
struct test {
  int test;
  int sdfsd;
};

class BST {
  class Node {
   public:
    int value_;
    Node* left_;
    Node* right_;
    Node(int value) : value_(value), left_(nullptr), right_(nullptr) {
    }
  };

 public:
  Node* root = nullptr;
  ~BST() {
    destroyTree(root);
  }

  void destroyTree(Node* node) {
    if (node != nullptr) {
      destroyTree(node->left_);
      destroyTree(node->right_);
      delete node;
    }
  }

  int get_min(Node* runner = nullptr) {
    if (runner == nullptr) {
      runner = root;
    }
    if (runner == nullptr) {
      return false;
    }
    while (runner->left_ != nullptr) {
      runner = runner->left_;
    }
    return runner->value_;
  }
  int get_max(Node* runner = nullptr) {
    if (runner == nullptr) {
      runner = root;
    }

    if (runner == nullptr) {
      return false;
    }
    while (runner->right_ != nullptr) {
      runner = runner->right_;
    }
    return runner->value_;
  }
  void RemoveMax() {
    Node* runner = root;
    if (runner == nullptr) {
      return;
    }
    Node* parent = nullptr;
    while (runner->right_ != nullptr) {
      parent = runner;
      runner = runner->right_;
    }
    if (parent == nullptr) {  // Если макс в корне
      root = root->left_;
      delete runner;
      return;
    } else {
      parent->right_ = runner->left_;
      delete runner;
    }
  }
  void RemoveMin(Node* runner = nullptr) {
    if (runner == nullptr) {
      runner = root;
    }
    if (runner == nullptr) {
      return;
    }
    Node* parent = nullptr;
    while (runner->left_ != nullptr) {
      parent = runner;
      runner = runner->left_;
    }
    if (parent == nullptr) {  // Если мин в корне
      root = root->right_;
      delete runner;
      return;
    } else {
      parent->left_ = runner->right_;
      delete runner;
    }
  }
  bool find(int element) {
    Node* runner = root;
    if (runner == nullptr) {
      return false;
    }
    while (runner != nullptr) {
      if (runner->value_ < element) {
        runner = runner->right_;
      } else if (runner->value_ > element) {
        runner = runner->left_;
      } else {
        return true;
      }
    }
    delete runner;
    return false;
  }
  void push(int element) {
    Node* runner = root;
    if (runner == nullptr) {
      root = new Node(element);
      return;
    }

    Node* parent = nullptr;
    while (runner != nullptr) {
      parent = runner;
      if (element < runner->value_) {
        runner = runner->left_;
      } else {
        runner = runner->right_;
      }
    }

    if (element < parent->value_) {
      parent->left_ = new Node(element);
    } else {
      parent->right_ = new Node(element);
    }
  }

  void pop(int element) {
    Node* runner = root;
    Node* parent = nullptr;

    while (runner != nullptr) {
      if (runner->value_ > element) {
        parent = runner;
        runner = runner->left_;
      } else if (runner->value_ < element) {
        parent = runner;
        runner = runner->right_;
      } else {                                                        // Элемент найден
        if (runner->left_ == nullptr && runner->right_ == nullptr) {  // Нет детей
          if (parent == nullptr) {                                    // Удаляем корень
            root = nullptr;
          } else if (parent->left_ == runner) {
            parent->left_ = nullptr;
          } else {
            parent->right_ = nullptr;
          }
          delete runner;
        } else if (runner->left_ != nullptr && runner->right_ != nullptr) {  // Два ребенка
          Node* minNode = runner->right_;
          parent = runner;
          while (minNode->left_ != nullptr) {
            parent = minNode;
            minNode = minNode->left_;
          }
          runner->value_ = minNode->value_;
          // Удаляем минимальный узел из правого поддерева
          if (parent->left_ == minNode) {
            parent->left_ = minNode->right_;
          } else {
            parent->right_ = minNode->right_;
          }
          delete minNode;
        } else {  // Один ребенок
          Node* child = (runner->left_ != nullptr) ? runner->left_ : runner->right_;
          if (parent == nullptr) {  // Удаляем корень
            root = child;
          } else if (parent->left_ == runner) {
            parent->left_ = child;
          } else {
            parent->right_ = child;
          }
          delete runner;
        }
        return;
      }
    }
  }
};
int main() {
  BST tree;

  std::string command;
  int request = 0;
  int data;
  std::cin >> request;
  for (size_t i = 0; i < request; i++) {
    std::cin >> command;
    if (command == "find" && std::cin >> data) {
      if (tree.find(data)) {
        std::cout << "TRUE\n";
      } else {
        std::cout << "FALSE\n";
      }
    } else if (command == "get_min") {
      std::cout << tree.get_min() << '\n';
    } else if (command == "get_max") {
      std::cout << tree.get_max() << '\n';
    } else if (command == "push" && std::cin >> data) {
      tree.push(data);
    } else if (command == "pop" && std::cin >> data) {
      tree.pop(data);
    }
  }

  return 0;
}