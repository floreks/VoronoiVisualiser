#include "Centroids.h"

default_random_engine Centroids::engine;

CPoint Centroids::getCentroid(BYTE dimensions, double min, double max) {

    QVector<double> result;

    engine.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> dis(min,max);

    for(BYTE i=0;i<dimensions;i++) {
        result.push_back(dis(engine));
    }

    return CPoint(result);
}

KPoint Centroids::getPoint(BYTE dimensions, double min, double max) {

    QVector<double> result;

    engine.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> dis(min,max);

    for(BYTE i=0;i<dimensions;i++) {
        result.push_back(dis(engine));
    }

    return KPoint(result);
}

QVector<CPoint> Centroids::randomizeCentroids(WORD quantity, BYTE dimensions, double min, double max) {

    QVector<CPoint> result;

    for(WORD i=0;i<quantity;i++) {
        result.push_back(getCentroid(dimensions,min,max));
    }

    return result;
}

QVector<KPoint> Centroids::randomizePoints(WORD quantity, BYTE dimensions, double min, double max) {
    QVector<KPoint> result;

    for(WORD i=0;i<quantity;i++) {
        result.push_back(getPoint(dimensions,min,max));
    }

    return result;
}

double Centroids::countDistance(CPoint &i, KPoint &j) {
    double result = 0;

    for(int k=0;k<i.getDimensions();k++) {
        result += (i.getParams()[k] - j.getParams()[k]) * (i.getParams()[k] - j.getParams()[k]);
    }

    return sqrt(result);
}
