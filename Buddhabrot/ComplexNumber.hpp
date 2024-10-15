#ifndef COMPLEX_NUMBER_HPP
#define COMPLEX_NUMBER_HPP

/*
 * ComplexNumber class represents a complex number and provides functions to
 * compute addition, multiplication, and square magnitude of complex numbers.
 */
class ComplexNumber {
public:
  ComplexNumber(double r = 0.0, double i = 0.0) : _r(r), _i(i) {}
  ComplexNumber(const ComplexNumber&) = default;

  ComplexNumber operator*(const ComplexNumber& other);
  ComplexNumber operator+(const ComplexNumber& other);

  inline double real() const { return _r; }
  inline double imaginary() const { return _i; }
  inline double squareMagnitude() const { return _r * _r + _i * _i; }

private:
  double _r;  // Real component
  double _i;  // Imaginary component
};

#endif // COMPLEX_NUMBER_HPP
