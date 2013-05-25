#include "neuron.h"

double Neuron::alpha = 0.6;

Neuron::Neuron(int size)
{
    signal.resize(size);
    weight.resize(size);
    lastWeight.resize(size);
    bias = 1.0;
    delta = 0;
    momentum = 0.0;
    biasEnabled = false;
}

Neuron::Neuron(int size, AbstractFunction *function)
{
    signal.resize(size);
    weight.resize(size);
    lastWeight.resize(size);
    this->activateFunction = function;
    bias = 1.0;
    delta = 0;
    momentum = 0.0;
    biasEnabled = false;
}

Neuron::~Neuron(){}

void Neuron::setNeuronSize(int size)
{
    //qDebug() << "Setting neuron size to: " << size;
    signal.resize(size);
    weight.resize(size);
}

double Neuron::process()
{
    double answer = 0;
    if(biasEnabled)
        answer += bias;

    for(unsigned int i=0;i<signal.size();i++)
        answer += signal[i] * weight[i];

    return activateFunction->equasion(answer);
}

void Neuron::countError(double delta)
{
    this->delta = delta * activateFunction->equasionDerivative(process());
    //qDebug() << "Delta: " << this->delta;
    deltaWeight = alpha * this->delta;
    //qDebug() << "Delta weight: " << deltaWeight;

    if(biasEnabled)
        bias += deltaWeight;
}

void Neuron::updateWeights()
{
    weightCopy = weight;
    for(unsigned int i=0;i<weight.size();i++)
    {
        weight[i] += deltaWeight * signal[i] + momentum*(weight[i]-lastWeight[i]);
        //qDebug() << "Update weight: " << weight[i];
    }
    lastWeight = weightCopy;
}

void Neuron::countLLError(int expectedOutput)
{
    delta = (expectedOutput - process()) * activateFunction->equasionDerivative(process());
    //qDebug() << "Delta: " << delta;
    deltaWeight = alpha * delta;
    //qDebug() << "Delta weight: " << deltaWeight;

    if(biasEnabled)
        bias += deltaWeight;
}

void Neuron::setActivationFunction(AbstractFunction *function)
{
    //qDebug() << "Setting activate function...";
    this->activateFunction = function;
}

void Neuron::setSignal(vector<double> signal)
{
    this->signal = signal;
    //qDebug() << "Setting signals...";
    //for(double &i : this->signal) qDebug() << "Signal: " << i;
}

void Neuron::setWeight(vector<double> weight)
{
    this->weight = weight;
    //qDebug() << "Setting weights...";
    //for(double &i : this->weight) qDebug() << "Weight: " << i;
}

void Neuron::initWeight(double min, double max)
{
    //qDebug() << "Initializing weights...";
    engine.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> dis(min,max);
    for(double &i : weight)
    {
        i = dis(engine);
        //qDebug() << "Weight: " << i;
    }
}

void Neuron::initSignal(double min, double max)
{
    //qDebug() << "Initializing signals...";
    engine.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> dis(min,max);
    for(double &i : signal)
    {
        i = dis(engine);
        //qDebug() << "Signal: " << i;
    }
}
