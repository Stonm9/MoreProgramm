//Реализация стека на основе связного списка
#include <iostream>
#include <utility>
template <class T>
struct Node {
  Node* next;
  T value;
};
template <class T>
class Stack{
  Node<T>* head_ = nullptr;
  size_t size_;
public:
~Stack(){
  while(head_){
    delete std::exchange(head_,head_->next);
  }
  std::cout << "Destruct" << '\n';
}
 void Push(T value){
  head_ = new Node<T>{head_, value};
  ++size_;
 }
 void Pop(){
  delete std::exchange(head_,head_->next);
 }
 T& Top(){
  return head_->value;
 }
 T Top() const{
  return head_->value;
 }
 void Clear(){
  while (head_ != nullptr){
    Pop();
    size_ = 0;
  }
 }
 bool Empty(){
  return size_ == 0;
 }
 size_t Size() const{
  return size_;
 }
};

void Function(){
  Stack<int> s;
  s.Push(1);
  s.Push(2);
}
int main() {
  Function();
  return 0;
}