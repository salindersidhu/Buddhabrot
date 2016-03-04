#include <string>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Buddhabrot.hpp"
using namespace std;
using namespace chrono;

/*
* Helper functions
*/
// Convert the time value from nanoseconds into a human readable time string
string timeElapsed(nanoseconds time) {
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

/* Return input from the user as a string. Return the default value if no input
was provided by the user */
string strFromUserInput(string title, string defaultVal) {
    cout << title << " [Default = " + defaultVal + "]: ";
    string input;
    getline(cin, input);
    if (!input.empty()) {
        return input;
    }
    return defaultVal;
}

/*
* Main function
*/
int main() {
    // Prompt the user to configure the variables
    cout << "Welcome to the Buddhabrot Fractal Image Generator" << endl;
    cout << "=================================================" << endl;
    cout << "Please setup variables (Press ENTER for Default)" << endl;
    int imageWidth = stoi(strFromUserInput("Image Width", "512"));
    int imageHeight = stoi(strFromUserInput("Image Height", "512"));
    int redIters = stoi(strFromUserInput("Red Iterations", "100"));
    int greenIters = stoi(strFromUserInput("Green Iterations", "100"));
    int blueIters = stoi(strFromUserInput("Blue Iterations", "100"));
    double minR = stod(strFromUserInput("Minimum Real", "-2.0"));
    double minI = stod(strFromUserInput("Minimum Imaginary", "-2.0"));
    double maxR = stod(strFromUserInput("Maximum Real", "2.0"));
    double maxI = stod(strFromUserInput("Maximum Imaginary", "2.0"));
    int samples = stoi(strFromUserInput("Samples", "100"));
    string filename = strFromUserInput("Filename", "out.ppm");

    // Obtain the starting time
    cout << "Generating PPM, this can take a while, please wait..." << endl;
    auto startTime = high_resolution_clock::now();

    // Initialize an output file stream for writing
    ofstream imageOut(filename);
    if (!imageOut) {
        // Display message and exit application if error on opening file
        cout << "Could not open image file for writing!" << endl;
        cout << "Press ENTER to exit." << endl;
        cin.ignore();
        return EXIT_FAILURE;
    }

    /* Initialize a new Buddhabrot object, generate the fractal PPM image file
    and write the fractal to a file */
    Buddhabrot buddhabrot(imageWidth, imageHeight, samples, minR, maxR, minI,
        maxI, redIters, blueIters, greenIters);
    buddhabrot.generate();
    buddhabrot.flushToPPMFile(imageOut);
    imageOut.close();

    // Obtain the ending time
    auto endTime = high_resolution_clock::now();

    // Report image generated message and time elapsed
    cout << "PPM generated in " << timeElapsed(endTime - startTime) <<
        ". Press ENTER to exit." << endl;
    cin.ignore();

    return EXIT_SUCCESS;
}
