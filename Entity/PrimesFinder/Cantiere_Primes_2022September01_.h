#ifndef CANTIERE_PRIMES_2022SEPTEMBER01__H_INCLUDED
#define CANTIERE_PRIMES_2022SEPTEMBER01__H_INCLUDED

#include <string>
#include <fstream>
#include <math.h>
#include <iostream>

namespace Cantiere_Primes_2022September01_
{


class Primes
{
    public:
    /// Data
    struct SingleFactor
    {
        unsigned long  factorBase;//pi;
        unsigned long  factorMultiplicity;//ai;
    };
    struct DumpElement
    {
        unsigned long  ordinal;
        unsigned long  prime;
    };
     struct AsinglePointInStream
     {
         long Ordinal;
         long Prime;
         long positionByte;
     };

    /// Ctor
    // ---start new deal
    Primes();
    bool SequentialCalcInterface( unsigned long Threshold);
    bool RandomCalcInterface( unsigned long Left, unsigned long Right);
    // ---end new deal
    ~Primes  ( void );
    /// method
    unsigned long getActualLength();
    unsigned long getLastOrdinal();
    unsigned long getLastPrime();
    // NON const; updates the members {lastOrdinal, lastPrime}.syntax: Prime[ordinal]==...
    unsigned long operator[] (const unsigned long requiredOrdinal);
    const char * getPrimeDumpFullPath( const std::string & sectionNameInFile) const;
    void  createOrAppend( const std::string & );
    const char * dumpTailReader( const std::string & fullPath);
    const char * dumpTailReaderByChar( const std::string & fullPath);
    SingleFactor * IntegerDecomposition( const unsigned long dividend);
// TODO no more : const char * theDumpPath = nullptr;// NB. remember to share and delete[].

private: // TODO let private after test
    /// Data
    unsigned long lastOrdinal;
    unsigned long lastPrime;
    unsigned long desiredOrdinal;
    unsigned long desiredThreshold;// in R+
    unsigned long actualPrimaryFileLength;
    const int tailRecordSize = 60;
    unsigned long secureRightBound = actualPrimaryFileLength - this->tailRecordSize;
    char * theDumpTailStr = nullptr;// NB. remember to share and delete[]. NB. cannot be CONST.
    int actualCoupleCardinality = 0;//NB cardinality of dumpTail[]
    DumpElement * dumpTail = nullptr;// NB. remember to share and delete[] // set up by Ctor

    bool isHealthlyConstructed = false;
    bool canOperate = false;
    // methods:{bisection,getToNextRecord,getToPrevRecord,StepThroughNrecordsFFWD,StepThroughNrecordsBKWD} all share the stream.
    //
    const char * sequentialDumpPath = nullptr;// NB. remember to share and delete[].
    const char * randomDumpPath = nullptr;// NB. remember to share and delete[].
    std::ifstream * sharedReader;// read
    std::ofstream * append_Sequential_Stream;// write
    std::ofstream * append_Random_Stream;// write
    // Riemann exponent s in C; s=:(sigma + i*t).
    double sigma;
    double t;
    /// Ctor
    // Primes();// no more hiding of empty Ctor.
    // copying methoda : not usable->private.
    // hide Copy Constructor
    Primes   ( const Primes & original );
    // construction helper:
    const char * feed_CustomDumpPath(); // non const
    // assignement : operator=
    Primes & operator=   ( const Primes & second );
    /// method
    /// method /// algo ///////////////////////////////////////////////////////////////////////
    void Start_PrimeDump_FileSys(
                                    unsigned long Left
                                    ,unsigned long Right
                                    ,std::ofstream * appendStream
                                );
    const char * feedDumpPath(); // non const
    const std::string & tokenEncoder( unsigned long ordinal, unsigned long prime ) const;
    void recoverLastRecord( const char * fromFile);// cannot be const: fills lastOrdinal, lastPrime.
    bool getLastCoupleInDefaultFile();
    DumpElement * recoverDumpTail( const char * dumpTail);
    int shiftInSumTissue( const  long requiredOrdinal, const  long initialization, bool wantInitialization );
    int Bisection( const  long requiredOrdinal, const  long initialization, bool wantInitialization );
    int NpartSection( const  long requiredOrdinal, const  long initialization, bool wantInitialization );
    int currentOperatorSquare( const  long requiredOrdinal, const  long initialization, bool wantInitialization );
    AsinglePointInStream readRecordAt(std::ifstream & dumpReader, long offsetFromBeg);// lettore di Record da IntegralDump

};// class

}// namespace Cantiere_Primes_2022September01_

#endif // CANTIERE_PRIMES_2022SEPTEMBER01__H_INCLUDED

/* cantina


    void Start_PrimeDump_FileSys() const;
    Primes(unsigned long threshold);// default Section, on default config-file, dedicated for this class: i.e. ./PrimeConfig.txt
    // below: a Ctor to span (inf, max] :
    Primes(unsigned long infLeft, unsigned long maxRight, const std::string& desiredConfigSectionName);// CUSTOM Section, on default config-file.



*/
