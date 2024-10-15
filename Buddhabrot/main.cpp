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

// Convert nanoseconds to a human-readable time string
string static timeElapsed(nanoseconds time) {
  hours h = duration_cast<hours>(time);
  minutes m = duration_cast<minutes>(time - h);
  seconds s = duration_cast<seconds>(time - h - m);
  milliseconds ms = duration_cast<milliseconds>(time - h - ms - s);

  stringstream result;
  if (h.count() >= 24) {
    result << h.count() / 24 << " Days, " << h.count() % 24 << " Hours, ";
  } else if (h.count() > 0) {
    result << h.count() << " Hours, ";
  }
  if (m.count() > 0) {
      result << m.count() << " Minutes, ";
  }
  if (s.count() > 0) {
      result << s.count() << " Seconds, ";
  }
  if (ms.count() > 0) {
      result << ms.count() << " Milliseconds";
  }

  return result.str();
}

// Get user input with a default value
string getUserInput(const string& prompt, const string& defaultValue) {
  cout << prompt << " [Default = " + defaultValue + "]: ";
  string input;
  getline(cin, input);
  return input.empty() ? defaultValue : input;
}

/*
 * Main function
 */
int main() {
  // Prompt the user to configure the variables
  cout << "Welcome to the Buddhabrot Fractal Image Generator" << endl;
  cout << "=================================================" << endl;
  cout << "Please configure the settings (Press ENTER for default values)" << endl;

  int imageWidth = stoi(getUserInput("Image Width", "512"));
  int imageHeight = stoi(getUserInput("Image Height", "512"));
  int redIters = stoi(getUserInput("Red Iterations", "100"));
  int greenIters = stoi(getUserInput("Green Iterations", "100"));
  int blueIters = stoi(getUserInput("Blue Iterations", "100"));
  double minR = stod(getUserInput("Minimum Real", "-2.0"));
  double minI = stod(getUserInput("Minimum Imaginary", "-2.0"));
  double maxR = stod(getUserInput("Maximum Real", "2.0"));
  double maxI = stod(getUserInput("Maximum Imaginary", "2.0"));
  int samples = stoi(getUserInput("Samples", "100"));
  string filename = getUserInput("Filename", "out.ppm");

  // Start timer
  cout << "Generating fractal data, please wait..." << endl;
  auto startTime = high_resolution_clock::now();

  // Initialize output file stream
  ofstream imageOut(filename);
  if (!imageOut) {
    cerr << "Error: Unable to open image file for writing!" << endl;
    cin.ignore();
    return EXIT_FAILURE;
  }

  // Generate Buddhabrot fractal and write to PPM file
  Buddhabrot buddhabrot(imageWidth, imageHeight, samples, minR, maxR, minI, maxI, redIters, blueIters, greenIters);
  buddhabrot.generate();
  cout << "Writing fractal data to PPM file..." << endl;
  buddhabrot.flushToPPM(imageOut);
  imageOut.close();

  // End timer and report time elapsed
  auto endTime = high_resolution_clock::now();
  cout << "PPM file generated in " << timeElapsed(endTime - startTime) << ". Press ENTER to exit." << endl;
  cin.ignore();

  return EXIT_SUCCESS;
}
