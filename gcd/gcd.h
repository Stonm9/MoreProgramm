#ifndef MY_GCD
#define MY_GCD

template <class T>
T Gcd(T x, T y) {
  while (x > 0 && y > 0) {
    if (x >= y) {
      x = x % y;
    } else {
      y = y % x;
    }
  }
  return x > y ? x : y;
}

#endif