
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


void fReader_byString()
{
   std::ifstream dumpReader( "./PrimeIntegral_fromOrigin_.txt", std::fstream::in );// read-only.
   dumpReader.seekg( 0, dumpReader.end);
   long dumpSize = dumpReader.tellg();
   dumpReader.seekg( dumpSize/2, dumpReader.beg );
   int bufSize = 80;
   char * buf = nullptr;
   for( ; !dumpReader.eof() && dumpSize>bufSize; )
   {
       dumpSize = dumpReader.tellg();
       bufSize = dumpSize/2;//adaptive.
       buf = new char[bufSize];
       dumpReader.read( buf, bufSize-1);
       buf[bufSize-1]=0;
       dumpReader.seekg( dumpSize/2, dumpReader.beg );
       std::cout << buf << std::endl;
       delete[] buf;
   }
   dumpReader.close();
}// fReader_byString



void fReader_byRow()
{
   std::ifstream dumpReader( "./PrimeIntegral_fromOrigin_.txt", std::fstream::in );// read-only.
   dumpReader.clear();
   dumpReader.seekg(0, ios::beg);// from start
   int bufSize = 20;
   char buf[bufSize];
   for( ; !dumpReader.eof(); )
   {
       dumpReader.getline( buf, bufSize-1,'\r');
       buf[bufSize-1]=0;
       std::cout << buf << std::endl;
   }
   dumpReader.close();
}// fReader_byRow

//NB.////dumpReader.clear();
//dumpReader.seekg( bufSize, ios::beg);// from start



unsigned long factorial( unsigned int par)
{
    unsigned long res = +1UL;
    for( int c=par; c>+1; c--)
    {
        res *= c;
    }
    return res;
}// factorial



// TODO:
    double ExpIntegralEi_Ramanujan( double x)//( Complex x)
    {// notes on a convergent series
        /*
        N[(+EulerGamma + Log[x]) +
          Exp[x/2]*Sum[((-1)^(n - 1) (+x)^n)/((n!)*(2^(n - 1)))*
             Sum[+1/(2*k + 1), {k, 0, Floor[(n - 1)/2]}], {n, +1, +99}]]
        */
        const double EulerGamma = +0.577216;
        double internalFactor = 0.0;
        double externalFactor = 0.0;
        for( int n=+1; n<=+9; n++)
        {
            for(int k=0; k< floor(((double)n - 1.0)/2.0); k++ )
            {
                internalFactor += +1.0/(2.0*(double)k + 1.0);
            }

            externalFactor += pow((-1.0),((double)n - 1.0)) * pow(+x,(double)n) /(factorial(n)* pow(2.0,((double)n - 1)) ) * internalFactor;
            internalFactor = 0.0;// reset.
        }
        externalFactor += EulerGamma + log(x);
        // ready.
        return externalFactor;
  }// Ramanujan series



int main()
{
    double res = ExpIntegralEi_Ramanujan( +33.0);
    //fReader_byRow();

//     PrimesFinder::Primes * p = new PrimesFinder::Primes(4349999);
//     unsigned long res = (*p)[3];
//     res = (*p)[65123];
//     res = (*p)[183123];
//     res = (*p)[253000];
//     res = (*p)[300001];
     //PrimesFinder::Primes * p = new PrimesFinder::Primes( 9888777, 9888999, "primeCustomFile");
     //p->Start_PrimeDump_FileSys();

    //
    std::cout<<"\n\t Strike Enter to leave\t";
    getchar();
    return 0;
}
