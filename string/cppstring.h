#ifndef STRING_H
#define STRING_H
#include <stdexcept>
class StringOutOfRange : public std::out_of_range {
 public:
  StringOutOfRange() : std::out_of_range("StringOutOfRange") {
    // YOUR CODE...
  }
};

class String {
  char* str_;
  size_t size_;
  size_t capacity_;  // Вместимость
  //-------------------------------------------------
  size_t Strlen(const char* str) {
    size_t len = 0;
    while (*str != '\0') {
      len++;
      str++;
    }
    return len;
  }
  //-------------------------------------------------
 public:
  // Конструктор по умолчанию
  String() : str_(nullptr), size_(0), capacity_(0) {
  }
  String(size_t count, const char symbol) {
    if (count == 0) {
      str_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    } else {
      str_ = new char[count + 1];
      for (size_t i = 0; i < count; i++) {
        str_[i] = symbol;
      }
      str_[count] = '\0';
      size_ = count;
      capacity_ = 2 * size_;
    }
  }

  String(const char* other, size_t size) : str_(new char[size + 1]), size_(size), capacity_(size) {
    std::copy(other, other + size, str_);
    str_[size] = '\0';
  }

  String(const char* other) : String(other, Strlen(other)) {  // NOLINT
  }
  // Конструктор копирования
  String(const String& other) {
    if (this != &other) { //проверка на самокопирование
      if (other.str_ == nullptr) {
        str_ = nullptr;
        size_ = 0;
        capacity_ = 0;
      } else {
        str_ = new char[other.size_ + 1];
        std::copy(other.str_, other.str_ + other.size_, str_);
        str_[other.size_] = '\0';
        size_ = other.size_;
        capacity_ = other.capacity_;
      }
    }
  }
  // Конструктор перемещания
  String(String&& other) : str_(other.str_), size_(other.size_), capacity_(other.capacity_) {
    other.str_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  [[nodiscard]] size_t Size() const {
    return size_;
  }
  [[nodiscard]] size_t Capacity() const {
    return capacity_;
  }
  [[nodiscard]] size_t Length() const {
    return size_;
  }
  char& Front() const {
    return str_[0];
  }
  char& Back() const {
    return str_[size_ - 1];
  }
  const char* Data() const {
    return str_;
  }
  char* CStr() {
    return str_;
  }
  const char* CStr() const {
    return str_;
  }

  [[nodiscard]] bool Empty() const {
    return (size_ == 0);
  }
  void Clear() {
    size_ = 0;
  }

  void Resize(size_t size, char symbol = ' ') {
    if (size == size_) {
      return;
    }
    if (size < size_) {  // Уменьшение размера
      size_ = size;
      str_[size_] = '\0';
    } else {  // Увеличение размера
      if (capacity_ < size) {
        capacity_ = size;
        char* new_str = new char[capacity_ + 1];
        std::copy(str_, str_ + size_, new_str);
        for (size_t i = size_; i < capacity_; ++i) {
          new_str[i] = symbol;
        }
        new_str[capacity_] = '\0';
        delete[] str_;
        str_ = new_str;
      } else {
        for (size_t i = size_; i < size; ++i) {
          str_[i] = symbol;
        }
        str_[size] = '\0';
      }
      size_ = size;
    }
  }

  void Reserve(size_t value) {
    if (capacity_ >= value) {
      return;
    }

    capacity_ = value;
    char* new_str = new char[capacity_ + 1];
    std::copy(str_, str_ + size_ + 1, new_str);
    delete[] str_;
    str_ = new_str;
  }
  void ShrinkToFit() {
    if (capacity_ > size_) {
      char* new_str = new char[size_ + 1];
      std::copy(str_, str_ + size_, new_str);
      new_str[size_] = '\0';
      delete[] str_;
      str_ = new_str;
      capacity_ = size_;
    }
  }
  void Swap(String& other) {
    std::swap(str_, other.str_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }
  void PushBack(char symbol) {
    if (size_ + 1 >= capacity_) {
      // Увеличиваем емкость, если нужно
      capacity_ = (capacity_ == 0 ? 1 : capacity_ * 2);
      char* new_str = new char[capacity_ + 1];
      if (str_ != nullptr) {
        std::copy(str_, str_ + size_, new_str);
        delete[] str_;
      }
      str_ = new_str;
    }

    str_[size_] = symbol;
    ++size_;
    str_[size_] = '\0';
  }
  void PopBack() {
    size_ = size_ - 1;
    str_[size_] = '\0';
  }
  // Операция копирующее присваивания
  String& operator=(const String& other) {
    if (this != &other) {  // Защита от самоприсваивания
      delete[] str_;
      if (other.str_ == nullptr) {
        str_ = nullptr;
        size_ = 0;
        capacity_ = 0;
      } else {
        str_ = new char[other.size_ + 1];
        std::copy(other.str_, other.str_ + other.size_, str_);
        str_[other.size_] = '\0';
        size_ = other.size_;
        capacity_ = other.capacity_;
      }
    }
    return *this;
  }
  // Перемещающее присваивание (Операция присваивания временного объекта)
  String& operator=(String&& other) {
    if (this != &other) {
      delete[] str_;
      str_ = other.str_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.str_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }
    return *this;
  }

  String& operator+=(const String& other) {
    if (other.size_ == 0) {
      return *this;
    }
    const auto new_size = size_ + other.size_;
    if (capacity_ < new_size) {
      capacity_ = 2 * new_size;
      const auto new_str = new char[capacity_ + 1];
      std::copy(str_, str_ + size_, new_str);
      delete[] str_;
      str_ = new_str;
    }
    std::copy(other.str_, other.str_ + other.size_ + 1, str_ + size_);
    size_ = new_size;
    return *this;
  }

  char& At(size_t idx) {
    if (idx < size_) {
      return str_[idx];
    }
    throw StringOutOfRange();  // Исключение при выходе за границы
  }

  const char& At(size_t idx) const {
    if (idx < size_) {
      return str_[idx];
    }
    throw StringOutOfRange();
  }
  char& operator[](size_t idx) {
    if (idx < size_) {
      return str_[idx];
    }
    throw StringOutOfRange();
  }
  const char& operator[](size_t idx) const {
    return str_[idx];
  }
  ~String() {
    delete[] str_;
  }
};

bool inline operator==(const String& lhs, const String& rhs) {
  if (lhs.Size() != rhs.Size()) {
    return false;
  }
  for (size_t i = 0; i < lhs.Size(); ++i) {
    if (lhs[i] != rhs[i]) {
      return false;
    }
  }
  return true;
}
bool inline operator!=(const String& lhs, const String& rhs) {
  return !(lhs == rhs);
}
bool inline operator<(const String& lhs, const String& rhs) {
  size_t min_size = std::min(lhs.Size(), rhs.Size());
  for (size_t i = 0; i < min_size; ++i) {
    if (lhs[i] < rhs[i]) {
      return true;
    }
    if (lhs[i] > rhs[i]) {
      return false;
    }
  }
  return lhs.Size() < rhs.Size();
}

bool inline operator>(const String& lhs, const String& rhs) {
  return rhs < lhs;
}

bool inline operator<=(const String& lhs, const String& rhs) {
  return !(rhs < lhs);
}

bool inline operator>=(const String& lhs, const String& rhs) {
  return !(lhs < rhs);
}

String inline operator+(const String& lhs, const String& rhs) {
  String tmp(lhs);
  tmp += rhs;
  return tmp;
}
// Операция вывода в поток
inline std::ostream& operator<<(std::ostream& os, const String& string) {
  if (string.Data() == nullptr) {
    return os;
  }
  os << string.Data();
  return os;
}

#endif