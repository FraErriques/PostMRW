
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



int main()
{
    PrimesFinder::Primes * p = new PrimesFinder::Primes(100);
    unsigned long overflowFlag = -1UL;
    unsigned long lastOrdinal = p->getLastOrdinal();
    unsigned long lastPrime = p->getLastPrime();
    unsigned long presentLength = p->getActualLength();
    delete p;
    p = new PrimesFinder::Primes(5000000); // re-use
    p->Start_PrimeDump_FileSys();
    lastOrdinal = p->getLastOrdinal();
    lastPrime = p->getLastPrime();
    presentLength = p->getActualLength();

     unsigned long res = (*p)[25];// TODO bug [lastOrdinal] #######################################################
     res = (*p)[1];
     res = (*p)[2];
     res = (*p)[3];
     res = (*p)[lastOrdinal];
     res = (*p)[lastOrdinal-1];
     res = (*p)[lastOrdinal-2];
     res = (*p)[lastOrdinal-3];
     res = (*p)[lastOrdinal-100];
     if(overflowFlag==res)
     {
         std::cout<<"\n\t invalid index fed to operator[] "<<std::endl;
     }
     for( int c=1970; c<=15021; c++)
     {
         std::cout<<"\n\t Prime["<<c<<"]=="<<(*p)[c];
     }
     const char * theDumpPath = p->getPrimeDumpFullPath( "primeDefaultFile");// Default Section Name.
     if( nullptr != theDumpPath)
      {}// else : TODO not-healthly built.
     char * straightContentOfDumpTail  = p->dumpTailReader( theDumpPath);
//PrimesFinder::Primes::DumpElement * dumpTail = p->recoverDumpTail( straightContentOfDumpTail);
     PrimesFinder::Primes::SingleFactor * factorization = p->IntegerDecomposition( 1279*7);
     delete[] theDumpPath;
     delete[] straightContentOfDumpTail;
     delete[] factorization;
//delete[] dumpTail;
     delete p;
    //
    std::cout<<"\n\n\n\t Strike Enter to leave\t";
    getchar();
    return 0;
}// main


/* cantina

//     unsigned long res = (*p)[lastOrdinal-2];// TODO bug [lastOrdinal] #######################################################
//     if(overflowFlag==res)
//     {
//         std::cout<<"\n\t invalid index fed to operator[] "<<std::endl;
//     }
//     for( int c=1; c<100; c++)
//     {
//         std::cout<<"\n\t Prime["<<c<<"]=="<<(*p)[c];
//     }
//     delete p;
//     //
////    SingleFactor * factorization = protoFactorize( 2*2*2 * 7 );// NB. when a prime not in divisors list is required, you get a div_by_0.
////    delete[] factorization;


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
