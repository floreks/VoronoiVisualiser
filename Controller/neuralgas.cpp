#include "neuralgas.h"
#include "Controller/Centroids.h"

#include <QDebug>
#include <QProgressDialog>

class SortByDistance {
public:
    bool operator() (CPoint i, CPoint j) {
        return i.getDistance() < j.getDistance();
    }
};

NeuralGas::NeuralGas() {
    end = false;
}

NeuralGas::NeuralGas(double alphaMin, double alphaMax, double lambdaMin) {
    end = false;
    this->alphaMin = alphaMin;
    this->alphaMax = alphaMax;
    this->lambdaMin = lambdaMin;
}

NeuralGas::NeuralGas(QVector<KPoint> &inputData, QVector<CPoint> &centroids) : inputPoints(inputData), inCentroids(centroids) {
    end = false;
}

NeuralGas::NeuralGas(QVector<KPoint> &inputData) : inputPoints(inputData) {
    end = false;
}

void NeuralGas::setCentroids(QVector<CPoint> &centroids) {
    this->inCentroids = centroids;
}

void NeuralGas::setInputData(QVector<KPoint> &inputData) {
    this->inputPoints = inputData;
}

void NeuralGas::randomizeCentroids(WORD quantity, BYTE dimensions, double min, double max) {
    this->inCentroids = Centroids::randomizeCentroids(quantity,dimensions,min,max);
}

void NeuralGas::start() {

    outCentroids = inCentroids;
    outputPoints = inputPoints;
    end = false;
    quantizationError = 0;

    lambdaMax = outCentroids.size()/2;

    // setting alpha for neurons
    for(CPoint &i : outCentroids)
        i.setAlpha(alphaMax);
}

bool NeuralGas::update(QWidget *parent) {

    double lambda;
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

    for(int i=1;i<=10;i++) {

        quantizationError = 0;
        lambda = lambdaMax * pow((lambdaMin/lambdaMax),i/10.0); //!!!!!!!!!!!!!!!!!!//
        for(int w=0; w<outputPoints.size(); w++) {
            progress.setValue(counter++);
            if(progress.wasCanceled())
                return false;

            for(int d=0;d<outCentroids.size();d++)
                outCentroids[d].setDistance(countDistance(outputPoints[w],outCentroids[d]));
            qSort(outCentroids.begin(),outCentroids.end(),SortByDistance());
            quantizationError += outCentroids[0].getDistance();
            for(int j=0; j<outCentroids.size(); j++) {
                outCentroids[j].setAlpha(alphaMax * pow((alphaMin/alphaMax),i/10.0)); //TMAX//
                for(int k=0;k<outCentroids[j].getDimensions();k++) {
                    outCentroids[j].paramAt(k) += outCentroids[j].getAlpha() * exp(-j/lambda) *
                            (outputPoints[w].paramAt(k) - outCentroids[j].paramAt(k));
                }
            }
        }
        qDebug() << "Iteration: " << i;
        quantizationError /= outputPoints.size();
    }

    progress.hide();

    return true;
}

void NeuralGas::normalizePoints(double size) {

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

double NeuralGas::countDistance(KPoint &i, CPoint &j) {
    double result = 0;

    for(int k=0;k<i.getDimensions();k++) {
        result += (i.getParams()[k] - j.getParams()[k]) * (i.getParams()[k] - j.getParams()[k]);
    }

    return sqrt(result);
}
