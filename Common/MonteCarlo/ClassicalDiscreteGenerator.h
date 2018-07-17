#ifndef CLASSICALDISCRETEGENERATOR_H_INCLUDED
#define CLASSICALDISCRETEGENERATOR_H_INCLUDED




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
#include "../StringBuilder/StringBuilder.h"
#include "./DeltaOmega.h"
#include <cmath>
#include <iostream>


namespace Common
{

namespace MonteCarlo
{


class ClassicalDiscreteGenerator
{
private:
    unsigned int currentSeed;
    const int generatorSUP; // = RAND_MAX;// #include <stdlib.h>
    friend class DeltaOmega;
    bool ConstructorCanContinue;
    std::string * ReasonForAbortingConstructor;
    int theIntervalMeasure;
    int Min, Sup;
    double omothetia;// it's rational since it contains a ratio, for proportion with default interval.
    int translation;
    std::vector<int> * discretePopulation;
    std::vector<DeltaOmega> * frequencyDistribution;

public:
    ClassicalDiscreteGenerator( unsigned int seed);// : currentSeed(seed), generatorSUP(RAND_MAX);
    // You can pass in a pointer to a time_t object that time will fill up with the current time (and the return value is the same
    // one that you pointed to). If you pass in NULL, it just ignores it and merely returns a new time_t object that represents the current time.
    ClassicalDiscreteGenerator();// : currentSeed( time(NULL)), generatorSUP(RAND_MAX);
    void resetExtractionInterval( int left, int right );// non const; resets instance members.
    void nextIntegerInInterval() const;
    void showDiscretePopulation() const;
    void showFrequencyDistribution() const;
    void showCumulatedFrequency() const;
    void buildDiscreteFrequencyDistribution();
    void buildOmega( double partizioneLeft ,double partizioneRight);
    unsigned int showCurrentSeed() const;
    int showSup() const;
}; // class ClassicalGenerator



} // namespace Common

} // namespace MonteCarlo

#endif // CLASSICALDISCRETEGENERATOR_H_INCLUDED
