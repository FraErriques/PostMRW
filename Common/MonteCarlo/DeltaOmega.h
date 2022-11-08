#ifndef DELTAOMEGA_H_INCLUDED
#define DELTAOMEGA_H_INCLUDED

 #include <cmath>

namespace Common{
namespace MonteCarlo{


//   class ClassicalDiscreteGenerator; // FFWD not necessary
//   class ClassicalContinuousGenerator;


class DeltaOmega
{
    // DeltaOmega is the DeltaAscissa in a Riemann partition of the Omega Event-space.
    // an instance of DeltaOmega is a single step in the Riemann partition.
    // The friend function "buildOmega" builds a vector, which contains the whole population of
    // DeltaOmega, which constitutes the whole Omega event space.

private:
    friend class ClassicalDiscreteGenerator;//these two friendship are necessary for friends to access DeltaOmega's members.
    friend class ClassicalContinuousGenerator;// each friend class has a vector<DeltaOmega> and needs to access its fields.
    double mediana;
    double eta;
    double categoryFrequency;
    int DeltaOmegaCardinality;

public:
    DeltaOmega( double medianaPoint, double etaMeasure)
    {// Ctor
        this->mediana = medianaPoint;
        this->eta = etaMeasure;
        this->categoryFrequency = 0.0;// init and then add on.
        this->DeltaOmegaCardinality = 0;// init and then add on.
    }// Ctor
    double getMediana(){return this->mediana;}

    bool belongsToDeltaOmega( double candidate)
    {//evaluates whether an event belongs to a histogram.
        bool res = fabs( candidate-this->mediana)<=this->eta;
        // ready
        return res;
    }// bool belongsToDeltaOmega( double candidate)

};// class DeltaOmega


} // namespace Common
} // namespace MonteCarlo

#endif // DELTAOMEGA_H_INCLUDED
