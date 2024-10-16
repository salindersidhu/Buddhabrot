#include "Buddhabrot.hpp"

// Mutex for thread synchronization during progress reporting
mutex progressMutex;
atomic<ULLI> iterationCount(0); // Shared atomic iteration counter

Buddhabrot::Buddhabrot(int width, int height, int samples, double minR,
                       double maxR, double minI, double maxI, int redIterations,
                       int greenIterations, int blueIterations, const string& filename,
                       bool verbose)
  : imageWidth(width), imageHeight(height), sampleCount(width * height * samples),
    min(minR, minI), max(maxR, maxI), redIterations(redIterations),
    blueIterations(blueIterations), greenIterations(greenIterations),
    maxHeatmapValue(0), filename(filename), totalIterations(sampleCount * 3),
    verbose(verbose) {
  // Allocate heatmaps for each color channel
  allocHeatmap(redHeatmap);
  allocHeatmap(blueHeatmap);
  allocHeatmap(greenHeatmap);
}

Buddhabrot::~Buddhabrot() {
  freeHeatmap(redHeatmap);
  freeHeatmap(blueHeatmap);
  freeHeatmap(greenHeatmap);
}

int Buddhabrot::generate() {
  // Create threads for each color channel
  thread redThread(&Buddhabrot::generateHeatmap, this, redHeatmap, redIterations);
  thread greenThread(&Buddhabrot::generateHeatmap, this, greenHeatmap, greenIterations);
  thread blueThread(&Buddhabrot::generateHeatmap, this, blueHeatmap, blueIterations);

  // Create a progress reporting thread only if verbose is enabled
  thread progressThread;
  if (verbose) {
    progressThread = thread(&Buddhabrot::reportProgress, this);
  }

  // Wait for all threads to finish
  redThread.join();
  greenThread.join();
  blueThread.join();

  // Notify the progress thread that work is done
  if (verbose) {
    progressThread.join();
  }

  // Scale heatmap values to a max color of 255
  scaleHeatmapsToColor();

  // Output heatmaps to ppm
  return flushToPPM();
}

int Buddhabrot::flushToPPM() {
  ofstream imageOut(filename);
  if (!imageOut) {
    return EXIT_FAILURE;
  }

  imageOut << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";
  for (int row = 0; row < imageHeight; ++row) {
    for (int col = 0; col < imageWidth; ++col) {
        imageOut << redHeatmap[row][col] << " " << greenHeatmap[row][col] << " " << blueHeatmap[row][col] << " ";
    }
    imageOut << endl;
  }
  imageOut.close();
  
  return EXIT_SUCCESS;
}

void Buddhabrot::allocHeatmap(UINT**& heatmap) {
  heatmap = new UINT*[imageHeight];
  for (int i = 0; i < imageHeight; ++i) {
    heatmap[i] = new UINT[imageWidth]();
  }
}

void Buddhabrot::freeHeatmap(UINT**& heatmap) {
  for (int i = 0; i < imageHeight; ++i) {
    delete[] heatmap[i];
  }
  delete[] heatmap;
  heatmap = nullptr;
}

int Buddhabrot::getColourFromHeatmap(UINT heatmapValue) {
  double scale = static_cast<double>(255) / maxHeatmapValue;
  return static_cast<int>(heatmapValue * scale);
}

int Buddhabrot::getRowFromReal(double real) {
  return static_cast<int>((real - min.real()) * (imageHeight / (max.real() - min.real())));
}

int Buddhabrot::getColumnFromImaginary(double imaginary) {
  return static_cast<int>((imaginary - min.imaginary()) * (imageWidth / (max.imaginary() - min.imaginary())));
}

vector<ComplexNumber> Buddhabrot::points(const ComplexNumber& c, int iterations) {
  vector<ComplexNumber> points;
  ComplexNumber z;
  points.reserve(iterations);

  for (int n = 0; n < iterations && z.squareMagnitude() <= 2.0; ++n) {
    z = z * z + c;
    points.push_back(z);
  }

  // Point is bounded in the Mandelbrot set
  if (points.size() == iterations) return {};

  return points;
}

void Buddhabrot::generateHeatmap(UINT** heatmap, int iterations) {
  mt19937 rng(static_cast<UINT>(high_resolution_clock::now().time_since_epoch().count()));
  uniform_real_distribution<double> rDist(min.real(), max.real());
  uniform_real_distribution<double> iDist(min.imaginary(), max.imaginary());

  for (ULLI i = 0; i < sampleCount; ++i) {
    ComplexNumber sample(rDist(rng), iDist(rng));
    vector<ComplexNumber> points = Buddhabrot::points(sample, iterations);
    
    for (const ComplexNumber& point : points) {
      if (point.real() >= min.real() && point.real() <= max.real() && point.imaginary() >= min.imaginary() && point.imaginary() <= max.imaginary()) {
        int row = getRowFromReal(point.real());
        int col = getColumnFromImaginary(point.imaginary());
        
        if (++heatmap[row][col] > maxHeatmapValue) {
          maxHeatmapValue = heatmap[row][col];
        }
      }
    }
    
    // Increment the shared iteration count atomically
    iterationCount++;
  }
}

void Buddhabrot::scaleHeatmapsToColor() {
  for (int row = 0; row < imageHeight; ++row) {
    for (int col = 0; col < imageWidth; ++col) {
      redHeatmap[row][col] = getColourFromHeatmap(redHeatmap[row][col]);
      greenHeatmap[row][col] = getColourFromHeatmap(greenHeatmap[row][col]);
      blueHeatmap[row][col] = getColourFromHeatmap(blueHeatmap[row][col]);
    }
  }
}

void Buddhabrot::reportProgress() {
  auto nextReport = high_resolution_clock::now() + seconds(1);

  while (iterationCount < totalIterations) {
    if (high_resolution_clock::now() >= nextReport) {
      lock_guard<mutex> lock(progressMutex); // Synchronize output
      double progress = (static_cast<double>(iterationCount) / totalIterations) * 100;
      cout << "Progress: " << round(progress) << "%\n";
      nextReport = high_resolution_clock::now() + seconds(1);
    }
  }

  // Print final progress when done
  {
    lock_guard<mutex> lock(progressMutex);
    cout << "Progress: 100%\n";
  }
}
