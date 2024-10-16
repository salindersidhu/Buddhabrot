#ifndef BUDDHABROT_HPP
#define BUDDHABROT_HPP

#include <mutex>
#include <atomic>
#include <string>
#include <random>
#include <chrono>
#include <vector>
#include <thread>
#include <fstream>
#include <iostream>
#include "ComplexNumber.hpp"

using std::mutex;
using std::atomic;
using std::string;
using std::vector;
using std::thread;
using std::lock_guard;
using std::ofstream;
using std::mt19937;
using std::lock_guard;
using std::uniform_real_distribution;
using std::cout;
using std::endl;
using namespace std::chrono;

typedef unsigned int UINT;
typedef unsigned long long int ULLI;

class Buddhabrot {
public:
  Buddhabrot(int width, int height, int samples, double minR, double maxR,
             double minI, double maxI, int redIterations, int greenIterations,
             int blueIterations, const string& filename, bool verbose = false);
  ~Buddhabrot();

  int generate();
private:
  void allocHeatmap(UINT**& heatmap);
  void freeHeatmap(UINT**& heatmap);
  void generateHeatmap(UINT** heatmap, int iterations);
  void scaleHeatmapsToColor();
  void reportProgress();

  int flushToPPM();
  int getColourFromHeatmap(UINT heatmapValue);
  int getRowFromReal(double real);
  int getColumnFromImaginary(double imaginary);

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

  bool verbose;
  string filename;

  ULLI totalIterations;
  atomic<ULLI> iterationCount;
};

#endif // BUDDHABROT_HPP
