#ifndef ABSTRACTFUNCTION_H
#define ABSTRACTFUNCTION_H

class AbstractFunction
{
public:
    virtual double equasion(double x) = 0;
    virtual double equasionDerivative(double x) = 0;
    virtual ~AbstractFunction() = 0;
};

#endif // ABSTRACTFUNCTION_H
