// Работа с комплексными числами.
#include <iostream>
// Компелксные числа
// x + y*i
// где i мнимое число при i * i даёт -1
struct Complex {
  double re;  // действительная часть
  double im;  // мнимая часть
  // Complex operator-(Complex rhs) const {  // lhs - это текущей объект
  //   return {re - rhs.re, im - rhs.im};
  // }
  //[[nodiscard]] operator double() const{
  //  return re;
  //}
  [[nodiscard]] operator double() const {
    return re;
  }
};
Complex Sum(Complex x, Complex y) {
  return {x.re + y.re, x.im + y.im};
}
Complex operator+(const Complex& lhs, const Complex& rhs) {  // lhs - Left(Right) hand side
  return {lhs.re + rhs.re, lhs.im + rhs.im};
}
Complex operator+(const Complex& value) {
  return value;
}
Complex operator-(const Complex& lhs, const Complex& rhs) {  // lhs - Left(Right) hand side

  return {lhs.re - rhs.re, lhs.im - rhs.im};
}
Complex operator-(const Complex& value) {
  return {-value.re, -value.im};
}
Complex operator++(const Complex& value, int) {
  return value++;
}

std::istream& operator>>(std::istream& is, Complex& complex) {
  is >> complex.re >> complex.im;
  return is;
}
std::ostream& operator<<(std::ostream& os, const Complex& complex) {
  os << complex.re << " " << complex.im << "i";
  return os;
}
// a * x^2 + b * x + c
class Quadratic {
  double a_;
  double b_;
  double c_;

 public:
  Quadratic(double a, double b, double c) : a_(a), b_(b), c_(c) {
  }
  double operator()(double x) const {
    return a_ * x * x + b_ * x + c_;
  }
};
int main() {
  Quadratic function(1, 2, 3);
  function(5);

  Complex x;
  Complex y;
  std::cin >> x >> y;
  std::cout << x << " " << y << '\n';
}