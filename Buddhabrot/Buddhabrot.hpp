#ifndef BUDDHABROT_HPP
#define BUDDHABROT_HPP

#include <string>
#include <random>
#include <chrono>
#include <vector>
#include <fstream>
#include <iostream>
#include "ComplexNumber.hpp"

using std::vector;
using std::ofstream;
using std::mt19937;
using std::uniform_real_distribution;
using std::cout;
using std::endl;
using namespace std::chrono;

typedef unsigned int UINT;
typedef unsigned long long int ULLI;

class Buddhabrot {
public:
    Buddhabrot(int width, int height, int samples, double minReal, double maxReal, double minImaginary, double maxImaginary, int redIterations, int greenIterations, int blueIterations);
    ~Buddhabrot();

    void generate();
    void flushToPPM(ofstream& outFile);
private:
    void allocHeatmap(UINT**& heatmap, int width, int height);
    void freeHeatmap(UINT**& heatmap, int height);
    void generateHeatmap(UINT** heatmap, int imageWidth, int imageHeight, const ComplexNumber& min, const ComplexNumber& max, int iterations, ULLI samples, UINT& maxHeatmapValue, ULLI& iterationCount, ULLI totalIterations);

    int getColourFromHeatmap(UINT heatmapValue, UINT maxHeatmapValue, int maxColour);
    int getRowFromReal(double real, double minReal, double maxReal, int imageHeight);
    int getColumnFromImaginary(double imaginary, double minImaginary, double maxImaginary, int imageWidth);

    vector<ComplexNumber> points(const ComplexNumber& c, int iterations);

    int imageWidth;
    int imageHeight;
    ULLI sampleCount;
    ComplexNumber min;
    ComplexNumber max;
    int redIterations;
    int blueIterations;
    int greenIterations;
    UINT** redHeatmap;
    UINT** blueHeatmap;
    UINT** greenHeatmap;
    UINT maxHeatmapValue;
};

#endif // BUDDHABROT_HPP
