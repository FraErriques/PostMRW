
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
    //#region Data
    public:
       /// Constructors and Destructors
       Primes(); // default Ctor
       // validate an existing file
       Primes   ( const size_t Prows, const size_t Pcols, const char * where );
       // create in RAM and, if desired, dump on a file. (where==NULL means no dump desired)
       StringMatrix   ( const size_t Prows, const size_t Pcols, bool randomized, const char * where=NULL );
       /// copying methoda
       // Copy Constructor
       Primes   ( const Primes & original );
       // operator= only between isomorphic matrixes
       Primes & operator=   ( const Primes & second );
       // Destructor
       ~Primes  ( void );
       // some methoda
       unsigned  getActualLength();

        private:
         unsigned ToBeDivided = 2;// start from two. One is invariant of the operator.
         fstream  previousShot;// the try-open act will initialize it.
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
   /// data ////////////////////////////////////////////////////////////////////////////////////////////
   const size_t rows, cols;
   std::string ** m;
   /// memory management ///////////////////////////////////////////////////////////////////////////////
   std::string ** allocate   ( void );                                           // cannot be const
   void deallocate ( void );                                                     // cannot be const
   /// input-output ////////////////////////////////////////////////////////////////////////////////////
   void generator ( void );                                                      // cannot be const; generates in RAM
   void input (void);                                                            // cannot be const; reads in RAM
   bool readfile  ( const char * where );                                        // cannot be const; reads in RAM
   bool validate (const char * where)                                            const;
   bool writefile ( const char * where )                                         const;
};// class

}// nmsp

#endif
