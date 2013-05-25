#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include "abstractfunction.h"
#include <random>
#include <chrono>

using std::vector;

class Neuron
{
private:
    vector<double>signal;
    vector<double>weight;
    vector<double>lastWeight;
    vector<double>weightCopy;
    AbstractFunction *activateFunction;
    std::default_random_engine engine;
    double delta;
    double deltaWeight;
    double bias;
    double momentum;
    bool biasEnabled;
    static double alpha;
public:
    Neuron(){}
    Neuron(int size);
    Neuron(int size, AbstractFunction *function);
    ~Neuron();

    double process();
    void countError(double delta);
    void countLLError(int expectedOutput); // last layer - LL error
    void updateWeights();

    // Setters & getters //
    double getDelta()const{return delta;}
    int getNeuronSize()const{return signal.size();}
    AbstractFunction *getActivationFunction()const{return activateFunction;}
    vector<double> &getWeight(){return weight;}
    vector<double> &getSignal(){return signal;}
    double getAlpha()const{return alpha;}

    void setNeuronSize(int size);
    void setActivationFunction(AbstractFunction *function);
    void setSignal(vector<double>signal);
    void setWeight(vector<double>weight);
    void setAlpha(double alpha){this->alpha = alpha;}
    void setDelta(double delta){this->delta = delta;}
    void initWeight(double min, double max);
    void initSignal(double min, double max);
    void enableBias(){biasEnabled = true; bias = 1.0;}
    void disableBias(){biasEnabled = false; bias = 0.0;}
    void setMomentum(double momentum){ this->momentum = momentum; }
};

#endif // NEURON_H
