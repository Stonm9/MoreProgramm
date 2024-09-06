// Декартово дерево
#include <iostream>
#include <ctime>
#include <cstdlib>

class DecartTree {
  class Node {
   public:
    int key;
    int priority;
    Node* left_;
    Node* right_;
    Node(int k, int p) : key(k), priority(p), left_(nullptr), right_(nullptr) {}
  };
  struct Result {
    Node* LeftT = nullptr;
    Node* RightT = nullptr;
  };

 public:
  Node* root = nullptr;
  ~DecartTree() {
    destroyTree(root);
  }

  void destroyTree(Node* node) {
    if (node != nullptr) {
      destroyTree(node->left_);
      destroyTree(node->right_);
      delete node;
    }
  }

  Result Split(Node* tree, int key) {
    if (tree == nullptr) {
      return {nullptr, nullptr};
    }
    if (key >= tree->key) {
      Result tooTree = Split(tree->right_, key);
      tree->right_ = tooTree.LeftT;
      return {tree, tooTree.RightT};
    } else {
      Result tooTree = Split(tree->left_, key);
      tree->left_ = tooTree.RightT;
      return {tooTree.LeftT, tree};
    }
  }

  Node* Merge(Node* left, Node* right) {
    if (left == nullptr) {
      return right;
    }
    if (right == nullptr) {
      return left;
    }
    if (left->priority >= right->priority) {
      left->right_ = Merge(left->right_, right);
      return left;
    } else {
      right->left_ = Merge(left, right->left_);
      return right;
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
    return runner->key;
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
    return runner->key;
  }

  bool find(int element) {
    Node* runner = root;
    while (runner != nullptr) {
      if (runner->key < element) {
        runner = runner->right_;
      } else if (runner->key > element) {
        runner = runner->left_;
      } else {
        return true;
      }
    }
    return false;
  }

  void push(int key) {
    if (find(key)) {
      return;  
    }
    Node* NewNode = new Node{key, rand()};
    if (root == nullptr) {
      root = NewNode;
    } else {
      Result tooTree = Split(root, key);
      tooTree.RightT = Merge(NewNode, tooTree.RightT);
      root = Merge(tooTree.LeftT, tooTree.RightT);
    }
  }

  void pop(int key) {
    if (!find(key)) {
      return;  
    }
    Result temp1 = Split(root, key - 1);
    Result temp2 = Split(temp1.RightT, key);
    if (temp2.LeftT != nullptr) {
      delete temp2.LeftT;  
    }
    root = Merge(temp1.LeftT, temp2.RightT);
  }
};
int main() {
  std::srand(std::time(0));
  DecartTree tree;
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