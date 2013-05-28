#ifndef CPOINT_H
#define CPOINT_H

#include "Defs.h"
#include <QVector>

class CPoint
{
private:
    QVector<double> params;
    WORD groupNr;
    double alpha;
    double distance;
public:
    CPoint(QVector<double> params);
    CPoint();

    void setParams(QVector<double> params);
    void setGroup(WORD groupNr);
    void setAlpha(double alpha) { this->alpha = alpha; }
    void setDistance(double dist) { this->distance = dist; }

    double &paramAt(WORD index) { return params[index]; }
    QVector<double> getParams()const {return params;}
    WORD getDimensions()const { return params.size(); }
    WORD getGroup()const { return groupNr; }
    double getAlpha()const { return alpha; }
    double getDistance()const { return distance; }

    double operator-(CPoint j);
};

#endif // CPOINT_H
