#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H
template <class T>
class UniquePtr {
  T* ptr_;

 public:
  UniquePtr(const UniquePtr&) = delete;
  UniquePtr& operator=(const UniquePtr&) = delete;
  explicit UniquePtr(T* pointer = nullptr) : ptr_(pointer) {
  }
  UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
    other.ptr_ = nullptr;
  }

  UniquePtr& operator=(UniquePtr&& other) noexcept {
    if (this != &other) {
      delete ptr_;

      ptr_ = other.ptr_;
      other.ptr_ = nullptr;
    }
    return *this;
  }
  [[nodiscard]] T* Get() const {
    return ptr_;
  }
  void Swap(UniquePtr<T>& other) {
    std::swap(ptr_, other.ptr_);
  }
  void Reset(T* ptr = nullptr) {
    if (ptr_ != ptr) {
      delete ptr_;
      ptr_ = ptr;
    }
  }
  T* Release() {
    T* temp = ptr_;
    ptr_ = nullptr;
    return temp;
  }
  T& operator*() const {
    return *ptr_;
  }
  T* operator->() const {
    return ptr_;
  }
  explicit operator bool() const {
    return ptr_ != nullptr;
  }
  ~UniquePtr() {
    delete ptr_;
  }
};
#endif