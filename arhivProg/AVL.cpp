//Релизация класса AVL-дерева 

#include <iostream>
class AVL {
  struct Node {
    int value = 0;
    int height = 0;  // Высота поддерева
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    Node(int val, int heig) : value(val), height(heig), left(nullptr), right(nullptr), parent(nullptr) {
    }
  };

 public:
  AVL() = default;

  ~AVL() {
    destroyTree(root);
  }

 private:
  void destroyTree(Node* node) {
    if (node != nullptr) {
      destroyTree(node->left);
      destroyTree(node->right);
      delete node;
    }
  }

  Node* root = nullptr;

  int Height(Node* node) {  // Возвращаем высоту
    if (node == nullptr) {
      return 0;
    }
    return node->height;
  }
  void fixHeight(Node* node) {  // Исправляем высоту
    int L = Height(node->left);
    int R = Height(node->right);
    node->height = std::max(L, R) + 1;
  }
  int Bfactor(Node* node) {
    return Height(node->left) - Height(node->right);
  }
  Node* LeftRotation(Node* node) {
    //      a                  b
    //    /   \               /  \ 
    //   l     b             a   r
    //        / \           / \
    //       c   r         l   c
    Node* b = node->right;
    Node* l = node->left;
    Node* r = node->right->right;
    Node* c = node->right->left;
    b->left = node;  // На левую ветку _b вешаем _a
    //       b
    //      / \
    //     a   r
    //    / \               
    //   l   b
    //      / \          
    //     c   r
    b->parent = node->parent;  // родитель _b равен родителю _a
    node->parent = b;          // родитель _a становиться _b
    node->right = c;           // на правую ветку _a вешам _c
    //      a
    //     / \           
    //    l   c
    if (c != nullptr) {
      c->parent = node;  // родителем _c становитья _a
    }
    fixHeight(node);
    fixHeight(b);
    return b;
  }
  Node* RightRotation(Node* node) {
    //     a             b
    //    / \           / \ 
    //   b   r         l   a
    //  / \               / \
    // l   c             c   r
    Node* b = node->left;
    Node* l = node->left->left;
    Node* r = node->right;
    Node* c = node->left->right;
    b->right = node;  // На правую ветку _b вешаем _a
    //       b
    //      / \
    //     l   a
    //        / \               
    //       b   r
    //      / \          
    //     l   c
    b->parent = node->parent;  // родитель _b равен родителю _a
    node->parent = b;          // родитель _a становиться _b
    node->left = r;            // на левую ветку _a вешам _r
    if (r != nullptr) {
      r->parent = node;  // родителем _r становитья _a
    }
    fixHeight(node);
    fixHeight(b);
    return b;
  }
  Node* BigLeftRotation(Node* node) {
    //      a                   c
    //    /   \               /   \ 
    //   l     b             a     b
    //        / \           / \   / \
    //       c   r         l   m n   r
    //      / \        
    //     m   n
    Node* a = node;
    Node* l = node->left;
    Node* b = node->right;
    Node* c = b->left;
    Node* m = c->left;
    Node* n = c->right;
    Node* r = b->right;
    c->right = b;              // На правую ветку _с вешаем _b
    b->parent = c;             // Родитель _b равен _с
    c->parent = node->parent;  // Родитель _с равен родителю _а

    a->right = m;  // На левую ветку _а вешаем _m
    if (m != nullptr) {
      m->parent = a;  // Родитель _m равен _a;
    }

    c->left = a;    // На левую ветку _с вешаем _a
    a->parent = c;  // Родиетль _a равен _с

    b->left = n;  // На левую ветку _b вешаем _n
    if (n != nullptr) {
      n->parent = b;  // Родитель _n равен _b
    }
    fixHeight(a);
    fixHeight(b);
    fixHeight(c);
    return c;
  }

  Node* BigRightRotation(Node* node) {
    //     a                c
    //    / \             /   \
    //   b   r           b     a
    //  / \             / \   / \
    // l   c           l   m n   r
    //    / \
    //   m   n
    Node* a = node;
    Node* b = node->left;
    Node* r = node->right;
    Node* l = b->left;
    Node* c = b->right;
    Node* m = c->left;
    Node* n = c->right;
    c->right = a;
    a->parent = c;
    c->parent = a;

    a->left = n;
    if (n != nullptr) {
      n->parent = a;
    }

    c->left = b;
    b->parent = c;

    b->right = m;
    if (m != nullptr) {
      m->parent = b;
    }
    fixHeight(b);
    fixHeight(a);
    fixHeight(c);
    return c;
  }
  Node* Balance(Node* node) {
    fixHeight(node);
    if (Bfactor(node) == 2) {
      if (Bfactor(node->left) >= 0) {
        return RightRotation(node);
      } else {
        return BigRightRotation(node);
      }
    } else if (Bfactor(node) == -2) {
      if (Bfactor(node->right) <= 0) {
        return LeftRotation(node);
      } else {
        return BigLeftRotation(node);
      }
    }
    return node;
  }
  Node* Insert2(int key, Node* tree) {
    if (tree == nullptr) {
      tree = new Node(key, 1);
      return tree;
    }
    if (key < tree->value) {
      tree->left = Insert2(key, tree->left);
      tree->left->parent = tree;
    } else {
      tree->right = Insert2(key, tree->right);
      tree->right->parent = tree;
    }
    return Balance(tree);
  }

 public:
  void RemoveMax(Node* runner = nullptr) {
    if (runner == nullptr) {
      runner = root;
    }
    if (runner == nullptr) {
      return;
    }
    Node* parent = nullptr;
    while (runner->right != nullptr) {
      parent = runner;
      runner = runner->right;
    }
    if (parent == nullptr) {  // Если макс в корне
      root = root->left;
      delete runner;
      return;
    } else {
      parent->right = runner->left;
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
    while (runner->left != nullptr) {
      parent = runner;
      runner = runner->left;
    }
    if (parent == nullptr) {  // Если мин в корне
      root = root->right;
      delete runner;
      return;
    } else {
      parent->left = runner->right;
      delete runner;
    }
  }
  void insert(int key) {
    root = Insert2(key, root);
  }
  bool find(int element) {
    Node* runner = root;
    while (runner != nullptr) {
      if (runner->value < element) {
        runner = runner->right;
      } else if (runner->value > element) {
        runner = runner->left;
      } else {
        return true;
      }
    }
    return false;
  }
  int get_min(Node* runner = nullptr) {
    if (runner == nullptr) {
      runner = root;
    }
    if (runner == nullptr) {
      return false;
    }
    while (runner->left != nullptr) {
      runner = runner->left;
    }
    return runner->value;
  }

  int get_max(Node* runner = nullptr) {
    if (runner == nullptr) {
      runner = root;
    }
    if (runner == nullptr) {
      return false;
    }
    while (runner->right != nullptr) {
      runner = runner->right;
    }
    return runner->value;
  }
  void remove(int key) {
    Node* runner = root;
    Node* parent = nullptr;

    while (runner != nullptr) {
      if (runner->value > key) {
        parent = runner;
        runner = runner->left;
      } else if (runner->value < key) {
        parent = runner;
        runner = runner->right;
      } else {                                                      // Элемент найден
        if (runner->left == nullptr && runner->right == nullptr) {  // Нет детей
          if (parent == nullptr) {                                  // Удаляем корень
            root = nullptr;
          } else if (parent->left == runner) {
            parent->left = nullptr;
          } else {
            parent->right = nullptr;
          }
          delete runner;
        } else if (runner->left != nullptr && runner->right != nullptr) {  // Два ребенка
          Node* minNode = runner->right;
          parent = runner;
          while (minNode->left != nullptr) {
            parent = minNode;
            minNode = minNode->left;
          }
          runner->value = minNode->value;
          // Удаляем минимальный узел из правого поддерева
          if (parent->left == minNode) {
            parent->left = minNode->right;
          } else {
            parent->right = minNode->right;
          }
          delete minNode;
        } else {  // Один ребенок
          Node* child = (runner->left != nullptr) ? runner->left : runner->right;
          if (parent == nullptr) {  // Удаляем корень
            root = child;
          } else if (parent->left == runner) {
            parent->left = child;
          } else {
            parent->right = child;
          }
          delete runner;
        }
        Node* current = parent;
        while (current != nullptr) {
          current = Balance(current);
          current = current->parent;
        }
        return;
      }
    }
  }
};
int main() {
  AVL tree;
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
      tree.insert(data);
    } else if (command == "pop" && std::cin >> data) {
      tree.remove(data);
    }
  }
  return 0;
}