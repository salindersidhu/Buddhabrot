#include "Buddhabrot.hpp"

Buddhabrot::Buddhabrot(int width, int height, int samples, double minReal,
                       double maxReal, double minImaginary, double maxImaginary,
                       int redIterations, int greenIterations,
                       int blueIterations)
  : imageWidth(width), imageHeight(height), sampleCount(width * height * samples),
    min(minReal, minImaginary), max(maxReal, maxImaginary), redIterations(redIterations),
    blueIterations(blueIterations), greenIterations(greenIterations), maxHeatmapValue(0),
    iterationCount(0), totalIterations(sampleCount * 3) {
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

void Buddhabrot::generate() {
  generateHeatmap(redHeatmap, redIterations);
  generateHeatmap(greenHeatmap, greenIterations);
  generateHeatmap(blueHeatmap, blueIterations);

  // Scale heatmap values to a max color of 255
  for (int row = 0; row < imageHeight; ++row) {
    for (int col = 0; col < imageWidth; ++col) {
      redHeatmap[row][col] = getColourFromHeatmap(redHeatmap[row][col]);
      greenHeatmap[row][col] = getColourFromHeatmap(greenHeatmap[row][col]);
      blueHeatmap[row][col] = getColourFromHeatmap(blueHeatmap[row][col]);
    }
  }
}

void Buddhabrot::flushToPPM(ofstream& outFile) {
  outFile << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";
  for (int row = 0; row < imageHeight; ++row) {
    for (int col = 0; col < imageWidth; ++col) {
      outFile << redHeatmap[row][col] << " " << greenHeatmap[row][col] << " " << blueHeatmap[row][col] << " ";
    }
    outFile << endl;
  }
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

  auto nextReport = high_resolution_clock::now() + seconds(5);

  for (ULLI i = 0; i < sampleCount; ++i) {
    if (high_resolution_clock::now() > nextReport) {
      cout << "Progress: " << (static_cast<double>(iterationCount) / totalIterations) * 100 << "%\n";
      nextReport = high_resolution_clock::now() + seconds(30);
    }
    
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
    ++iterationCount;
  }
}
