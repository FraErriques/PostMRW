#ifndef CLASSICALDISCRETEGENERATOR_H_INCLUDED
#define CLASSICALDISCRETEGENERATOR_H_INCLUDED

#include <stdio.h>      /* NULL */
#include <stdlib.h>     /* RAND_MAX , srand , rand */
#include <time.h>       /* time */
#include "./DeltaOmega.h"
#include <cmath>
#include <iostream>
#include <vector>


namespace Common
{

namespace MonteCarlo
{


//    If seed is set to 1, the generator is reinitialized to its initial value and produces the same values as
//    before any call to rand or srand.
//    Two different initializations with the same seed will generate the same succession of results
//    in subsequent calls to rand.
class ClassicalDiscreteGenerator
{
private:
    unsigned int currentSeed;
    const int generatorSUP; // = RAND_MAX;// #include <stdlib.h>
    bool ConstructorCanContinue;
    void resetExtractionInterval( int left, int right );// non const; resets instance members. Called from Ctor only.
    std::string * ReasonForAbortingConstructor;
    double theIntervalMeasure;
    int Min, Sup;
    double omothetia;// it's rational since it contains a ratio, for proportion with default interval.
    int translation;
    std::vector<int> * discretePopulation;
    std::vector<DeltaOmega*> * frequencyDistribution;

public:
    // Note on time_seed: You can pass in a pointer to a time_t object that time will fill up with the
    // current time (and the return value is the same one that you pointed to). If you pass in NULL, it just
    // ignores it and merely returns a new time_t object that represents the current time.
    ClassicalDiscreteGenerator( unsigned int seed, int left, int right );// : currentSeed(seed), generatorSUP(RAND_MAX);
    ClassicalDiscreteGenerator( int left, int right);// : currentSeed( time(NULL)), generatorSUP(RAND_MAX);
    ~ClassicalDiscreteGenerator();//Dtor
    //
    int nextIntegerInInterval() const;
    void showDiscretePopulation() const;
    void showFrequencyDistribution() const;
    void showCumulatedFrequency() const;
    void buildDiscreteFrequencyDistribution();
    void buildOmega( int partizioneLeft ,int partizioneRight);
    unsigned int showCurrentSeed() const;
    int showSup() const;
}; // class ClassicalGenerator



} // namespace Common

} // namespace MonteCarlo

#endif // CLASSICALDISCRETEGENERATOR_H_INCLUDED
