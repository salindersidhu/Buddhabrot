#include "ComplexNumber.hpp"

// Define multiplication for two complex numbers
ComplexNumber ComplexNumber::operator*(const ComplexNumber& other) {
    // (a + bi) (c + di)
    return ComplexNumber(_r * other._r - _i * other._i, _r * other._i +
        _i * other._r);
}
// Define addition for two complex numbers
ComplexNumber ComplexNumber::operator+(const ComplexNumber& other) {
    // (a + bi) + (c + di)
    return ComplexNumber(_r + other._r, _i + other._i);
}
// Retur the real component of the real number
double ComplexNumber::r() const {
    return _r;
}
// Return the imaginary component of the real number
double ComplexNumber::i() const {
    return _i;
}
// Return the square magnitude of the complex number
double ComplexNumber::squareMagnitude() const {
    return _r * _r + _i * _i;
}
