#ifndef DELTAOMEGA_H_INCLUDED
#define DELTAOMEGA_H_INCLUDED

namespace Common
{

namespace MonteCarlo
{

//
//   class ClassicalDiscreteGenerator;
//   class ClassicalContinuousGenerator;

// DeltaOmega is the DeltaAscissa in a Riemann partition of the Omega Event-space.
class DeltaOmega
{// an instance of DeltaOmega is a single step in the Riemann partition. The friend function "buildOmega" builds a vector, which contains the whole population of
 // DeltaOmega, which constitutes the whole Omega event space.
private:
    friend class ClassicalDiscreteGenerator;
    friend class ClassicalContinuousGenerator;
    double mediana;
    double eta;
    double categoryFrequency;
    int DeltaOmegaCardinality;
public:
    DeltaOmega( double medianaPoint, double etaMeasure)
    {
        this->mediana = medianaPoint;
        this->eta = etaMeasure;
        this->categoryFrequency = 0.0;// init and then add on.
        this->DeltaOmegaCardinality = 0;// init and then add on.
    }// Ctor
    double getMediana(){return this->mediana;}

    bool belongsToDeltaOmega( double candidate)
    {
        bool res = fabs( candidate-this->mediana)<=this->eta;
        // ready
        return res;
    }// bool belongsToDeltaOmega( double candidate)

};// class DeltaOmega


} // namespace Common

} // namespace MonteCarlo

#endif // DELTAOMEGA_H_INCLUDED
