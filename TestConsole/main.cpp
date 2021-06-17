
//#include <boost/lambda/lambda.hpp>
//#include "../Common/DbConnectionService/dbCall.h"
// TODO study <algorithm>
#include <iostream>
#include "../Common/StringBuilder/StringBuilder.h"
#include "../Common/Config_wrap/Config_wrap.h"
#include "../Common/Stream/stream_io_.h"
#include "../Common/Stream/RWtxtfile.h"
#include "../Process/MonteCarlo_wrap/MonteCarlo_wrap.h"
#include "../Common/MonteCarlo/BaseConverter.h"
#include "../Common/MonteCarlo/ClassicalDiscreteGenerator.h"
#include "../Common/MonteCarlo/ClassicalContinuousGenerator.h"
#include "../Entity/Integration/Integration.h"
#include "../Entity/Integration/Integrate.h"
#include "../Entity/PrimesFinder/Primes.h"
#include "../Entity/Complex/Complex.h"


    struct SingleFactor
    {
        unsigned long  pi;
        unsigned long  ai;
    };

SingleFactor * protoFactorize( unsigned long par)
{
    SingleFactor * factorization = new SingleFactor[100];// TODO #
    unsigned long sogliaStimata = par/2;// greatest involved-prime is the cofactor of the smallest one(i.e. 2).
    // ordinaleStimato == LogIntegral[ sogliaStimata] NB. overstimate it, for safety.
//        Entity::Integration::FunctionalForm LogIntegral = LogIntegral_coChain;// function pointer.
//        double LogIntegral_ofInfPar = Entity::Integration::trapezi( +2.0, (double)infLeft, ((double)infLeft-2.0)*4, LogIntegral );
//     unsigned ordinaleStimato = (unsigned long)LogIntegral_ofInfPar;//TODO stima !
    unsigned ordinaleStimato = 11UL; // TODO
    for(int c=0; c<ordinaleStimato; c++)
    {// init.
        factorization[c].pi = 0;
        factorization[c].ai = 0;
    }
    // TODO readRange( 1, ordinaleStimato);
    unsigned long * involvedPrimes = new unsigned long[ordinaleStimato]{2,3,5,7,11,13,17,19,23,29,31};
    unsigned long dividendo, divisore;
    dividendo = par;
    double realQuotient;
    unsigned long intQuotient;
    int i=0;// start from +2. indice nel vettore dei primi.
    int acc=0;// indice nel vettore dei risultati.
    divisore=involvedPrimes[i];
    bool lastDivisionWasDiophantine =  false;
    double soglia = (double)dividendo/2.0; // greatest involved-prime is the cofactor of the smallest one(i.e. 2).
    //
    for(  ; +1<dividendo; )
    {
        realQuotient = (double)dividendo/(double)divisore;
        intQuotient = dividendo/divisore;
        if( realQuotient-intQuotient <+1.0E-80 )// ####### ramo lastDivisionWasDiophantine ##
        {// divisione diofantea : the prime acting as divisor is a factor (i.e. divides dividendo).
            if(  lastDivisionWasDiophantine)
            {
                // factorization[acc].pi is already correct: do nothing.
                factorization[acc].ai++;// increment ai on this pi
                lastDivisionWasDiophantine =  true;
            }
            else if( ! lastDivisionWasDiophantine)
            {
                factorization[acc].pi = divisore;// promote current prime and its exponent.
                factorization[acc].ai++;// increment ai on this pi
                lastDivisionWasDiophantine =  true;
            }// No other else.
            // in common btw curDivDiophantine
            dividendo = intQuotient;// NB. swap the dividendo, after a successful
        }// if // divisione diofantea : the prime acting as divisor is a factor (i.e. divides dividendo).
        else// ### no Diophantine division ##
        {// else: goto test next prime, as divisor.
            if(  lastDivisionWasDiophantine)
            {
                acc++;// next factor slot, in the results array.
            }
            else if( ! lastDivisionWasDiophantine)
            {
                // NO acc++ we don't have an idoneous factor, yet.
            }// No other else.
            // factors in common btw lastDivisionWasDiophantine when curDivNOTDiophantine
            i++;// test next prime, as factor.
            lastDivisionWasDiophantine =  false;
            divisore=involvedPrimes[i];// to next prime, if cur one works no more
        }// else// ### no Diophantine division ##
    }
    //..
    delete[] involvedPrimes;
    // ready.
    return factorization;// NB. the caller has to delete.
}// protoFactorize


int main()
{
<<<<<<< HEAD
 
=======
     PrimesFinder::Primes * p = new PrimesFinder::Primes(100);
     unsigned long lastOrdinal = p->getLastOrdinal();
     unsigned long lastPrime = p->getLastPrime();
     unsigned long presentLength = p->getActualLength();
     unsigned long res = (*p)[1];// TODO bug [1] #######################################################
     delete p;
     //
//    SingleFactor * factorization = protoFactorize( 2*2*2 * 7 );// NB. when a prime not in divisors list is required, you get a div_by_0.
//    delete[] factorization;
>>>>>>> refs/remotes/origin/WinPlatf

    //
    std::cout<<"\n\t Strike Enter to leave\t";
    getchar();
    return 0;
}// main


/* cantina

// declare a threshold under the actual one. In case of accidental start, mothing will be done.
     PrimesFinder::Primes * p = new PrimesFinder::Primes(100);
     unsigned long lastOrdinal = p->getLastOrdinal();
     unsigned long lastPrime = p->getLastPrime();
     unsigned long presentLength = p->getActualLength();
     unsigned long res = (*p)[3000];// TODO bug [1] #######################################################
     unsigned long Delta = res - 4349987;// test
     unsigned long *pRange = new unsigned long[100];
     for( int c=0; c<100; c++)
     {
         pRange[c] = (*p)[c+300000];
     }
     for( int c=0; c<100; c++)
     {
         std::cout<<"\n\t Prime["<<c+2<<"]== "<<pRange[c];
     }
     //...
     delete p;
     delete[] pRange;

//     PrimesFinder::Primes * p = new PrimesFinder::Primes(4349999);
//     unsigned long res = (*p)[3];
//     res = (*p)[65123];
//     res = (*p)[183123];
//     res = (*p)[253000];
//     res = (*p)[300001];
//     delete p;
     //PrimesFinder::Primes * p = new PrimesFinder::Primes( 9888777, 9888999, "primeCustomFile");
     //p->Start_PrimeDump_FileSys();

*/
