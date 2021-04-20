
//#include <boost/lambda/lambda.hpp>
//#include "../Common/DbConnectionService/dbCall.h"
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




int main()
{
  PrimesFinder::Primes * p = new PrimesFinder::Primes();
     const char * primeDefaultFile = p->getDefaultPrimeDumpFullPath( "primeDefaultFile");
     p->createOrAppend( primeDefaultFile);
     const char * straightContent = p->lastRecordReader( primeDefaultFile);
     std::cout<<"\n\n"<< straightContent;
     delete primeDefaultFile;
     delete straightContent;


    //
    std::cout<<"\n\t Strike Enter to leave\t";
    getchar();
    return 0;
}
