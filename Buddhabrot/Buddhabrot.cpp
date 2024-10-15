#include "Buddhabrot.hpp"

Buddhabrot::Buddhabrot(int width, int height, int samples, double minReal,
                       double maxReal, double minImaginary, double maxImaginary,
                       int redIterations, int greenIterations,
                       int blueIterations)
  : imageWidth(width), imageHeight(height), sampleCount(width * height * samples),
    min(minReal, minImaginary), max(maxReal, maxImaginary), redIterations(redIterations),
    blueIterations(blueIterations), greenIterations(greenIterations), maxHeatmapValue(0) {
  // Allocate heatmaps for each color channel
  allocHeatmap(redHeatmap, imageWidth, imageHeight);
  allocHeatmap(blueHeatmap, imageWidth, imageHeight);
  allocHeatmap(greenHeatmap, imageWidth, imageHeight);
}

Buddhabrot::~Buddhabrot() {
  freeHeatmap(redHeatmap, imageHeight);
  freeHeatmap(blueHeatmap, imageHeight);
  freeHeatmap(greenHeatmap, imageHeight);
}

void Buddhabrot::generate() {
  ULLI iterationCount = 0;
  ULLI totalIterations = sampleCount * 3;

  generateHeatmap(redHeatmap, imageWidth, imageHeight, min, max,
                  redIterations, sampleCount, maxHeatmapValue, iterationCount, totalIterations);
  generateHeatmap(greenHeatmap, imageWidth, imageHeight, min, max,
                  greenIterations, sampleCount, maxHeatmapValue, iterationCount, totalIterations);
  generateHeatmap(blueHeatmap, imageWidth, imageHeight, min, max,
                  blueIterations, sampleCount, maxHeatmapValue, iterationCount, totalIterations);

  // Scale heatmap values to a max color of 255
  for (int row = 0; row < imageHeight; ++row) {
    for (int col = 0; col < imageWidth; ++col) {
      redHeatmap[row][col] = getColourFromHeatmap(redHeatmap[row][col], maxHeatmapValue, 255);
      greenHeatmap[row][col] = getColourFromHeatmap(greenHeatmap[row][col], maxHeatmapValue, 255);
      blueHeatmap[row][col] = getColourFromHeatmap(blueHeatmap[row][col], maxHeatmapValue, 255);
    }
  }
}

void Buddhabrot::flushToPPM(ofstream& outFile) {
  outFile << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";
  for (int row = 0; row < imageHeight; ++row) {
    for (int col = 0; col < imageWidth; ++col) {
      outFile << redHeatmap[row][col] << " " << greenHeatmap[row][col] << " " << blueHeatmap[row][col] << "   ";
    }
    outFile << endl;
  }
}

void Buddhabrot::allocHeatmap(UINT**& heatmap, int width, int height) {
  heatmap = new UINT*[height];
  for (int i = 0; i < height; ++i) {
    heatmap[i] = new UINT[width]();
  }
}

void Buddhabrot::freeHeatmap(UINT**& heatmap, int height) {
  for (int i = 0; i < height; ++i) {
    delete[] heatmap[i];
  }
  delete[] heatmap;
  heatmap = nullptr;
}

int Buddhabrot::getColourFromHeatmap(UINT heatmapValue, UINT maxHeatmapValue, int maxColour) {
  double scale = static_cast<double>(maxColour) / maxHeatmapValue;
  return static_cast<int>(heatmapValue * scale);
}

int Buddhabrot::getRowFromReal(double real, double minReal, double maxReal, int imageHeight) {
  return static_cast<int>((real - minReal) * (imageHeight / (maxReal - minReal)));
}

int Buddhabrot::getColumnFromImaginary(double imaginary, double minImaginary, double maxImaginary, int imageWidth) {
  return static_cast<int>((imaginary - minImaginary) * (imageWidth / (maxImaginary - minImaginary)));
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

void Buddhabrot::generateHeatmap(UINT** heatmap, int imageWidth, int imageHeight,
                                 const ComplexNumber& min, const ComplexNumber& max,
                                 int iterations, ULLI samples, UINT& maxHeatmapValue,
                                 ULLI& iterationCount, ULLI totalIterations) {
  mt19937 rng(static_cast<UINT>(high_resolution_clock::now().time_since_epoch().count()));
  uniform_real_distribution<double> rDist(min.real(), max.real());
  uniform_real_distribution<double> iDist(min.imaginary(), max.imaginary());

  auto nextReport = high_resolution_clock::now() + seconds(5);

  for (ULLI i = 0; i < samples; ++i) {
    if (high_resolution_clock::now() > nextReport) {
      cout << "Progress: " << (static_cast<double>(iterationCount) / totalIterations) * 100 << "%\n";
      nextReport = high_resolution_clock::now() + seconds(30);
    }
    
    ComplexNumber sample(rDist(rng), iDist(rng));
    vector<ComplexNumber> points = Buddhabrot::points(sample, iterations);
    
    for (const ComplexNumber& point : points) {
      if (point.real() >= min.real() && point.real() <= max.real() && point.imaginary() >= min.imaginary() && point.imaginary() <= max.imaginary()) {
        int row = getRowFromReal(point.real(), min.real(), max.real(), imageHeight);
        int col = getColumnFromImaginary(point.imaginary(), min.imaginary(), max.imaginary(), imageWidth);
        
        if (++heatmap[row][col] > maxHeatmapValue) {
          maxHeatmapValue = heatmap[row][col];
        }
      }
    }
    ++iterationCount;
  }
}
