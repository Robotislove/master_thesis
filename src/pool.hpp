#ifndef __OPTI_POOL__
#define __OPTI_POOL__

#include <memory>
#include <opencv2/opencv.hpp>
#include <experimental/random>
#include <cstdio>
#include <iostream>

using namespace cv;
using namespace std;

namespace opti_ga {
  struct genome
  {
    shared_ptr<cv::Mat> map;
    vector<Point> waypoints;
    float fitness = 0;
  };


  //Helper functions:
  //--------------------------------------------------
  Point randomPointShift(Point p, int magnitude = 200);
  vector<Point> genWaypoints(int width, int height, Point start, int n = 10);
  void printFitness(vector<genome> &gens);
  void printWaypoints(vector<Point>& waypoints);

  template<typename T>
  vector<T> slice(vector<T> vec, int start_idx, int stop_idx);

  template<typename T>
  vector<T> sliceErase(vector<T>& vec, int start_idx, int stop_idx);

  template<typename T>
  void joinSlices(vector<T>& vecA, vector<T>& vecB);
  bool compareFitness(const struct genome &genA, const struct genome &genB);
  void markOcc( Mat &img, Point &start, Point &end, int val = 255, int size=10);
  void markPath(genome &gen);
  // --------------------------------------------------


  class GenPool{
    // Generate population
    // Perform crossover
    // perform mutation
    // perform selection

  public:
    GenPool(int width, int height, Point start, Point end):width(width), height(height), start(start), end(end) {}
    void populatePool(int size, int waypoints);

    // Returns currently best fitness
    float update(int iterations = 100);

    // private:
    vector<genome> gens;
    const int width;
    const int height;
    const Point start;
    const Point end;

    void crossover();
    void mutation();
    void selection();
    float calFittness(struct genome &gen);
    float calOcc(struct genome &gen);
    float calTime(struct genome &gen, int speed = 3);
    struct genome getBest();
    void conditionalPointShift(Point &p, int magnitude = 30);
    void randomInsert(struct genome &gen);
    void randomRemove(struct genome &gen);
  };
}

#endif //__OPTI_POOL__
