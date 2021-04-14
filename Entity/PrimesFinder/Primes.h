
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
    /// Constructors and Destructors
       Primes(// span upper_threshold-lastRecordOrdinal Integers, in default IntegralFile.
              unsigned long upper_threshold
         // ConfigFile for class Primes is "./PrimeConfig.txt"
         // ConfigSectionName for standard-dump is assumed to be "PrimeIntegral_fromOrigin_" and dump-fullpath is in the ConfigSection.
       );
       Primes(); // default Ctor : span 100_Int, from lastRecord, in default IntegralFile.
    Primes(
        unsigned long lower_threshold,
        unsigned long upper_threshold,
        string & desiredConfigSectionName // SectionName in "./PrimeConfig.txt" for the desiderd file
    );
    // db-Ctor
    Primes( string & DbConnectionStringConfigSectionName
            ,string & tableName
            ,unsigned long lower_threshold// only on custom intervals; no default table, on db.
            ,unsigned long upper_threshold
           );



       // Destructor
       ~Primes  ( void );
       // some methoda
       unsigned  getActualLength();
       void LoggerSinkFS_example( unsigned long inf, unsigned long sup) const;
       void IntegralFileFromStartFSproducer( unsigned long sup) const;
       void IntegralFileFromAnywhereFSproducer( unsigned long inf, unsigned long sup) const;
       void dumper();
       unsigned long   operator[]  ( const unsigned long & requiredOrdinal )         const;// it's a read-only utility; syntax: Prime[ordinal]==...

        private:
         ofstream * appendStream = nullptr;// let it a ptr, so to annichilate it, as needed.
         bool isHealthlyConstructed = false;
         unsigned ToBeDivided = 2;// start from two. One is invariant of the operator.
         unsigned threshold;
         unsigned primeOrdinal = 0L; // meglio non fare(after test). exTODO switch to System.Uunsigned everywhere. It leads from 9*10^18 to 18*10^18
         unsigned actualLength = 0L;
         unsigned desiredLength = 0L;
         unsigned actualOrdinal = 0L;
         unsigned desiredOrdinal = 0L;
         unsigned actualPrime = 0L;
         bool canOperate = false;
        // Riemann exponent s in C; s=:(sigma + i*t).
        double sigma;
        double t;
       /// copying methoda : not usable->private.
       // Copy Constructor
       Primes   ( const Primes & original );
       // operator= only between isomorphic matrixes
       Primes & operator=   ( const Primes & second );

   /// algo ////////////////////////////////////////////////////////////////////////////////////////////////////
   /*
   StringMatrix   transpose   ( const StringMatrix & orig   )                    const;
   StringMatrix   operator+   ( const StringMatrix & second )                    const;
   bool           operator==  ( const StringMatrix & second )                    const;
   void           reductor    ( size_t i, size_t j, StringMatrix & smaller )     const;
   /// input-output ////////////////////////////////////////////////////////////////////////////////////////////
   void show   (void)                                                            const;
   bool insert (const std::string & what, const size_t row, const size_t col);   // cannot be const
   */

private:
    const std::string & tokenEncoder( unsigned long ordinal, unsigned long prime ) const;
    //
//   /// data ////////////////////////////////////////////////////////////////////////////////////////////
//   bool readfile  ( const char * where );                                        // cannot be const; reads in RAM
//   bool validate (const char * where)                                            const;
//   bool writefile ( const char * where )                                         const;
};// class

}// nmsp

#endif
