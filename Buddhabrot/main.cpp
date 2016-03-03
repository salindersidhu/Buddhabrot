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
    ComplexNumber(double r = 0.0, double i = 0.0) : real(r), imaginary(i) {}
    ComplexNumber(const ComplexNumber&) = default;
    // Operation functions
    ComplexNumber operator*(const ComplexNumber& other) {
        // (a + bi) * (c + di)
        return ComplexNumber(real * other.getReal() - imaginary *
			other.getImaginary(), real * other.getImaginary() +
			imaginary * other.getReal());
    }
    ComplexNumber operator+(const ComplexNumber& other) {
        // (a + bi) + (c + di)
        return ComplexNumber(real + other.getReal(),
			imaginary + other.getImaginary());
    }
    // ComplexNumber getter and setter functions
    double getReal() const {
        return real;
    }
    double getImaginary() const {
        return imaginary;
    }
    double getSQmagnitude() const {
        return real * real + imaginary * imaginary;
    }
private:
    // ComplexNumber variables
    double real, imaginary;
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

void writePPMHeader(ofstream& imageOut, string id, int colourRange,
	int imageWidth, int imageHeight) {
    imageOut << id << endl;
    imageOut << imageWidth << " " << imageHeight << endl;
    imageOut << colourRange << endl;
}

int rowFromReal(double real, double minReal, double maxReal, int imageHeight) {
    return static_cast<int>((real - minReal) * (imageHeight /
		(maxReal - minReal)));
}

int columnFromImaginary(double imaginary, double minImaginary,
	double maxImaginary, int imageWidth) {
    return static_cast<int>((imaginary - minImaginary) *
		(imageWidth / (maxImaginary - minImaginary)));
}

int colourFromHeatmap(UINT inputValue, UINT maxHeatmapValue, int maxColour) {
    double scale = static_cast<double>(maxColour) / maxHeatmapValue;
    return inputValue * scale;
}

vector<ComplexNumber> buddhabrotPoints(const ComplexNumber& c, int numIters) {
    int n = 0;
    ComplexNumber z;
    // Generate a vector of complex numbers and reserve required space
    vector<ComplexNumber> outputNums;
    outputNums.reserve(numIters);
    // The iteration that generates the points
    while (n < numIters && z.getSQmagnitude() <= 2.0) {
        z = z * z + c;
        ++n;
        outputNums.push_back(z);
    }
    /* If the point remains bounded through numIters, the point is bounded in
    the Mandelbrot set and can be ignored */
    if (n == numIters) {
        return vector<ComplexNumber>();
    } else {
        return outputNums;
    }
}

void generateHeatmap(UINT** heatmap, int imageWidth, int imageHeight,
    const ComplexNumber& min, const ComplexNumber& max, int numIters,
    long long numSamples, UINT& maxHeatmapValue, string consoleMessagePrefix) {
    // Configure the random number generator, seed and uniform distributions
    mt19937 randomNumGenerator;
    uniform_real_distribution<double> realDistribution(min.getReal(),
		max.getReal());
    uniform_real_distribution<double> imaginaryDistribution(min.getImaginary(),
		max.getImaginary());
    randomNumGenerator.seed(high_resolution_clock::now().time_since_epoch().
		count());
    auto next = high_resolution_clock::now() + seconds(5);
    // Collect numSamples samples where sample is just a random complex number
    for (long long sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex) {
        if (high_resolution_clock::now() > next) {
            next = high_resolution_clock::now() + seconds(30);
            cout << consoleMessagePrefix << "Samples Taken: " << sampleIndex <<
				"/" << numSamples << endl;
        }
        // Each sample gets a list of points if it escapes to infinity
        ComplexNumber sample(realDistribution(randomNumGenerator),
			imaginaryDistribution(randomNumGenerator));
        vector<ComplexNumber> pointsList = buddhabrotPoints(sample, numIters);
        for (ComplexNumber& point : pointsList) {
            if (point.getReal() <= max.getReal() && point.getReal() >=
				min.getReal() && point.getImaginary() <= max.getImaginary() &&
                point.getImaginary() >= min.getImaginary()) {
                int row = rowFromReal(point.getReal(), min.getReal(),
					max.getReal(), imageHeight);
                int column = columnFromImaginary(point.getImaginary(),
					min.getImaginary(), max.getImaginary(), imageWidth);
                ++heatmap[row][column];
                if (heatmap[row][column] > maxHeatmapValue) {
                    maxHeatmapValue = heatmap[row][column];
                }
            }
        }
    }
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
    // Define constants for the heatmap generator
    const ComplexNumber MINIMUM(-2.0, -2.0);
    const ComplexNumber MAXIMUM(1.0, 2.0);
    const int IMAGE_HEIGHT = 1024;
    const int IMAGE_WIDTH = 1024;
    const int RED_ITERS = 50;
    const int BLUE_ITERS = 50;
    const int GREEN_ITERS = 50;
    const long long int SAMPLE_COUNT = IMAGE_WIDTH * IMAGE_HEIGHT * 30;
    // Obtain the starting time
    cout << "Generating PEM Image, please wait..." << endl;
    auto startTime = high_resolution_clock::now();
    // Open output file stream
    ofstream imageOut("out.ppm");
    if (!imageOut) {
        cout << "Could not open image file for writing!" << endl;
        cout << "Press ENTER to continue..." << endl;
        cin.ignore();
        return EXIT_FAILURE;
    }
    // Allocate a heatmap of the size of the image
    UINT maxHeatmapValue = 0;
    UINT** red;
    UINT** green;
    UINT** blue;
    allocHeatmap(red, IMAGE_WIDTH, IMAGE_HEIGHT);
    allocHeatmap(green, IMAGE_WIDTH, IMAGE_HEIGHT);
    allocHeatmap(blue, IMAGE_WIDTH, IMAGE_HEIGHT);
    // Generate the heatmap
    generateHeatmap(red, IMAGE_WIDTH, IMAGE_HEIGHT, MINIMUM, MAXIMUM,
        RED_ITERS, SAMPLE_COUNT, maxHeatmapValue, "Red Channel: ");
    generateHeatmap(green, IMAGE_WIDTH, IMAGE_HEIGHT, MINIMUM, MAXIMUM,
        GREEN_ITERS, SAMPLE_COUNT, maxHeatmapValue, "Green Channel: ");
    generateHeatmap(blue, IMAGE_WIDTH, IMAGE_HEIGHT, MINIMUM, MAXIMUM,
        BLUE_ITERS, SAMPLE_COUNT, maxHeatmapValue, "Blue Channel: ");
    // Scale the heatmap down
    for (int row = 0; row < IMAGE_HEIGHT; ++row) {
        for (int column = 0; column < IMAGE_WIDTH; ++column) {
            red[row][column] = colourFromHeatmap(red[row][column],
                maxHeatmapValue, 255);
            green[row][column] = colourFromHeatmap(green[row][column],
                maxHeatmapValue, 255);
            blue[row][column] = colourFromHeatmap(blue[row][column],
                maxHeatmapValue, 255);
        }
    }
    // Write the PPM header
    writePPMHeader(imageOut, "P3", 255, IMAGE_WIDTH, IMAGE_HEIGHT);
    // Write the PPM image from the colour heatmaps
    for (int row = 0; row < IMAGE_HEIGHT; ++row) {
        for (int column = 0; column < IMAGE_WIDTH; ++column) {
            imageOut << red[row][column] << " ";
            imageOut << green[row][column] << " ";
            imageOut << blue[row][column] << "   ";
        }
        imageOut << endl;
    }
    imageOut.close();
    // Free the colour heatmaps
    freeHeatmap(red, IMAGE_HEIGHT);
    freeHeatmap(green, IMAGE_HEIGHT);
    freeHeatmap(blue, IMAGE_HEIGHT);
    // Obtain the ending time
    auto endTime = high_resolution_clock::now();
    // Report the time elapsed and image generated message.
    cout << "Time Elapsed: " << elapsedTime(endTime - startTime) << endl;
    cout << "PEM image successfully generated. Press ENTER to exit." << endl;
    cin.ignore();

    return EXIT_SUCCESS;
}
