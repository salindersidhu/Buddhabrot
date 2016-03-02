#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <vector>
using namespace std;

typedef unsigned int HeatmapType;

/*
* ComplexNumber class represents a complex number and provides functions to
* compute addition, multiplication and square magnitude of complex numbers.
*/
class ComplexNumber {
public:
    // Constructor
    ComplexNumber(double r = 0.0, double i = 0.0) : real(r), imaginery(i) {}
    ComplexNumber(const ComplexNumber&) = default;
    // Operation functions
    ComplexNumber operator*(const ComplexNumber& other) {
        // (a + bi) * (c + di)
        return ComplexNumber(real * other.getReal() - imaginery *
			other.getImaginery(), real * other.getImaginery() +
			imaginery * other.getReal());
    }
    ComplexNumber operator+(const ComplexNumber& other) {
        // (a + bi) + (c + di)
        return ComplexNumber(real + other.getReal(),
			imaginery + other.getImaginery());
    }
    // ComplexNumber getter and setter functions
    double getReal() const {
        return real;
    }
    double getImaginery() const {
        return imaginery;
    }
    double getSQmagnitude() const {
        return real * real + imaginery * imaginery;
    }
private:
    // ComplexNumber variables
    double real, imaginery;
};

/*
* Utility functions.
*/

/*
* Main method.
*/
int main() {
    return EXIT_SUCCESS;
}
