#ifndef KOHONEN_H
#define KOHONEN_H

#include <Model/cpoint.h>
#include <Model/kpoint.h>

class Kohonen
{
private:
    QVector<KPoint> inputPoints, outputPoints;
    QVector<CPoint> inCentroids, outCentroids;
    bool end;
    double quantizationError;
    double alpha;

    double countDistance(KPoint &i, CPoint &j);
    void updateCentroids();
public:
    Kohonen(QVector<KPoint> &inputData);
    Kohonen(QVector<KPoint> &inputData, QVector<CPoint> &centroids);
    Kohonen();

    void normalizePoints(double size);
    void setAlpha(double alpha) { this->alpha = alpha; }
    void setInputData(QVector<KPoint> &inputData);
    void setCentroids(QVector<CPoint> &centroids);

    void randomizeCentroids(WORD quantity, BYTE dimensions, double min, double max);

    void start();
    bool update();

    QVector<CPoint> getCentroids()const { return outCentroids; }
    QVector<KPoint> getPoints()const { return outputPoints; }
    double getQuantizationError()const { return quantizationError; }
};

#endif // KOHONEN_H
