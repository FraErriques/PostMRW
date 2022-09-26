#ifndef CANTIERE_PRIMES_2022SEPTEMBER01__H_INCLUDED
#define CANTIERE_PRIMES_2022SEPTEMBER01__H_INCLUDED

#include <string>
#include <fstream>
#include <math.h>
#include <iostream>
#include <map>

namespace Cantiere_Primes_2022September01_
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
    Primes();// no more hiding of empty Ctor.
    virtual ~Primes();
    /// method : public interfaces.
	bool SequentialCalcInterface( unsigned long long Threshold );
	bool RandomCalcInterface( unsigned long long infLeft, unsigned long long maxRight );
    bool ReadSequentialDumpInterface_nextRec( long long acquireRecordNextToOffset);
    bool ReadSequentialDumpInterface_arrayOfRec_anywhere( long long recArray_seek_START, long long recArray_seek_END);
    unsigned long long operator[]( unsigned long long desiredOrdinal);

	private:
    /// Data
    std::map<unsigned long long, unsigned long long> * memoryMappedDump;
    bool isHealthlyConstructed = false;
    bool canOperate = false;
    const char * sequentialDumpPath = nullptr;// NB. remember to share and delete[].
    const char * randomDumpPath = nullptr;// NB. remember to share and delete[].
	// methods:{bisection,getToNextRecord,getToPrevRecord,StepThroughNrecordsFFWD,StepThroughNrecordsBKWD} all share the stream.
    std::ifstream * sharedReader;// read
    std::ofstream * append_Sequential_Stream;// write
    std::ofstream * append_Random_Stream;// write
    // Riemann exponent s in C; s=:(sigma + i*t).
    // copying methoda : not usable->private.
    // hide Copy Constructor
    Primes   ( const Primes & original );
    // construction helper:
    //const char * feed_CustomDumpPath(); // non const
    // assignement : operator=
    Primes & operator=   ( const Primes & second );
    const char * feedDumpPath(); // non const
    const char * feed_CustomDumpPath(); // non const
    const char * getPrimeDumpFullPath( const std::string & sectionNameInFile) const;
    void createOrAppend( const std::string & fullPath);
    const char * lastRecordReaderByChar( const std::string & fullPath);
    const char * newDeal_dumpTailReaderByChar( const std::string & fullPath);
    DumpElement * newDeal_recoverLastRecord( const char * dumpTail);
    // newDeal_recoverDumpTail : produce an array of couples {ordinal,prime} from a String : dumpTail_String.
    DumpElement * newDeal_recoverDumpTail( const char * dumpTail_String , int *recordArrayCardinality);
    // newDeal : state of the art.
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
    bool Bisection( unsigned long long requiredOrdinal , unsigned sogliaDistanza );
    // follows : IntegerDecomposition : the Fundamental Thm of Arithmetic.
    SingleFactor * IntegerDecomposition( const unsigned long long dividend);

};// class

}// namespace Cantiere_Primes_2022September01_

#endif // CANTIERE_PRIMES_2022SEPTEMBER01__H_INCLUDED

/* cantina

*/
