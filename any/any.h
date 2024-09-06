#ifndef ANY_H
#define ANY_H
#include <utility>
#include <stdexcept>
#include <utility>
class BadAnyCast : public std::bad_cast {
 public:
  [[nodiscard]] const char* what() const noexcept override {
    return "BadAnyCast error";
  }
};

struct IHolder {
  virtual std::unique_ptr<IHolder> Clone() const = 0;
  virtual ~IHolder() = default;
};

template <class T>
struct AnyHolder : public IHolder {
  T value;
  explicit AnyHolder(T val) : value(val){};  // Конструктор от одного аргумента
  std::unique_ptr<IHolder> Clone() const override {
    return std::make_unique<AnyHolder<T>>(value);
  }
};

class Any {
  std::unique_ptr<IHolder> holder_;

 public:
  Any() : holder_(nullptr){};
  template <class T>
  Any(T value) : holder_(std::make_unique<AnyHolder<T>>(std::move(value))) {  // NOLINT
  }
  Any(const Any& other) : holder_(other.holder_->Clone()){};

  void Reset() {
    holder_ = nullptr;
  }
  void Swap(Any& other) {
    std::swap(holder_, other.holder_);
  }
  bool HasValue() const {
    return holder_ != nullptr;
  }

  Any& operator=(const Any& other) {
    if (this != &other) {
      if (other.holder_) {
        holder_ = other.holder_->Clone();
      } else {
        holder_ = nullptr;
      }
    }
    return *this;
  }

  Any& operator=(Any&& other) noexcept {
    if (this != &other) {
      holder_ = std::move(other.holder_);
      other.holder_ = nullptr;
    }
    return *this;
  }
  template <class T>
  friend T AnyCast(const Any& any);
};

template <class T>
T AnyCast(const Any& any) {
  if (auto p = dynamic_cast<AnyHolder<T>*>(any.holder_.get())) {
    return p->value;
  }
  throw BadAnyCast();
};
#endif
// YOUR CODE...
