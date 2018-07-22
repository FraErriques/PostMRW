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
#include "ClassicalDiscreteGenerator.h"

namespace Common
{

namespace MonteCarlo
{

ClassicalDiscreteGenerator::ClassicalDiscreteGenerator( unsigned int seed) : currentSeed(seed), generatorSUP(RAND_MAX)
{
    srand( seed);
    this->discretePopulation = nullptr;// necessary on Win.
    this->frequencyDistribution = nullptr;
}// END Ctor( unsigned int seed)


// You can pass in a pointer to a time_t object that time will fill up with the current time (and the return value is the same
// one that you pointed to). If you pass in NULL, it just ignores it and merely returns a new time_t object that represents the current time.
ClassicalDiscreteGenerator::ClassicalDiscreteGenerator() : currentSeed( time(NULL)), generatorSUP(RAND_MAX)
{
    srand( this->currentSeed);// i.e. time(NULL)
    this->discretePopulation = nullptr;// necessary on Win.
    this->frequencyDistribution = nullptr;
}// END Ctor()



void ClassicalDiscreteGenerator::resetExtractionInterval( int left, int right )
         {
            this->Min = left;// reset.
            this->Sup = right;// reset.
             this->theIntervalMeasure = right-left;// init for both models; correct only for discrete ones.
             // default model is [min,sup) on [0,RAND_MAX)==[0,32767)
             this->omothetia = this->theIntervalMeasure/((double)RAND_MAX);
             this->translation = left;// NB. valid within a single population.
             if( nullptr != this->discretePopulation)
             {
               delete this->discretePopulation;
             }// else the population vector is already null.
             this->discretePopulation = new std::vector<int>();// get a new population.
             //
             if( nullptr != this->frequencyDistribution)
             {
               delete this->frequencyDistribution;
             }// else the frequencyDistribution vector is already null.
             this->frequencyDistribution = new std::vector<DeltaOmega>();// get a new frequencyDistribution.
         }// ex_Ctor


        void ClassicalDiscreteGenerator::nextIntegerInInterval() const
        {
            double temp =  rand()*this->omothetia + this->translation;
            this->discretePopulation->push_back( (int)temp);
        }// END nextIntegerInInterval



        void ClassicalDiscreteGenerator::showDiscretePopulation() const
        {
          if( nullptr == this->discretePopulation)
                {
                    return;
                }
            for( std::vector<int>::const_iterator it=this->discretePopulation->begin();
                 it!=this->discretePopulation->end();
                 it++
                )
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
            for( std::vector<DeltaOmega>::const_iterator it=this->frequencyDistribution->begin();
                 it!=this->frequencyDistribution->end();
                 it++
                )
                {
                    std::cout<<" mediana "<< it->mediana
                    <<" eta "<<it->eta
                    <<" DeltaOmegaCardinality " << it->DeltaOmegaCardinality
                    <<" categoryFrequency "<< it->categoryFrequency <<std::endl;
                }
        }// END showFrequencyDistribution() const

        void ClassicalDiscreteGenerator::showCumulatedFrequency() const
        {
           double CumulatedFrequency = 0.0;
           for( std::vector<DeltaOmega>::iterator frequencyWriter=this->frequencyDistribution->begin();
                frequencyWriter != this->frequencyDistribution->end();
                frequencyWriter++
              )
              {
                 CumulatedFrequency += frequencyWriter->categoryFrequency;
              }// for frequencyWriter
              std::cout<< "\n\n\t CumulatedFrequency : " << CumulatedFrequency <<std::endl;
        }// END showCumulatedFrequency() const


        void ClassicalDiscreteGenerator::buildDiscreteFrequencyDistribution()
        {
            int populationCardinality = this->discretePopulation->size();
            double elementPresenceWeight = +1.0/populationCardinality;
            for( std::vector<int>::const_iterator populationReader=this->discretePopulation->begin();
                 populationReader != this->discretePopulation->end();
                 populationReader++
               )
               {
                   for( std::vector<DeltaOmega>::iterator frequencyWriter=this->frequencyDistribution->begin();
                        frequencyWriter != this->frequencyDistribution->end();
                        frequencyWriter++
                      )
                      {
                          if( frequencyWriter->belongsToDeltaOmega( *populationReader) )
                          {
                              frequencyWriter->categoryFrequency += elementPresenceWeight;
                              frequencyWriter->DeltaOmegaCardinality++;
                              break;// belonging class found.
                          }
                      }// for frequencyWriter

               }// for populationReader
        }// buildDiscreteFrequencyDistribution()



//        void ClassicalDiscreteGenerator::buildOmega( int partizioneLeft ,int partizioneRight)
//        {//this->frequencyDistribution has been built by Ctor.
//            for( double position=partizioneLeft; position<partizioneRight; position++)
//            {
//                DeltaOmega * curDeltaOmega = new DeltaOmega( position, +1E-80);// TODO verify
//                this->frequencyDistribution->push_back( *curDeltaOmega );
//            }//for
//        }//buildOmega

void ClassicalDiscreteGenerator::buildOmega(
    double partizioneLeft
    ,double partizioneRight
    )
{//this->frequencyDistribution has been built by Ctor.
    double eta = (partizioneRight-partizioneLeft)/20.0;//each DeltaOmega is 1/10*Omega. Eta is 1/2*DeltaOmega.
    if(eta<+0.5){eta=+1.0/+2.0;}
    double mediana = 0;// init
    for( double position=partizioneLeft-+1.0E-80; mediana<partizioneRight; position+=2.0*eta)
    {
        mediana = position+eta;
        if(mediana>=partizioneRight){break;}
        DeltaOmega * curDeltaOmega = new DeltaOmega( mediana, eta);// TODO verify
        this->frequencyDistribution->push_back( *curDeltaOmega );
    }//for
}//buildOmega


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
