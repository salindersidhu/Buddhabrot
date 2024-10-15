#include <string>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Buddhabrot.hpp"

using namespace std;

void displayHelp() {
  cout << "Usage: ./Buddhabrot.exe [options]\n\n"
       << "Options:\n"
       << "  -W, -w <int>        Image width (default: 512)\n"
       << "  -H, -h <int>        Image height (default: 512)\n"
       << "  -R, -r <int>        Red iterations (default: 100)\n"
       << "  -G, -g <int>        Green iterations (default: 100)\n"
       << "  -B, -b <int>        Blue iterations (default: 100)\n"
       << "  -minR <double>      Minimum real value (default: -2.0)\n"
       << "  -minI <double>      Minimum imaginary value (default: -2.0)\n"
       << "  -maxR <double>      Maximum real value (default: 2.0)\n"
       << "  -maxI <double>      Maximum imaginary value (default: 2.0)\n"
       << "  -samples <int>      Number of samples per pixel (default: 100)\n"
       << "  -file <string>      Output filename (default: out.ppm)\n"
       << "  -help, -usage       Display this help message\n\n"
       << "Example:\n"
       << "  ./Buddhabrot.exe -w 1024 -h 1024 -r 200 -g 200 -b 200 -file fractal.ppm\n";
}

int main(int argc, char* argv[]) {
  int imageWidth = 512;
  int imageHeight = 512;
  int redIters = 100;
  int greenIters = 100;
  int blueIters = 100;
  double minR = -2.0;
  double minI = -2.0;
  double maxR = 2.0;
  double maxI = 2.0;
  int samples = 100;
  string filename = "out.ppm";

  // Check if any help flag is provided
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "-usage") == 0) {
      displayHelp();
      return EXIT_SUCCESS;
    }
  }

  // Parsing command-line arguments (case-insensitive for W, H, R, G, B)
  for (int i = 1; i < argc; ++i) {
    if ((strcmp(argv[i], "-W") == 0 || strcmp(argv[i], "-w") == 0) && i + 1 < argc) imageWidth = atoi(argv[++i]);
    else if ((strcmp(argv[i], "-H") == 0 || strcmp(argv[i], "-h") == 0) && i + 1 < argc) imageHeight = atoi(argv[++i]);
    else if ((strcmp(argv[i], "-R") == 0 || strcmp(argv[i], "-r") == 0) && i + 1 < argc) redIters = atoi(argv[++i]);
    else if ((strcmp(argv[i], "-G") == 0 || strcmp(argv[i], "-g") == 0) && i + 1 < argc) greenIters = atoi(argv[++i]);
    else if ((strcmp(argv[i], "-B") == 0 || strcmp(argv[i], "-b") == 0) && i + 1 < argc) blueIters = atoi(argv[++i]);
    else if (strcmp(argv[i], "-minR") == 0 && i + 1 < argc) minR = atof(argv[++i]);
    else if (strcmp(argv[i], "-minI") == 0 && i + 1 < argc) minI = atof(argv[++i]);
    else if (strcmp(argv[i], "-maxR") == 0 && i + 1 < argc) maxR = atof(argv[++i]);
    else if (strcmp(argv[i], "-maxI") == 0 && i + 1 < argc) maxI = atof(argv[++i]);
    else if (strcmp(argv[i], "-samples") == 0 && i + 1 < argc) samples = atoi(argv[++i]);
    else if (strcmp(argv[i], "-file") == 0 && i + 1 < argc) filename = argv[++i];
  }

  // Display user settings
  cout << "Image Width: " << imageWidth << "\nImage Height: " << imageHeight
       << "\nRed Iterations: " << redIters << "\nGreen Iterations: " << greenIters
       << "\nBlue Iterations: " << blueIters << "\nMin Real: " << minR
       << "\nMin Imaginary: " << minI << "\nMax Real: " << maxR
       << "\nMax Imaginary: " << maxI << "\nSamples: " << samples
       << "\nOutput Filename: " << filename << endl;

  // Start fractal generation
  cout << "Generating fractal data, please wait..." << endl;
  Buddhabrot buddhabrot(imageWidth, imageHeight, samples, minR, maxR, minI, maxI, redIters, greenIters, blueIters);
  buddhabrot.generate();

  // Output PPM file
  ofstream imageOut(filename);
  if (!imageOut) {
    cerr << "Error: Could not open file for writing!" << endl;
    return EXIT_FAILURE;
  }
  buddhabrot.flushToPPM(imageOut);
  imageOut.close();
  cout << "Fractal image saved to " << filename << endl;

  return EXIT_SUCCESS;
}
