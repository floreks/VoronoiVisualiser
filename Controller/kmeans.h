#ifndef KMEANS_H
#define KMEANS_H

#include <Model/cpoint.h>
#include <Model/kpoint.h>

class KMeans
{
private:
    QVector<KPoint> inputPoints, outputPoints;
    QVector<CPoint> inCentroids, outCentroids;
    bool end;
    double quantizationError;

    double countDistance(KPoint &i, CPoint &j);
    void setGroup(KPoint &point, QVector<CPoint> &centroids);
    void updateCentroids();
public:
    KMeans(QVector<KPoint> &inputData);
    KMeans(QVector<KPoint> &inputData, QVector<CPoint> &centroids);
    KMeans();

    void setInputData(QVector<KPoint> &inputData);
    void setCentroids(QVector<CPoint> &centroids);

    void randomizeCentroids(WORD quantity, BYTE dimensions, double min, double max);

    void start();
    bool update();

    QVector<CPoint> getCentroids()const { return outCentroids; }
    QVector<KPoint> getPoints()const { return outputPoints; }
    double getQuantizationError()const { return quantizationError; }

};

#endif // KMEANS_H
