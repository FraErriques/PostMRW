#ifndef CLASSICALCONTINUOUSGENERATOR_H_INCLUDED
#define CLASSICALCONTINUOUSGENERATOR_H_INCLUDED




/*
    If seed is set to 1, the generator is reinitialized to its initial value and produces the same values as
    before any call to rand or srand.
    Two different initializations with the same seed will generate the same succession of results
    in subsequent calls to rand.
*/


#include <stdio.h>      /* NULL */
#include <stdlib.h>     /* RAND_MAX , srand , rand */
#include <time.h>       /* time */
//#include "MonteCarlo.h"
//  #include "../StringBuilder/StringBuilder.h"
#include "./DeltaOmega.h"
#include <cmath>
#include <iostream>
#include <vector>


namespace Common
{

namespace MonteCarlo
{





class ClassicalContinuousGenerator
{
private:
    unsigned int currentSeed;
    const int generatorSUP; // = RAND_MAX;// #include <stdlib.h>
    friend class DeltaOmega;
    bool ConstructorCanContinue;
    std::string * ReasonForAbortingConstructor;
    double theIntervalMeasure;
    double Min, Sup;
    double omothetia;// it's rational since it contains a ratio, for proportion with default interval.
    double translation;
    std::vector<double> * continuousPopulation;
    std::vector<DeltaOmega*> * frequencyDistribution;

public:
    ClassicalContinuousGenerator( unsigned int seed, double left, double right);// : currentSeed(seed), generatorSUP(RAND_MAX);
    // You can pass in a pointer to a time_t object that time will fill up with the current time (and the return value is the same
    // one that you pointed to). If you pass in NULL, it just ignores it and merely returns a new time_t object that represents the current time.
    ClassicalContinuousGenerator( double left, double right);// : currentSeed( time(NULL)), generatorSUP(RAND_MAX);
    ~ClassicalContinuousGenerator();// Dtor
    void resetExtractionInterval( double left, double right );// non const; resets instance members.
    double nextDoubleInInterval() const;
    void showContinuousPopulation() const;
    void showFrequencyDistribution() const;
    void showCumulatedFrequency() const;
    void buildContinuousFrequencyDistribution();
//void buildOmega( int partizioneLeft ,int partizioneRight);
    void buildOmega( double partizioneLeft, double partizioneRight);
    unsigned int showCurrentSeed() const;
    double showSup() const;

}; // class ClassicalContinuousGenerator



} // namespace Common

} // namespace MonteCarlo


#endif // CLASSICALCONTINUOUSGENERATOR_H_INCLUDED
