#ifndef CPOINT_H
#define CPOINT_H

#include "Defs.h"
#include <QVector>

class CPoint
{
private:
    QVector<double> params;
    WORD groupNr;
public:
    CPoint(QVector<double> params);
    CPoint();

    void setParams(QVector<double> params);
    void setGroup(WORD groupNr);

    QVector<double> getParams()const {return params;}
    WORD getDimensions()const { return params.size(); }
    WORD getGroup()const { return groupNr; }

    double operator-(CPoint j);
};

#endif // CPOINT_H
