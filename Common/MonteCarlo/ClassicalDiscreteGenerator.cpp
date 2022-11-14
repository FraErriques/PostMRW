#include <stdio.h>      /* NULL */
#include <stdlib.h>     /* RAND_MAX , srand , rand */
#include <time.h>       /* time */
#include <cmath>
#include <iostream>
#include "../StringBuilder/StringBuilder.h"
#include "../LogFs_wrap/LogFs_wrap.h"
#include "ClassicalDiscreteGenerator.h"


namespace Common
{

namespace MonteCarlo
{
/*
    If seed is set to 1, the generator is reinitialized to its initial value and produces the same values as
    before any call to rand or srand.
    Two different initializations with the same seed will generate the same succession of results
    in subsequent calls to rand.
*/


ClassicalDiscreteGenerator::ClassicalDiscreteGenerator( unsigned int seed, int left, int right) : currentSeed(seed), generatorSUP(RAND_MAX)
{
    Process::LogWrappers::SectionOpen("ClassicalDiscreteGenerator:: Ctor with seed",0);
    srand( seed);
    this->discretePopulation = nullptr;// necessary on Win.
    this->frequencyDistribution = nullptr;
    if(right-left <=0)
    {
        this->ReasonForAbortingConstructor = new std::string("wrong boundaries: should be left<right.");
        this->ConstructorCanContinue = false;
    }
    else
    {
        this->ReasonForAbortingConstructor = nullptr;
        resetExtractionInterval( left, right );
    }
    Process::LogWrappers::SectionClose();
}// END Ctor( unsigned int seed)


// You can pass in a pointer to a time_t object that time will fill up with the current time (and the return value is the same
// one that you pointed to). If you pass in NULL, it just ignores it and merely returns a new time_t object that represents the current time.
ClassicalDiscreteGenerator::ClassicalDiscreteGenerator( int left, int right) : currentSeed( time(NULL)), generatorSUP(RAND_MAX)
{// Ctor without seed
    Process::LogWrappers::SectionOpen("ClassicalDiscreteGenerator:: Ctor without seed",0);
    srand( this->currentSeed);// i.e. time(NULL)
    this->discretePopulation = nullptr;// necessary on Win.
    this->frequencyDistribution = nullptr;
    if(right-left <=0)
    {
        this->ReasonForAbortingConstructor = new std::string("wrong boundaries: should be left<right.");
        this->ConstructorCanContinue = false;
    }
    else
    {
        this->ReasonForAbortingConstructor = nullptr;
        resetExtractionInterval( left, right );
    }
    Process::LogWrappers::SectionClose();
}// END Ctor()



void ClassicalDiscreteGenerator::resetExtractionInterval( int left, int right )
{
    //NB. for DiscreteGenerator the domain is a compact [left,right]
    Process::LogWrappers::SectionOpen("ClassicalDiscreteGenerator::resetExtractionInterval",0);
    this->Min = left;// reset.
    this->Sup = right;// reset.
    Process::LogWrappers::SectionContent_variable_name_value("this->Min", (long long int)this->Min,0);
    Process::LogWrappers::SectionContent_variable_name_value("this->Sup", (long long int)this->Sup,0);
    this->theIntervalMeasure = (double)right-(double)left;// init for Discrete models: measure[+1,+3]==+3=={1,2,3}==right-left+1
    // but the omothetia is on continuous measures, so Measure([+1,+3])==+2 -> Meas([a,b]==b-a
    Process::LogWrappers::SectionContent_variable_name_value("this->theIntervalMeasure", (long long int)this->theIntervalMeasure,0);
    // default model is [min,sup) on [0,RAND_MAX)==[0,32767)
    this->omothetia = this->theIntervalMeasure/((double)RAND_MAX);
    Process::LogWrappers::SectionContent_variable_name_value("this->omothetia",  this->omothetia,0);
    this->translation = left;// shift
    Process::LogWrappers::SectionContent_variable_name_value("this->translation", (long long int)this->translation,0);
    if( nullptr != this->discretePopulation)
    {
        delete this->discretePopulation;
    }// else the population vector is already null.
    this->discretePopulation = new std::vector<int>();// get a new population.
    //
    if( nullptr != this->frequencyDistribution)
    {
        for( std::vector<DeltaOmega*>::const_iterator it=this->frequencyDistribution->begin();
             it!=this->frequencyDistribution->end();
             it++
            )
        {// first delete node pointees : the scalars are DeltaOmega.
            delete (*it);
        }
        this->frequencyDistribution->clear();//then delete the pointers.
        delete this->frequencyDistribution;
        this->frequencyDistribution = nullptr;
    }// else the frequencyDistribution vector is already null.
    this->frequencyDistribution = new std::vector<DeltaOmega*>();// get a new frequencyDistribution.
    Process::LogWrappers::SectionClose();
}// ex_Ctor


ClassicalDiscreteGenerator::~ClassicalDiscreteGenerator()
{// Dtor
    if( nullptr != this->discretePopulation)
    {
        delete this->discretePopulation;
        this->discretePopulation = nullptr;
    }// else the population vector is already null.
    //
    if( nullptr != this->frequencyDistribution)
    {
        for( std::vector<DeltaOmega*>::const_iterator it=this->frequencyDistribution->begin();
             it!=this->frequencyDistribution->end();
             it++
            )
        {// first delete node pointees : the scalars are DeltaOmega.
            delete (*it);
        }
        this->frequencyDistribution->clear();//then delete the pointers.
        delete this->frequencyDistribution;
        this->frequencyDistribution = nullptr;
    }// else the frequencyDistribution vector is already null.
    //
    if( nullptr != this->ReasonForAbortingConstructor)
    {
        delete this->ReasonForAbortingConstructor;
        this->ReasonForAbortingConstructor = nullptr;
    }// else the ReasonForAbortingConstructor is already null.
}// Dtor


long double linerarInterpolation_inline(
    long double x0, long double x1,
    long double y0, long double y1,
    long double x   )
{
    long double DeltaX = x1 - x0;
    long double DeltaY = y1 - y0;
    long double DyOnDx = DeltaY/DeltaX;
    long double y;// y=y(x) the return value; image calculated.
    y = DyOnDx*(x-x0) + y0;
    //ready
    return y;
}// linerarInterpolation_inline

int ClassicalDiscreteGenerator::nextIntegerInInterval() const
{
    Process::LogWrappers::SectionOpen("nextIntegerInInterval",0);
    // model : randomized =  rand()*this->omothetia + this->translation;
    double randomized =  rand();
    Process::LogWrappers::SectionContent_variable_name_value("RAND_MAX= ", (long long)RAND_MAX,0);//2^31 i.e. a 32bit integer
    Process::LogWrappers::SectionContent_variable_name_value("randomized= ", randomized,0);
    double transformed = randomized*this->omothetia + this->translation;
    Process::LogWrappers::SectionContent_variable_name_value("this->omothetia= ", this->omothetia,0);
//Process::LogWrappers::SectionContent_variable_name_value("this->translation= ", (int)this->translation,0);
    Process::LogWrappers::SectionContent_variable_name_value("transformed= ", transformed,0);
    double rounded_to_nearest_integer = round( transformed);
    Process::LogWrappers::SectionContent_variable_name_value("rounded_to_nearest_integer= ", rounded_to_nearest_integer,0);
    this->discretePopulation->push_back( (int)rounded_to_nearest_integer);
    Process::LogWrappers::SectionClose();
    // ready
    return ( (int)rounded_to_nearest_integer);
}// END nextIntegerInInterval



void ClassicalDiscreteGenerator::showDiscretePopulation() const
{
    if( nullptr == this->discretePopulation)
    {
        return;
    }
    for( std::vector<int>::const_iterator it=this->discretePopulation->begin();
            it!=this->discretePopulation->end();
            it++  )
    {
        std::cout<< *it<<std::endl;
    }
}// END showDiscretePopulation() const


void ClassicalDiscreteGenerator::showFrequencyDistribution() const
{
    if( nullptr == this->frequencyDistribution)
    {
        return;
    }
    for( std::vector<DeltaOmega*>::const_iterator it=this->frequencyDistribution->begin();
        it!=this->frequencyDistribution->end();
        it++  )
    {
        std::cout<<" mediana "<< (*it)->mediana
        <<" eta "<<(*it)->eta
        <<" DeltaOmegaCardinality " << (*it)->DeltaOmegaCardinality
        <<" categoryFrequency "<< (*it)->categoryFrequency <<std::endl;
    }
}// END showFrequencyDistribution() const


void ClassicalDiscreteGenerator::showCumulatedFrequency() const
{
   double CumulatedFrequency = 0.0;
   for( std::vector<DeltaOmega*>::iterator frequencyWriter=this->frequencyDistribution->begin();
        frequencyWriter != this->frequencyDistribution->end();
        frequencyWriter++
      )
      {
         CumulatedFrequency += (*frequencyWriter)->categoryFrequency;
      }// for frequencyWriter
      std::cout<< "\n\n\t CumulatedFrequency [Discrete] : " << CumulatedFrequency <<std::endl;
}// END showCumulatedFrequency() const


void ClassicalDiscreteGenerator::buildDiscreteFrequencyDistribution()
{
    Process::LogWrappers::SectionOpen("ClassicalDiscreteGenerator::buildDiscreteFrequencyDistribution()",0);
    int populationCardinality = this->discretePopulation->size();
    double elementPresenceWeight = +1.0/populationCardinality;
    for( std::vector<int>::const_iterator populationReader=this->discretePopulation->begin();
         populationReader != this->discretePopulation->end();
         populationReader++  )
   {
       bool hasFoundHisBelongingDeltaOmega = false;
       for( std::vector<DeltaOmega*>::iterator frequencyWriter=this->frequencyDistribution->begin();
            frequencyWriter != this->frequencyDistribution->end();
            frequencyWriter++  )
          {
              if( (*frequencyWriter)->belongsToDeltaOmega( *populationReader) )
              {
                  (*frequencyWriter)->categoryFrequency += elementPresenceWeight;
                  (*frequencyWriter)->DeltaOmegaCardinality++;
                  hasFoundHisBelongingDeltaOmega = true;// NB. log if the element has found no DeltaOmega which it belongs to.
                  break;// belonging class found.
              }
          }// for frequencyWriter
          // NB. log if the element has found no DeltaOmega which it belongs to.
          if( ! hasFoundHisBelongingDeltaOmega)
          {
             Common::StringBuilder sb(1024);
             sb.append("element ");
             std::string * strRepresentationOfPopulationReader = Common::StrManipul::doubleToString( *populationReader);
             sb.append( *strRepresentationOfPopulationReader );
             sb.append(" has found no DeltaOmega which it belongs to.");
             Process::LogWrappers::SectionContent( sb.str().c_str(),0);
             delete strRepresentationOfPopulationReader;
          }// end log.
   }// for populationReader
   Process::LogWrappers::SectionClose();
}// buildContinuousFrequencyDistribution()



void ClassicalDiscreteGenerator::buildOmega( int partizioneLeft ,int partizioneRight)
{//this->frequencyDistribution has been built by Ctor.
    for( double position=partizioneLeft; position<=partizioneRight; position++)
    {
        DeltaOmega * curDeltaOmega = new DeltaOmega( position, +1E-80);// TODO verify
        this->frequencyDistribution->push_back( curDeltaOmega );
    }//for
}//buildOmega

//void ClassicalDiscreteGenerator::buildOmega( double partizioneLeft, double partizioneRight)
//{//this->frequencyDistribution has been built by Ctor.
//    double eta = (partizioneRight-partizioneLeft)/20.0;//each DeltaOmega is 1/10*Omega. Eta is 1/2*DeltaOmega.
//    if(eta<+0.5){eta=+1.0/+2.0;}
//    double mediana = 0;// init
//    for( double position=partizioneLeft-+1.0E-80; mediana<partizioneRight; position+=2.0*eta)
//    {
//        mediana = position+eta;
//        if(mediana>=partizioneRight){break;}
//        DeltaOmega * curDeltaOmega = new DeltaOmega( mediana, eta);// TODO verify
//        this->frequencyDistribution->push_back( curDeltaOmega );
//    }//for
//}//buildOmega


unsigned int ClassicalDiscreteGenerator::showCurrentSeed() const
{
    return this->currentSeed;
}

int ClassicalDiscreteGenerator::showSup() const
{
    return this->generatorSUP;
}


} // namespace Common
} // namespace MonteCarlo
