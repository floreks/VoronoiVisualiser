#ifndef KOHONEN_H
#define KOHONEN_H

#include <Model/cpoint.h>
#include <Model/kpoint.h>

#include <QObject>

class Kohonen
{
private:
    QVector<KPoint> inputPoints, outputPoints;
    QVector<CPoint> inCentroids, outCentroids;
    bool end;
    double quantizationError;
    double alpha,lambda;

    double countDistance(KPoint &i, CPoint &j);
    double countDistance(CPoint &i, CPoint &j);
public:
    Kohonen(QVector<KPoint> &inputData);
    Kohonen(QVector<KPoint> &inputData, QVector<CPoint> &centroids);
    Kohonen();
    Kohonen(double alpha, double lambda);

    void normalizePoints(double size);
    void setAlpha(double alpha) { this->alpha = alpha; }
    void setInputData(QVector<KPoint> &inputData);
    void setCentroids(QVector<CPoint> &centroids);

    void randomizeCentroids(WORD quantity, BYTE dimensions, double min, double max);

    void start();
    bool update(QWidget *parent);

    QVector<CPoint> getCentroids()const { return outCentroids; }
    QVector<KPoint> getPoints()const { return outputPoints; }
    double getQuantizationError()const { return quantizationError; }
};

#endif // KOHONEN_H
