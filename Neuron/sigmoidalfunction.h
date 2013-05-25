#ifndef SIGMOIDALFUNCTION_H
#define SIGMOIDALFUNCTION_H

#include "abstractfunction.h"
#include <cmath>

class SigmoidalFunction : public AbstractFunction
{
private:
    double beta;
public:
    SigmoidalFunction(double beta);
    double equasion(double x);
    double equasionDerivative(double x);
};

#endif // SIGMOIDALFUNCTION_H
