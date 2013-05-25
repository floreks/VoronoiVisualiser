#include "kpoint.h"

KPoint::KPoint(QVector<double> params) : params(params) {
    groupNr = -1;
}

KPoint::KPoint() {
    groupNr = -1;
}

void KPoint::setGroup(WORD groupNr) {
    this->groupNr = groupNr;
}

void KPoint::setParams(QVector<double> params) {
    this->params = params;
}
