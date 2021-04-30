
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

double factorial( unsigned int par, bool onReals)
{
    double res = +1.0;
    for( int c=par; c>+1; c--)
    {
        res *= c;
    }
    return res;
}// factorial


// TODO: verificare convergenza, per zone del piano complesso.
Numerics::Complex ExpIntegralEi_Ramanujan( Numerics::Complex zVariable)
{/* notes on a convergent series:
    N[(+EulerGamma + Log[zVariable]) +
    Exp[zVariable/2]*Sum[((-1)^(n - 1) (zVariable)^n)/((n!)*(2^(n - 1)))*
    Sum[+1/(2*k + 1), {k, 0, Floor[(n - 1)/2]}], {n, +1, +99}]]
    */
    const double EulerGamma = +0.577216;// Euler-Mascheroni.
    Numerics::Complex externalAddend(0.0, 0.0);// init to zero the Sum element. The external one is never updated.
    Numerics::Complex internalFactor(+1.0, 0.0);// the internal one is a factor and gets reset at each external-loop.
    //
    for( int n=+1; n<=+12; n++)// NB. ######## factorial does not support (now) more than 12! ######
    {
        for(int k=0; k< floor(((double)n - 1.0)/2.0); k++ )
        {
            internalFactor += +1.0/(2.0*(double)k + 1.0);
        }// end internal-factor finite sum.
        externalAddend += (zVariable^n)*pow((-1.0),((double)n - 1.0))  /(factorial(n,true)* pow(2.0,((double)n - 1)) ) * internalFactor;
        internalFactor = +1.0;// reset.
    }// end external series step.
    externalAddend += zVariable.LnC() + EulerGamma;// final addenda.
    // ready.
    return externalAddend;
}// Ramanujan series



int main()
{
    Numerics::Complex zVariable( +33.0, 1952.0 );
    Numerics::Complex res = ExpIntegralEi_Ramanujan( zVariable);
    std::cout<<"\n\t ExpIntegralEi_Ramanujan( "<< zVariable.Re() <<" +I* "<< zVariable.Im() <<")==" << res.Re() <<" +I* "<< res.Im() <<std::endl;

//    for( int c=0; c<19; c++)
//    {
//        std::cout<<"\n\tFactorial("<<c<<")=="<<factorial(c)<<std::endl;
//        std::cout<<"\n\tFactorial("<<c<<")=="<<factorial(c, true)<<std::endl;
//    }


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
