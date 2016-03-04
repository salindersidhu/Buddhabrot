#ifndef BUDDHABROT_HPP
#define BUDDHABROT_HPP

#include <string>
#include <random>
#include <chrono>
#include <vector>
#include <fstream>
#include <iostream>
#include "ComplexNumber.hpp"
using namespace std;
using namespace chrono;

typedef unsigned int UINT;
typedef long long int LLI;

class Buddhabrot {
public:
    // Constructor and destructor
    Buddhabrot(int, int, int, double, double, double, double, int, int, int);
    virtual ~Buddhabrot() {};
    // Buddhabrot public functions
    void generate();
    void flushToPPMFile(ofstream&);
private:
    // Private functions
    void freeHeatmap(UINT**&, int);
    void allocHeatmap(UINT**&, int, int);
    int colourFromHeatmap(UINT, UINT, int);
    int rowFromR(double, double, double, int);
    int colFromI(double, double, double, int);
    vector<ComplexNumber> buddhabrotPoints(const ComplexNumber&, int);
    void generateHeatmap(UINT**, int, int, const ComplexNumber&,
        const ComplexNumber&, int, LLI, UINT&, LLI&, LLI);
    // Private variables
    int imageWidth;
    int imageHeight;
    LLI sampleCount;
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
