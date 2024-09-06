#ifndef ARRAY_H
#define ARRAY_H
#include <utility>
template <class T, size_t N>
struct Array {
  T data[N];
  [[nodiscard]] size_t Size() const {
    return N;
  }
  const T& operator[](size_t index) const {
    return data[index];
  }
  T& operator[](size_t index) {
    return data[index];
  }
  [[nodiscard]] const T& Front() const {
    return data[0];
  }
  T& Front() {
    return data[0];
  }
  [[nodiscard]] const T& Back() const {
    return data[N - 1];
  }
  T& Back() {
    return data[N - 1];
  }
  [[nodiscard]] const T* Data() const {
    return &data[0];
  }
  [[nodiscard]] T* Data() {
    return &data[0];
  }
  [[nodiscard]] bool Empty() const {
    return !N;
  }
  void Fill(const T& value) {
    for (size_t i = 0; i < N; ++i) {
      data[i] = value;
    }
  }
  void Swap(Array<T, N>& other) {
    std::swap(data, other.data);
  }
};
//-----------------------------------
template <class T>
size_t GetSize(const T&) {
  return 0;
}
template <class T, size_t N>
size_t GetSize(T (&array)[N]) {
  return sizeof(array) / sizeof(T);
}
//-----------------------------------
template <class T>
size_t GetRank(const T&) {
  return 0;
}
template <class T, size_t N>
size_t GetRank(T (&array)[N]) {
  return 1 + GetRank(array[0]);
}
//-----------------------------------
template <class T>
size_t GetNumElements(const T&) {
  return 1;
}
template <class T, size_t N>
size_t GetNumElements(T (&array)[N]) {
  return N * GetNumElements(array[0]);
}
#endif
