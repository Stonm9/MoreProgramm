#ifndef STRING_VIEW_H
#define STRING_VIEW_H
#include <cstddef>
class StringView {
  const char* str_;
  const char* end_;

 public:
  StringView() : str_(nullptr), end_(nullptr) {
  }
  StringView(const char* str) : str_(str), end_(str) {  // NOLINT
    while (*end_ != '\0') {
      ++end_;
    }
  }
  StringView(const char* str, size_t size) : str_(str), end_(str + size) {
  }
  // nodiscard - это значит что нужно возвращать куда-то значения.

  [[nodiscard]] size_t Size() const {
    return end_ - str_;
  }
  [[nodiscard]] size_t Length() const {
    return end_ - str_;
  }
  void RemovePrefix(size_t prefix_size) {
    str_ += prefix_size;
  }
  void RemoveSuffix(size_t suffix_size) {
    end_ -= suffix_size;
  }

  [[nodiscard]] char operator[](size_t idx) const {
    return *(str_ + idx);
  }
  [[nodiscard]] char At(size_t idx) const {
    return idx > Size() ? '\n' : str_[idx];
  }
  void Swap(StringView& other);
  [[nodiscard]] char Front() const {
    return str_[0];
  }
  [[nodiscard]] char Back() const {
    return str_[(end_ - str_) - 1];
  }
  [[nodiscard]] bool Empty() const {
    return str_ == end_;
  }
  [[nodiscard]] const char* Data() const {
    return str_;
  }
  [[nodiscard]] StringView Substr(size_t pos, size_t count = -1) const {
    StringView answer;
    answer.str_ = str_ + pos;
    answer.end_ = str_ + pos + count;
    return answer;
  }
};
#endif
