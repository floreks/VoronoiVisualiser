#ifndef NEURALGAS_H
#define NEURALGAS_H

#include <Model/cpoint.h>
#include <Model/kpoint.h>

#include <QObject>

class NeuralGas
{
private:
    QVector<KPoint> inputPoints, outputPoints;
    QVector<CPoint> inCentroids, outCentroids;
    bool end;
    double quantizationError;
    double alphaMin, alphaMax, lambdaMin, lambdaMax;

    double countDistance(KPoint &i, CPoint &j);
public:
    NeuralGas(QVector<KPoint> &inputData);
    NeuralGas(QVector<KPoint> &inputData, QVector<CPoint> &centroids);
    NeuralGas();
    NeuralGas(double alphaMin, double alphaMax, double lambdaMin);

    void normalizePoints(double size);
    void setAlpha(double alpha) { this->alphaMax = alpha; }
    void setInputData(QVector<KPoint> &inputData);
    void setCentroids(QVector<CPoint> &centroids);

    void randomizeCentroids(WORD quantity, BYTE dimensions, double min, double max);

    void start();
    bool update(QWidget *parent);

    QVector<CPoint> getCentroids()const { return outCentroids; }
    QVector<KPoint> getPoints()const { return outputPoints; }
    double getQuantizationError()const { return quantizationError; }
};

#endif // NEURALGAS_H
