#ifndef CENTROIDS_H
#define CENTROIDS_H

#include <Model/cpoint.h>
#include <Model/kpoint.h>
#include <chrono>

using namespace std;

class Centroids {
private:
    static default_random_engine engine;
    static CPoint getCentroid(BYTE dimensions, double min, double max);
    static KPoint getPoint(BYTE dimensions, double min, double max);
public:
    static QVector<CPoint> randomizeCentroids(WORD quantity, BYTE dimensions, double min, double max);
    static QVector<KPoint> randomizePoints(WORD quantity, BYTE dimensions, double min, double max);
};

#endif
