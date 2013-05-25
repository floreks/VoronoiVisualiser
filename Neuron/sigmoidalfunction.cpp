#include "sigmoidalfunction.h"

SigmoidalFunction::SigmoidalFunction(double beta) : beta(beta)
{}

double SigmoidalFunction::equasion(double x)
{
    return (1/(1 + exp(-beta*x)));
}

double SigmoidalFunction::equasionDerivative(double x)
{
    return (equasion(x) * (1 - equasion(x)));
}
