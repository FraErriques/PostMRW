
/**
 *  \class  Numerics::Primes
 *  \brief  a Analytic Number Theory class
 *  \author F. Erriques
 *  \since  2021#march#31 CET 2021
 * this version contained a buggy dump-reader
 * the correction arrives in September 2022
 */


#ifndef __Primes_production__H_INCLUDED
#define __Primes_production__H_INCLUDED

#include <string>
#include <fstream>
#include <math.h>
#include <iostream>
#include <map>

namespace PrimesFinder
{

class Primes
{
    public:
    /// Type
    struct SingleFactor
    {
        unsigned long long factorBase;//pi;
        unsigned long long factorMultiplicity;//ai;
    };
    struct DumpElement
    {
        unsigned long long ordinal;
        unsigned long long prime;
    };
     struct AsinglePointInStream
     {
         unsigned long long Ordinal;
         unsigned long long Prime;
         unsigned long long startPositionOfRecord;
         unsigned long long endPositionOfRecord;
     };
    /// Ctor
    Primes( unsigned semiAmplitudeOfEachMapSegment );
    virtual ~Primes();
    /// method : public interfaces.
	bool SequentialCalcInterface( unsigned long long Threshold );
	bool RandomCalcInterface( unsigned long long infLeft, unsigned long long maxRight );
    bool ReadSequentialDumpInterface_nextRec( long long acquireRecordNextToOffset);
    bool ReadSequentialDumpInterface_arrayOfRec_anywhere( long long recArray_seek_START, long long recArray_seek_END);
    unsigned long long operator[]( unsigned long long desiredOrdinal);
    long long sequentialStreamSize();
    // TODO let it private, with properties
    unsigned sogliaDistanza;


	private:
    /// Data
    std::map<unsigned long long, unsigned long long> * memoryMappedDump;
    bool isHealthlyConstructed = false;
    bool canOperate = false;
    const std::string * sequentialDumpPath = nullptr;// NB. remember to share and delete
    const std::string * randomDumpPath = nullptr;// NB. remember to share and delete
    //    std::ifstream has been let an automatic variable.
    std::ofstream * append_Sequential_Stream;// write
    std::ofstream * append_Random_Stream;// write
    // Riemann exponent s in C; s=:(sigma + i*t).
    // copying methoda : not usable->private.
    // hide some Constructors
    Primes( const Primes & original );
    Primes();
    // assignement : operator=
    Primes & operator=   ( const Primes & second );
    // construction helper:
    const std::string * feedDumpPath(); // non const
    const std::string * feed_CustomDumpPath(); // non const
    const std::string * getPrimeDumpFullPath( const std::string * sectionNameInFile) const;
    void createOrAppend( const std::string * fullPath);
    const std::string * lastRecordReaderByChar( const std::string * fullPath);
    const std::string * dumpTailReaderByChar( const std::string * fullPath);
    DumpElement * recoverLastRecord( const std::string * dumpTail);
    // recoverDumpTail : produce an array of couples {ordinal,prime} from a String : dumpTail_String.
    DumpElement * recoverDumpTail( const std::string * dumpTail_String , int *recordArrayCardinality);
    //  state of the art.
    void Start_PrimeDump_FileSys(
            unsigned long long Left
            ,unsigned long long Right
            ,std::ofstream * appendStream
            ,unsigned long long ordinal // passed as real xor extimated ordinal of "Left" i.e. Left==Prime[ordinal]
        );
    AsinglePointInStream * acquireNextRecord( unsigned long long discriminatingElement_position);
    DumpElement * acquireSequenceOfRecord(
        unsigned long long discriminatingElement_position
        , unsigned long long until_position
        , int * howMany_RecordInSequence
                                            );
    bool MoveToMap(
        long long      discriminatingElement_position
        , long long    until_position
        , int *                 howMany_RecordInSequence
                   );
    unsigned long long queryMap( unsigned long long desiredOrdinal);
    bool Bisection( unsigned long long requiredOrdinal );
    // follows : IntegerDecomposition : the Fundamental Thm of Arithmetic.
    SingleFactor * IntegerDecomposition( const unsigned long long dividend);

};// class

}// nmsp

#endif // __Primes_production__H_INCLUDED
