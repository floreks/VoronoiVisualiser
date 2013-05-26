#include "kmeans.h"

#include "Controller/Centroids.h"
#include <QDebug>

QDebug deb = qDebug();

KMeans::KMeans(QVector<KPoint> &inputData) : inputPoints(inputData), end(false) {
}

KMeans::KMeans(QVector<KPoint> &inputData, QVector<CPoint> &centroids) :
    inputPoints(inputData), inCentroids(centroids), end(false) {
}

KMeans::KMeans() : end(false) {
}

void KMeans::setCentroids(QVector<CPoint> &centroids) {
    this->inCentroids = centroids;
}

void KMeans::setInputData(QVector<KPoint> &inputData) {
    this->inputPoints = inputData;
}

void KMeans::randomizeCentroids(WORD quantity, BYTE dimensions, double min, double max) {
    this->inCentroids = Centroids::randomizeCentroids(quantity,dimensions,min,max);
}

void KMeans::start() {
    outCentroids = inCentroids;
    outputPoints = inputPoints;
    end = false;

    //setting centroids groups //
    for(int i=1;i<=outCentroids.size();i++)
        outCentroids[i-1].setGroup(i);

    quantizationError = 0;
    for(KPoint &kpoint : outputPoints) {
        setGroup(kpoint,outCentroids);
        quantizationError += countDistance(kpoint,outCentroids[kpoint.getGroup()-1]);
    }

    quantizationError /= outputPoints.size();
}

bool KMeans::update() {

    if(end)
        return false;

    inCentroids = outCentroids; // remember last positions;

    updateCentroids();

    quantizationError = 0;
    for(KPoint &kpoint : outputPoints) {
        setGroup(kpoint,outCentroids);
        quantizationError += countDistance(kpoint,outCentroids[kpoint.getGroup()-1]);
    }

    quantizationError /= outputPoints.size();

    // check if centroids moved

    double error = 0;

    for(int i=0;i<outCentroids.size();i++)
        error += abs(inCentroids[i]-outCentroids[i]);

    if(error > 0.001) {
        return true;
    }

    end = true;
    return false;
}

// ============= PRIVATE =============== //

double KMeans::countDistance(KPoint &i, CPoint &j) {
    double result = 0;

    for(int k=0;k<i.getDimensions();k++) {
        result += (i.getParams()[k] - j.getParams()[k]) * (i.getParams()[k] - j.getParams()[k]);
    }

    return result;
}

void KMeans::setGroup(KPoint &point, QVector<CPoint> &centroids) {

    CPoint winner = centroids[0];
    double dist = countDistance(point,centroids[0]);

    for(WORD i=1;i<centroids.size();i++) {
        if(dist > countDistance(point,centroids[i])) {
            dist = countDistance(point,centroids[i]);
            winner = centroids[i];
        }
    }

    point.setGroup(winner.getGroup());
}

void KMeans::updateCentroids() {

    WORD inGroup = 0;
    QVector<double> paramsAvg(outputPoints[0].getParams().size());

    for(int i=0;i<outCentroids.size();i++) {
        for(int j=0;j<outputPoints.size();j++) {
            if(outputPoints[j].getGroup() == outCentroids[i].getGroup()) {
                for(int k=0;k<outputPoints[j].getParams().size();k++) {
                    paramsAvg[k] += outputPoints[j].getParams()[k];
                }
                inGroup++;
            }
        }

        if(inGroup > 0) {
            for(double &k : paramsAvg)
                k /= inGroup;
            outCentroids[i].setParams(paramsAvg);
        }

        inGroup = 0;
        paramsAvg.clear();
        paramsAvg.resize(outputPoints[0].getParams().size());
    }
}
