#include "kohonen.h"

#include "Controller/Centroids.h"

Kohonen::Kohonen() : end(false) {
}

Kohonen::Kohonen(QVector<KPoint> &inputData, QVector<CPoint> &centroids) : end(false), inputPoints(inputData), inCentroids(centroids) {

}

Kohonen::Kohonen(QVector<KPoint> &inputData) : end(false), inputPoints(inputData) {

}

void Kohonen::setCentroids(QVector<CPoint> &centroids) {
    this->inCentroids = centroids;
}

void Kohonen::setInputData(QVector<KPoint> &inputData) {
    this->inputPoints = inputData;
}

void Kohonen::randomizeCentroids(WORD quantity, BYTE dimensions, double min, double max) {
    this->inCentroids = Centroids::randomizeCentroids(quantity,dimensions,min,max);
}

void Kohonen::start() {

}

bool Kohonen::update() {

}

void Kohonen::normalizePoints(double size) {

    // getting min and max
    double min=0,max=0;

    for(int i=0;i<inputPoints.size();i++) {
        for(int j=0;j<inputPoints[i].getParams().size();j++) {
            if(min > inputPoints[i].getParams()[j])
                min = inputPoints[i].getParams()[j];
            if(max < inputPoints[i].getParams()[j])
                max = inputPoints[i].getParams()[j];
        }
    }

    double multiplier = size/(max+abs(min));

    for(int i=0;i<inputPoints.size();i++) {
        for(int j=0;j<inputPoints[i].getParams().size();j++) {
            inputPoints[i].getParams()[j] += abs(min);
            inputPoints[i].getParams()[j] *= multiplier;
        }
    }
}

// ============= PRIVATE =============== //

double Kohonen::countDistance(KPoint &i, CPoint &j) {
    double result = 0;

    for(int k=0;k<i.getDimensions();k++) {
        result += (i.getParams()[k] - j.getParams()[k]) * (i.getParams()[k] - j.getParams()[k]);
    }

    return result;
}

void Kohonen::updateCentroids() {

}
