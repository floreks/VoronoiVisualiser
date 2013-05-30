#include "kohonen.h"

#include "Controller/Centroids.h"
#include <QDebug>
#include <QProgressDialog>

class SortByDistance {
public:
    bool operator() (CPoint i, CPoint j) {
        return i.getDistance() < j.getDistance();
    }
};

Kohonen::Kohonen() {
    end = false;
}

Kohonen::Kohonen(QVector<KPoint> &inputData, QVector<CPoint> &centroids) : inputPoints(inputData), inCentroids(centroids) {
    end = false;
}

Kohonen::Kohonen(QVector<KPoint> &inputData) : inputPoints(inputData) {
    end = false;
}

Kohonen::Kohonen(double alpha, double lambda) {
    end = false;
    this->alpha = alpha;
    this->lambda = lambda;
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
    outCentroids = inCentroids;
    outputPoints = inputPoints;
    end = false;
    quantizationError = 0;
}

bool Kohonen::update(QWidget *parent) {

    QProgressDialog progress(parent);
    progress.setLabelText("Teaching pattern...");
    progress.setMinimum(0);
    progress.setWindowTitle("Self-organizing maps");
    progress.setMaximum(10*outputPoints.size());
    progress.setValue(50);
    progress.setCancelButtonText("Cancel");
    progress.setModal(true);
    progress.show();

    int counter = 0;

    for(int i=0; i<10; i++) {
        quantizationError = 0;
        for(int w=0; w<outputPoints.size(); w++) {
            progress.setValue(counter++);
            if(progress.wasCanceled())
                return false;

            // sortowanie
            for(int d=0;d<outCentroids.size();d++)
                outCentroids[d].setDistance(countDistance(outputPoints[w],outCentroids[d]));

            qSort(outCentroids.begin(),outCentroids.end(),SortByDistance());
            quantizationError += outCentroids[0].getDistance();

            for(int k=0;k<outCentroids[0].getDimensions();k++)
                outCentroids[0].paramAt(k) += alpha * exp(-(outCentroids[0].getDistance()*outCentroids[0].getDistance()) /
                        2.0*lambda*lambda) *
                        (outputPoints[w].paramAt(k)-outCentroids[0].paramAt(k));

            // sasiedztwo
            for(int j=1; j<outCentroids.size();j++) {
                double dist = countDistance(outCentroids[j],outCentroids[0]);
                for(int k=0;k<outCentroids[0].getDimensions();k++)
                    outCentroids[j].paramAt(k) = outCentroids[j].paramAt(k) + (alpha *
                       exp(-(dist*dist)/(2.0*lambda*lambda)) * (outputPoints[w].paramAt(k) - outCentroids[j].paramAt(k)));
            }

        }
        qDebug() << "Kohonen iteration: " << i;
        quantizationError /= outputPoints.size();
    }
    return false;
}

void Kohonen::normalizePoints(double size) {

    // getting min and max
    double min=0,max=0;

    for(int i=0;i<inputPoints.size();i++) {
        for(int j=0;j<inputPoints[i].getParams().size();j++) {
            if(min > inputPoints[i].paramAt(j))
                min = inputPoints[i].paramAt(j);
            if(max < inputPoints[i].paramAt(j))
                max = inputPoints[i].paramAt(j);
        }
    }

    double multiplier = size/(max+abs(min));

    for(int i=0;i<inputPoints.size();i++) {
        for(int j=0;j<inputPoints[i].getParams().size();j++) {
            inputPoints[i].paramAt(j) += abs(min);
            inputPoints[i].paramAt(j) *= multiplier;
        }
    }
}

// ============= PRIVATE =============== //

double Kohonen::countDistance(KPoint &i, CPoint &j) {
    double result = 0;

    for(int k=0;k<i.getDimensions();k++) {
        result += (i.getParams()[k] - j.getParams()[k]) * (i.getParams()[k] - j.getParams()[k]);
    }

    return sqrt(result);
}

double Kohonen::countDistance(CPoint &i, CPoint &j) {
    double result = 0;

    for(int k=0;k<i.getDimensions();k++) {
        result += (i.getParams()[k] - j.getParams()[k]) * (i.getParams()[k] - j.getParams()[k]);
    }

    return sqrt(result);
}
