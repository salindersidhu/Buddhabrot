#include <chrono>
#include <random>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "ComplexNumber.hpp"
using namespace std;
using namespace chrono;

typedef unsigned int UINT;

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

int rowFromR(double r, double minR, double maxR, int imageHeight) {
    return static_cast<int>((r - minR) * (imageHeight / (maxR - minR)));
}

int colFromI(double i, double minI, double maxI, int imageWidth) {
    return static_cast<int>((i - minI) * (imageWidth / (maxI - minI)));
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
    while (n < numIters && z.squareMagnitude() <= 2.0) {
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
    long long numSamples, UINT& maxHeatmapValue) {
    // Configure the random number generator, seed and uniform distributions
    mt19937 rng;
    uniform_real_distribution<double> rDistribution(min.r(), max.r());
    uniform_real_distribution<double> iDistribution(min.i(), max.i());
    rng.seed(high_resolution_clock::now().time_since_epoch().count());
    auto next = high_resolution_clock::now() + seconds(5);
    // Collect numSamples samples where sample is just a random complex number
    for (long long sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex) {
        if (high_resolution_clock::now() > next) {
            next = high_resolution_clock::now() + seconds(30);
        }
        // Each sample gets a list of points if it escapes to infinity
        ComplexNumber sample(rDistribution(rng), iDistribution(rng));
        vector<ComplexNumber> pointsList = buddhabrotPoints(sample, numIters);
        for (ComplexNumber& point : pointsList) {
            if (point.r() <= max.r() && point.r() >= min.r() &&
				point.i() <= max.i() && point.i() >= min.i()) {
                // Obtain the row and column and increment heatmap position
                int row = rowFromR(point.r(), min.r(), max.r(), imageHeight);
                int col = colFromI(point.i(), min.i(), max.i(), imageWidth);
                ++heatmap[row][col];
                // Record new maximum heatmap value
                if (heatmap[row][col] > maxHeatmapValue) {
                    maxHeatmapValue = heatmap[row][col];
                }
            }
        }
    }
}

string elapsedTime(nanoseconds time) {
    // Obtain the appropriate time for each unit
    hours hrs = duration_cast<hours>(time);
    minutes mins = duration_cast<minutes>(time - hrs);
    seconds secs = duration_cast<seconds>(time - hrs - mins);
    milliseconds ms = duration_cast<milliseconds>(time - hrs - mins - secs);
    // Generate the string representing the output time
    stringstream timeElapsed("");
    if (hrs.count() > 24) {
        timeElapsed << hrs.count() / 24 << " Days, " << hrs.count() % 24 <<
			" Hours, ";
    } else if (hrs.count() > 0) {
        timeElapsed << hrs.count() << " Hours, ";
    }
    if (mins.count() > 0) {
        timeElapsed << mins.count() << " Minutes, ";
    }
    if (secs.count() > 0) {
        timeElapsed << secs.count() << " Seconds, ";
    }
    if (ms.count() > 0) {
        timeElapsed << ms.count() << " Milliseconds";
    }
    return timeElapsed.str();
}

/*
* Main function.
*/
int main() {
    // Define constants for the heatmap generator
    const ComplexNumber MINIMUM(-2.0, -2.0);
    const ComplexNumber MAXIMUM(1.0, 2.0);
    const int IMAGE_HEIGHT = 512;
    const int IMAGE_WIDTH = 512;
    const int RED_ITERS = 200;
    const int BLUE_ITERS = 200;
    const int GREEN_ITERS = 200;
    const long long int SAMPLE_COUNT = IMAGE_WIDTH * IMAGE_HEIGHT * 100;
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
    // Allocate a heatmap of the size of the image for each channel
    UINT maxHeatmapValue = 0;
    UINT** red;
    UINT** green;
    UINT** blue;
    allocHeatmap(red, IMAGE_WIDTH, IMAGE_HEIGHT);
    allocHeatmap(green, IMAGE_WIDTH, IMAGE_HEIGHT);
    allocHeatmap(blue, IMAGE_WIDTH, IMAGE_HEIGHT);
    // Generate the heatmap for each colour channel
    generateHeatmap(red, IMAGE_WIDTH, IMAGE_HEIGHT, MINIMUM, MAXIMUM,
        RED_ITERS, SAMPLE_COUNT, maxHeatmapValue);
    generateHeatmap(green, IMAGE_WIDTH, IMAGE_HEIGHT, MINIMUM, MAXIMUM,
        GREEN_ITERS, SAMPLE_COUNT, maxHeatmapValue);
    generateHeatmap(blue, IMAGE_WIDTH, IMAGE_HEIGHT, MINIMUM, MAXIMUM,
        BLUE_ITERS, SAMPLE_COUNT, maxHeatmapValue);
    // Scale the heatmap down for each colour channel
    for (int row = 0; row < IMAGE_HEIGHT; ++row) {
        for (int col = 0; col < IMAGE_WIDTH; ++col) {
            red[row][col] = colourFromHeatmap(red[row][col],
                maxHeatmapValue, 255);
            green[row][col] = colourFromHeatmap(green[row][col],
				maxHeatmapValue, 255);
            blue[row][col] = colourFromHeatmap(blue[row][col],
				maxHeatmapValue, 255);
        }
    }
    // Write the PPM header
    writePPMHeader(imageOut, "P3", 255, IMAGE_WIDTH, IMAGE_HEIGHT);
    // Write the PPM image from the colour heatmaps
    for (int row = 0; row < IMAGE_HEIGHT; ++row) {
        for (int col = 0; col < IMAGE_WIDTH; ++col) {
            imageOut << red[row][col] << " ";
            imageOut << green[row][col] << " ";
            imageOut << blue[row][col] << "   ";
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
