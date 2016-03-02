#include <chrono>
#include <random>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
using namespace chrono;

typedef unsigned int UINT;

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
void allocHeatmap(UINT**& heatmap, int width, int height) {
    heatmap = new UINT*[height];
    for (int i = 0; i < height; ++i) {
        heatmap[i] = new UINT[width];
        // Allocate each point in the heatmap with default a value of 0
        for (int j = 0; j < width; ++j) {
            heatmap[i][j] = 0;
        }
    }
}

void freeHeatmap(UINT**& heatmap, int height) {
    // Delete and free memory used by the rows of the heatmap pointer
    for (int i = 0; i < height; ++i) {
        delete[] heatmap[i];
        heatmap[i] = nullptr;
    }
    // Delete and free memory used by the heatmap pointer itself
    delete heatmap;
    heatmap = nullptr;
}

string elapsedTime(nanoseconds elapsedTime) {
    // Obtain the appropriate time for each unit
    hours hrs = duration_cast<hours>(elapsedTime);
    minutes mins = duration_cast<minutes>(elapsedTime - hrs);
    seconds secs = duration_cast<seconds>(elapsedTime - hrs - mins);
    milliseconds msecs = duration_cast<milliseconds>
		(elapsedTime - hrs - mins - secs);
    // Generate the string representing the output time
    stringstream outTime("");
    if (hrs.count() > 24) {
        outTime << hrs.count() / 24 << " Days, " << hrs.count() % 24 <<
			" Hours, ";
    } else if (hrs.count() > 0) {
        outTime << hrs.count() << " Hours, ";
    }
    if (mins.count() > 0) {
        outTime << mins.count() << " Minutes, ";
    }
    if (secs.count() > 0) {
        outTime << secs.count() << " Seconds, ";
    }
    if (msecs.count() > 0) {
        outTime << msecs.count() << " Milliseconds";
    }
    return outTime.str();
}

/*
* Main function.
*/
int main() {
    cout << "Generating PEM Image, please wait..." << endl;
    // Obtain the starting time
    auto startTime = high_resolution_clock::now();
    // Obtain the ending time
    auto endTime = high_resolution_clock::now();
    // Report the time elapsed and image generated message.
    cout << "Time Elapsed: " << elapsedTime(endTime - startTime) << endl;
    cout << "PEM image successfully generated. Press ENTER to exit." << endl;
    cin.ignore();

    return EXIT_SUCCESS;
}
