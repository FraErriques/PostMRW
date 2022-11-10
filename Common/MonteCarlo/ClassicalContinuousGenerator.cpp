/*
    If seed is set to 1, the generator is reinitialized to its initial value and produces the same values as
    before any call to rand or srand.
    Two different initializations with the same seed will generate the same succession of results
    in subsequent calls to rand.
*/


#include <stdio.h>      /* NULL */
#include <stdlib.h>     /* RAND_MAX , srand , rand */
#include <time.h>       /* time */
#include <cmath>
#include <iostream>
#include "../StringBuilder/StringBuilder.h"
#include "ClassicalContinuousGenerator.h"
#include "../LogFs_wrap/LogFs_wrap.h"

namespace Common
{

namespace MonteCarlo
{



ClassicalContinuousGenerator::ClassicalContinuousGenerator( unsigned int seed, double left, double right) : currentSeed(seed), generatorSUP(RAND_MAX)
{// Ctor with seed
    srand( seed);
    this->continuousPopulation = nullptr;// necessary on Win.
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
}// END Ctor( unsigned int seed)


// You can pass in a pointer to a time_t object that time will fill up with the current time (and the return value is the same
// one that you pointed to). If you pass in NULL, it just ignores it and merely returns a new time_t object that represents the current time.
ClassicalContinuousGenerator::ClassicalContinuousGenerator(double left, double right) : currentSeed( time(NULL)), generatorSUP(RAND_MAX)
{// Ctor without seed
    srand( this->currentSeed);// i.e. time(NULL)
    this->continuousPopulation = nullptr;// necessary on Win.
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
}// END Ctor()



void ClassicalContinuousGenerator::resetExtractionInterval( double left, double right )
{// if the call comes from a Ctor the pointers are null and there's no deletion.
 // if the call comes along the lifetime of an instance, the pointers are not null; so there is deletion and new construction.
    this->Min = left;// reset.
    this->Sup = right;// reset.
    this->theIntervalMeasure = right-left;// init for Continuous model: measure(+1,+3)==+2==right-left
    // default model is [min,sup) on [0,RAND_MAX)==[0,32767)
    this->omothetia = this->theIntervalMeasure/((double)RAND_MAX);
    this->translation = left;// NB. valid within a single population.
    if( nullptr != this->continuousPopulation)
    {
        delete this->continuousPopulation;
    }// else the population vector is already null.
    this->continuousPopulation = new std::vector<double>();// get a new population.
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
}// ex_Ctor

ClassicalContinuousGenerator::~ClassicalContinuousGenerator()
{// Dtor
    if( nullptr != this->continuousPopulation)
    {
        delete this->continuousPopulation;
        this->continuousPopulation = nullptr;
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


    double ClassicalContinuousGenerator::nextDoubleInInterval() const
    {
        Process::LogWrappers::SectionOpen("ClassicalContinuousGenerator::nextDoubleInInterval()", 0);
        double originalExtracted = rand();
        double temp =  originalExtracted*this->omothetia + this->translation;
        if( this->Min<= temp
            && this->Sup>temp )
        {
            this->continuousPopulation->push_back( temp);
        }
        else
        {// log about
            Common::StringBuilder sb(1024);
            sb.append("element out of bounds extracted: original was ");
            std::string * strOriginalExtracted = Common::StrManipul::doubleToString( originalExtracted);
            std::string * strAfterAffinity = Common::StrManipul::doubleToString( temp);
            sb.append( strOriginalExtracted->c_str() );
            sb.append(" while after affinity it was ");
            sb.append( strAfterAffinity->c_str() );
            Process::LogWrappers::SectionContent( sb.str().c_str(), 0 );
            delete strOriginalExtracted;
            delete strAfterAffinity;
        }
        Process::LogWrappers::SectionClose();
        // ready.
        return temp;
    }// END nextDoubleInInterval



        void ClassicalContinuousGenerator::showContinuousPopulation() const
        {
          if( nullptr == this->continuousPopulation)
                {
                    return;
                }
            for( std::vector<double>::const_iterator it=this->continuousPopulation->begin();
                 it!=this->continuousPopulation->end();
                 it++
                )
                {
                    std::cout<< *it<<std::endl;
                }
        }// END showContinuousPopulation() const


        void ClassicalContinuousGenerator::showFrequencyDistribution() const
        {
          if( nullptr == this->frequencyDistribution)
                {
                    return;
                }
            for( std::vector<DeltaOmega*>::const_iterator it=this->frequencyDistribution->begin();
                 it!=this->frequencyDistribution->end();
                 it++
                )
                {
                    std::cout<<" mediana "<< (*it)->mediana
                    <<" eta "<<(*it)->eta
                    <<" DeltaOmegaCardinality " << (*it)->DeltaOmegaCardinality
                    <<" categoryFrequency "<< (*it)->categoryFrequency <<std::endl;
                }
        }// END showFrequencyDistribution() const

        void ClassicalContinuousGenerator::showCumulatedFrequency() const
        {
           double CumulatedFrequency = 0.0;
           for( std::vector<DeltaOmega*>::iterator frequencyWriter=this->frequencyDistribution->begin();
                frequencyWriter != this->frequencyDistribution->end();
                frequencyWriter++
              )
              {
                 CumulatedFrequency += (*frequencyWriter)->categoryFrequency;
              }// for frequencyWriter
              std::cout<< "\n\n\t CumulatedFrequency : " << CumulatedFrequency <<std::endl;
        }// END showCumulatedFrequency() const


        void ClassicalContinuousGenerator::buildContinuousFrequencyDistribution()
        {
            //Process::LogWrappers::SectionOpen("ClassicalContinuousGenerator::buildContinuousFrequencyDistribution()", 0);
            int populationCardinality = this->continuousPopulation->size();
            double elementPresenceWeight = +1.0/populationCardinality;
            for( std::vector<double>::const_iterator populationReader=this->continuousPopulation->begin();
                 populationReader != this->continuousPopulation->end();
                 populationReader++
               )
               {
                   bool hasFoundHisBelongingDeltaOmega = false;
                   for( std::vector<DeltaOmega*>::iterator frequencyWriter=this->frequencyDistribution->begin();
                        frequencyWriter != this->frequencyDistribution->end();
                        frequencyWriter++
                      )
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
                          Process::LogWrappers::SectionContent( sb.str().c_str(), 0);
                          delete strRepresentationOfPopulationReader;
                      }// end log.
               }// for populationReader
               Process::LogWrappers::SectionClose();
        }// buildContinuousFrequencyDistribution()



void ClassicalContinuousGenerator::buildOmega(
    double partizioneLeft
    ,double partizioneRight
    )
{//this->frequencyDistribution has been built by Ctor.
    double eta = (partizioneRight-partizioneLeft)/20.0;//each DeltaOmega is 1/10*Omega. Eta is 1/2*DeltaOmega.
    double mediana = partizioneLeft;// init
    for( double position=partizioneLeft-+1.0E-80; mediana<partizioneRight; position+=2.0*eta)
    {
        mediana = position+eta;
        if(mediana>=partizioneRight){break;}
        DeltaOmega * curDeltaOmega = new DeltaOmega( mediana, eta);// TODO verify
        this->frequencyDistribution->push_back( curDeltaOmega );
    }//for
}//buildOmega




unsigned int ClassicalContinuousGenerator::showCurrentSeed() const
{
    return this->currentSeed;
}

double ClassicalContinuousGenerator::showSup() const
{
    return this->generatorSUP;
}


} // namespace Common

} // namespace MonteCarlo
