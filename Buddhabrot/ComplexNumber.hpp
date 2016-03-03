#ifndef COMPLEX_NUMBER_HPP
#define COMPLEX_NUMBER_HPP

/*
* ComplexNumber class represents a complex number and provides functions to
* compute addition, multiplication and square magnitude of complex numbers.
*/
class ComplexNumber {
public:
    // Constructor
    ComplexNumber(double r = 0.0, double i = 0.0) : _r(r), _i(i) {}
    ComplexNumber(const ComplexNumber&) = default;
    // ComplexNumber operator functions
    ComplexNumber operator*(const ComplexNumber& other);
    ComplexNumber operator+(const ComplexNumber& other);
    // ComplexNumber getter functions
    double r() const;
    double i() const;
    double squareMagnitude() const;
private:
    double _r;  // Real component
    double _i;  // Imaginary component
};

#endif // COMPLEX_NUMBER_HPP
