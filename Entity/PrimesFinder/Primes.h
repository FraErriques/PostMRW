
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
    /// Data
    /// Ctor
    Primes();
    ~Primes  ( void );
    /// method
    unsigned long getActualLength();
    unsigned long getLastOrdinal();
    unsigned long getLastPrime();
    unsigned long operator[] (const unsigned long & requiredOrdinal) const;// it's a read-only utility; syntax: Prime[ordinal]==...
    // to be tested:
    const char * getDefaultPrimeDumpFullPath( const std::string & sectionNameInFile);
    void  createOrAppend( const std::string & );
    char * lastRecordReaderByString( const std::string & fullPath);
    void IntegralFileFromStartFSproducer( unsigned long sup ) const;


   /// algo ///////////////////////////////////////////////////////////////////////


private:
    /// Data
    unsigned long lastOrdinal;
    unsigned long lastPrime;
    unsigned long desiredOrdinal;
    unsigned long desiredPrime;
    unsigned long actualPrimaryFileLength;
    const char * theDumpPath = nullptr;
    bool isHealthlyConstructed = false;
    bool canOperate = false;
    // Riemann exponent s in C; s=:(sigma + i*t).
    double sigma;
    double t;
    /// Ctor
    // copying methoda : not usable->private.
    // Copy Constructor
    Primes   ( const Primes & original );
    // operator= only between isomorphic matrixes
    Primes & operator=   ( const Primes & second );
    /// method
    const std::string & tokenEncoder( unsigned long ordinal, unsigned long prime ) const;
    void recoverLastRecord( const char * fromFile);// cannot be const: fills lastOrdinal, lastPrime.




};// class

}// nmsp

#endif
