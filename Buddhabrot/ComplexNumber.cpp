#include "ComplexNumber.hpp"

/*
 * Multiplication for two complex numbers
 */
ComplexNumber ComplexNumber::operator*(const ComplexNumber& other) {
  return ComplexNumber(_r * other._r - _i * other._i, _r * other._i + _i * other._r);
}

/*
 * Addition for two complex numbers
 */
ComplexNumber ComplexNumber::operator+(const ComplexNumber& other) {
  return ComplexNumber(_r + other._r, _i + other._i);
}
