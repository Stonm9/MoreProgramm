#ifndef SHARED_H
#define SHARED_H
#include <stdexcept>
#include <iostream>
class BadWeakPtr : public std::runtime_error {
 public:
  BadWeakPtr() : std::runtime_error("BadWeakPtr") {
  }
};
template <class T>
struct Block {
  T* ptr_;
  size_t cnt_;
};

template <class T>
class SharedPtr {
  Block<T>* block_;

 public:
  SharedPtr() : block_(nullptr){};
  SharedPtr(std::nullptr_t) noexcept : SharedPtr() {  // NOLINT
  }
  explicit SharedPtr(T* ptr) : block_(new Block<T>{ptr, 1}) {  // explicit конструктор от одного аргумента
  }

  // Конструктор копирования
  SharedPtr(const SharedPtr& other) : block_(other.block_) {
    if (block_ != nullptr) {
      ++((*block_).cnt_);
    }
  }
  // Конструктор перемещения
  SharedPtr(SharedPtr&& other) noexcept : block_(std::move(other.block_)) {
    other.block_ = nullptr;
  }
  // Перемещающее присваивание
  SharedPtr& operator=(SharedPtr&& other) noexcept {
    Realese();
    block_ = other.block_;
    other.block_ = nullptr;
    return *this;
  }
  // Присваивание
  SharedPtr& operator=(const SharedPtr& other) {
    SharedPtr tmp = other;
    Swap(tmp);
    return *this;
  }
  T* Realese() {  // Больше не владею текущим блоком
    if (!block_) {
      return nullptr;
    }
    auto ptr = block_->ptr_;
    --(block_->cnt_);
    if (block_->cnt_ == 0) {
      delete ptr;
      delete block_;
    }
    block_ = nullptr;
    return ptr;
  }
  void Swap(SharedPtr<T>& other) noexcept {
    std::swap(block_, other.block_);
  }
  void Reset() {
    SharedPtr().Swap(*this);
  }
  void Reset(T* ptr) {
    SharedPtr(ptr).Swap(*this);
  }
  [[nodiscard]] T* Get() const {
    if (block_ == nullptr) {
      return nullptr;
    }
    return block_->ptr_;
  }
  size_t UseCount() const {
    if (block_ == nullptr) {
      return 0;
    }
    return block_->cnt_;
  }
  T& operator*() const {
    return *block_->ptr_;
  }
  T* operator->() const {
    return block_->ptr_;
  }
  explicit operator bool() const {
    return block_ && block_->ptr_ != nullptr;
  }
  ~SharedPtr() {
    Realese();
  }
};

#endif