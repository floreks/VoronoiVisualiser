#include "cpoint.h"

CPoint::CPoint(QVector<double> params) : params(params) {
    groupNr = -1;
}

CPoint::CPoint() {
    groupNr = -1;
}

void CPoint::setGroup(WORD groupNr) {
    this->groupNr = groupNr;
}

void CPoint::setParams(QVector<double> params) {
    this->params = params;
}

double CPoint::operator -(CPoint j) {
    double result = 0;
    for(int k=0;k<j.getParams().size();k++)
        result += params[k] - j.getParams()[k];

    return result;
}
