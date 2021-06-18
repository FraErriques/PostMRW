
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
    struct SingleFactor
    {
        unsigned long  pi;
        unsigned long  ai;
    };

    /// Ctor
    Primes(unsigned long threshold);// default Section, on default config-file, dedicated for this class: i.e. ./PrimeConfig.txt
    // below: a Ctor to span (inf, max] :
    Primes(unsigned long infLeft, unsigned long maxRight, const std::string& desiredConfigSectionName);// CUSTOM Section, on default config-file.
    ~Primes  ( void );
    /// method
    unsigned long getActualLength();
    unsigned long getLastOrdinal();
    unsigned long getLastPrime();
    unsigned long operator[] (const unsigned long requiredOrdinal);// NON const; updates the members {lastOrdinal, lastPrime}.syntax: Prime[ordinal]==...
    const char * getPrimeDumpFullPath( const std::string & sectionNameInFile) const;
    void  createOrAppend( const std::string & );
    char * lastRecordReaderByString( const std::string & fullPath);
    /// method /// algo ///////////////////////////////////////////////////////////////////////
    void Start_PrimeDump_FileSys() const;
    SingleFactor * IntegerDecomposition( const unsigned long dividend);

private:
    /// Data
    unsigned long lastOrdinal;
    unsigned long lastPrime;
    unsigned long desiredOrdinal;
    unsigned long desiredThreshold;// in R+
    unsigned long actualPrimaryFileLength;
    const char * theDumpPath = nullptr;
    bool isHealthlyConstructed = false;
    bool canOperate = false;
    // Riemann exponent s in C; s=:(sigma + i*t).
    double sigma;
    double t;
    /// Ctor
    Primes();// empty Ctor not available; a threshold is required.
    // copying methoda : not usable->private.
    // Copy Constructor
    Primes   ( const Primes & original );
    // operator= only between isomorphic matrixes
    Primes & operator=   ( const Primes & second );
    /// method
    const std::string & tokenEncoder( unsigned long ordinal, unsigned long prime ) const;
    void recoverLastRecord( const char * fromFile);// cannot be const: fills lastOrdinal, lastPrime.
    bool getLastCoupleInDefaultFile();

};// class

}// nmsp

#endif
