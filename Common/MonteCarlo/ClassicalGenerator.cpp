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
#include <cmath>
#include <iostream>


namespace Common
{

namespace MonteCarlo
{



// DeltaOmega is the DeltaAscissa in a Riemann partition of the Omega Event-space.
class DeltaOmega
{// an instance of DeltaOmega is a single step in the Riemann partition. The friend function "buildOmega" builds a vector, which contains the whole population of
 // DeltaOmega, which constitutes the whole Omega event space.
private:
    friend class ClassicalGenerator;
    double mediana;
    double eta;
    double categoryFrequency;
public:
    DeltaOmega( double medianaPoint, double etaMeasure)
    {
        this->mediana = medianaPoint;
        this->eta = etaMeasure;
        this->categoryFrequency = 0.0;// init and then add on.
    }// Ctor
    double getMediana(){return this->mediana;}
    bool belongsToDeltaOmega( double candidate)
    {
        bool res = fabs( candidate-this->mediana)<this->eta;
        return res;
    }
};// class DeltaOmega



class ClassicalGenerator
{
private:
    unsigned int currentSeed;
    const int SUP; // = RAND_MAX;// #include <stdlib.h>
    friend class DeltaOmega;
    bool ConstructorCanContinue;
    std::string * ReasonForAbortingConstructor;
    double theIntervalMeasure;
    double Min, Sup;
    double omothetia;
    bool leftClosed, rightClosed;
    std::vector<double> * continuousPopulation;
    std::vector<int> * discretePopulation;
    std::vector<DeltaOmega> * frequencyDistribution;

public:
ClassicalGenerator( unsigned int seed) : currentSeed(seed), SUP(RAND_MAX)
{
    srand( seed);
}// END Ctor( unsigned int seed)


ClassicalGenerator() : currentSeed( time(NULL)), SUP(RAND_MAX)
{
    srand( this->currentSeed);// i.e. time(NULL)
}// END Ctor()



void ex_CtorMonteCarloGenerator( double left, bool isLeftIncluded
                                 ,double right, bool isRightIncluded
                                 ,bool wantContinuousModel )
                         /*
                         :ConstructorCanContinue(true)
                         ,ReasonForAbortingConstructor(new std::string("NOT YET.") )
                         ,theIntervalMeasure(right-left)
                         ,Min(left), Sup(right)
                         ,leftClosed( isLeftIncluded), rightClosed( isRightIncluded)
                         ,population( new std::vector<double>() )
                         ,frequencyDistribution(  new std::vector<DeltaOmega>() )
                         */
         {
             //theIntervalMeasure = right-left;// init for both models; correct only for discrete ones.
             // default model is [min,sup) on [0,RAND_MAX)==[0,32767)
             if( ! wantContinuousModel)//---------------------- discrete -------------------------------------------------
             {
                 if( fabs(left-(int)left)>+1E-80
                     || fabs(right-(int)right)>+1E-80 )
                 {
                     ConstructorCanContinue = false;
                     delete this->ReasonForAbortingConstructor;//free memory of default Ctor message.
                     this->ReasonForAbortingConstructor = new std::string("Invocation of a discrete model, with non Diophantine boundaries. Aborting.");
                 }
                 else // estremi interi
                 {// non default boundary conditions.
                     if( !isLeftIncluded)
                     {
                         left += +1.0;
                         theIntervalMeasure -= +1.0;// NB. such corrections only affect the discrete case, since they have zero measure on continuous models.
                     }// else the default is already appropriate
                     if( isRightIncluded)
                     {
                         right += +1.0;
                         theIntervalMeasure += +1.0;
                     }// else the default is already appropriate
                 }// estremi interi
             }////-------END--------------- discrete -------------------------------------------------
             this->omothetia = this->theIntervalMeasure/((double)RAND_MAX);
             //
         }// ex_Ctor

        void nextIntegerInInterval() const
        {
            double temp =  rand()*this->omothetia+this->Min;
            this->discretePopulation->push_back( (int)temp);
        }// END nextIntegerInInterval

        void nextDoubleInInterval() const
        {
            double temp = rand()*this->omothetia+this->Min;
            if( this->rightClosed)
            {
                while( temp>this->Sup)
                {
                    temp = rand()*this->omothetia+this->Min;
                }
            }
            this->continuousPopulation->push_back( temp);
        }// END nextIntegerInInterval

        void showDiscretePopulation() const
        {
            for( std::vector<int>::const_iterator it=this->discretePopulation->begin();
                 it!=this->discretePopulation->end();
                 it++
                )
                {
                    std::cout<< *it<<std::endl;
                }
        }// END showDiscretePopulation() const

        void showContinuousPopulation() const
        {
            for( std::vector<double>::const_iterator it=this->continuousPopulation->begin();
                 it!=this->continuousPopulation->end();
                 it++
                )
                {
                    std::cout<< *it<<std::endl;
                }
        }// END showContinuousPopulation() const

        void showFrequencyDistribution() const
        {
            for( std::vector<DeltaOmega>::const_iterator it=this->frequencyDistribution->begin();
                 it!=this->frequencyDistribution->end();
                 it++
                )
                {
                    /*
                        double mediana;
                        double eta;
                        double categoryFrequency;
                    */
                    std::cout<<" mediana "<< *Common::StrManipul::doubleToString(it->mediana)<<" eta "
                    << *Common::StrManipul::doubleToString(it->eta)
                    <<" categoryFrequency "<<*Common::StrManipul::doubleToString(it->categoryFrequency) <<std::endl;
                }
        }// END showFrequencyDistribution() const

        void showCumulatedFrequency() const
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


        void buildDiscreteFrequencyDistribution()
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
                              break;// belonging class found.
                          }
                      }// for frequencyWriter

               }// for populationReader
        }// buildDiscreteFrequencyDistribution()



        void buildOmega( double partizioneLeft
                         ,double partizioneRight
                         ,double eta
                       )
        {//this->frequencyDistribution has been built by Ctor.
            for( double position=partizioneLeft-eta; position<=partizioneRight+eta; position+=2*eta)
            {
                DeltaOmega * curDeltaOmega = new DeltaOmega( position+eta, eta);// TODO verify
                this->frequencyDistribution->push_back( *curDeltaOmega );
            }//for
        }//buildOmega




int nextIntegerInInterval( int Min, int Threshold) const
{
    double omothetia = (double)(Threshold-Min)/(double)this->SUP;
    return rand()*omothetia+Min;
}// END nextIntegerInInterval


double nextProbabilityMeasure() const
{// i.e. in [0, +1.0]
    return ( ((double)rand())/((double)SUP) );
}// END nextProbabilityMeasure

unsigned int showCurrentSeed() const
{
    return this->currentSeed;
}

int showSup() const
{
    return this->SUP;
}


}; // class ClassicalGenerator



} // namespace Common

} // namespace MonteCarlo
