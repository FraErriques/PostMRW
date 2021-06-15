
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



void protoFactorize( unsigned long par)
{
    unsigned longsogliaStimata = par/2;// greatest involved-prime is the cofactor of the smallest one(i.e. 2).
    // ordinaleStimato == LogIntegral[ sogliaStimata] NB. overstimate it, for safety.
    unsigned ordinaleStimato = 56UL; // TODO
    // TODO readRange( 1, ordinaleStimato);
    unsigned long * involvedPrimes = new unsigned long[ordinaleStimato];
    //..
    delete[] involvedPrimes;
}// protoFactorize


int main()
{// declare a threshold under the actual one. In case of accidental start, mothing will be done.
     PrimesFinder::Primes * p = new PrimesFinder::Primes(100);
     unsigned long res = (*p)[2];// TODO bug [1] #######################################################
     unsigned long *pRange = new unsigned long[100];
     for( int c=0; c<100; c++)
     {
         pRange[c] = (*p)[c+2];
     }
     for( int c=0; c<100; c++)
     {
         std::cout<<"\n\t Prime["<<c+2<<"]== "<<pRange[c];
     }
     //...
     delete p;
     delete[] pRange;

    //
    std::cout<<"\n\t Strike Enter to leave\t";
    getchar();
    return 0;
}// main


/* cantina

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
