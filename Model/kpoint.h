#ifndef KPOINT_H
#define KPOINT_H

#include "Defs.h"
#include <QVector>

class KPoint
{
private:
    QVector<double> params;
    WORD groupNr;
public:
    KPoint(QVector<double> params);
    KPoint();

    double &paramAt(WORD index) { return params[index]; }
    void setParams(QVector<double> params);
    void setGroup(WORD groupNr);

    QVector<double> getParams()const {return params;}
    WORD getDimensions()const { return params.size(); }
    WORD getGroup()const { return groupNr; }
};

#endif // KPOINT_H
