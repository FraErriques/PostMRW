
/**
 *  \class  Numerics::Primes
 *  \brief  a Analytic Number Theory class
 *  \author F. Erriques
 *  \since  2021#march#31 CET 2021
 */


#ifndef __Primes_
#define __Primes_

#include <string>
#include <fstream>
#include <stdlib.h>

using namespace std;

namespace PrimesFinder
{


class Primes
{

    public:
       /// Constructors and Destructors
       Primes(); // default Ctor
       // Destructor
       ~Primes  ( void );
       // some methoda
       const char * getDefaultPrimeDumpFullPath( const std::string & sectionNameInFile);
       void  createOrAppend( const std::string & );
       const char * lastRecordReader( const std::string & fullPath);

        private:
        unsigned long lastOrdinal;
        unsigned long lastPrime;
        bool canOperate = false;
        // Riemann exponent s in C; s=:(sigma + i*t).
        double sigma;
        double t;

   /// algo ////////////////////////////////////////////////////////////////////////////////////////////////////


private:
   /// data ////////////////////////////////////////////////////////////////////////////////////////////

};// class

}// nmsp

#endif
