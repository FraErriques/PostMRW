#ifndef CANTIERE_PRIMES_2022SEPTEMBER01__H_INCLUDED
#define CANTIERE_PRIMES_2022SEPTEMBER01__H_INCLUDED

#include <string>
#include <fstream>
#include <math.h>
#include <iostream>
#include <map>
#include <vector>

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
    struct LogIntegralPillarPoint
    {
        unsigned long long threshold;
        unsigned long long logIntegral;
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


	// TODO PRIIVATE removed only in Cantiere, everything has to be accessible. When passing in Produciont, let private again.
    /// Data
    std::map<unsigned long long, unsigned long long> * memoryMappedDump;
    std::vector<LogIntegralPillarPoint> * logIntegralPillars;
//std::map<unsigned long long, unsigned long long>
    bool isHealthlyConstructed = false;
    bool canOperate = false;
    const std::string * sequentialDumpPath = nullptr;// NB. remember to share and delete
    const std::string * randomDumpPath = nullptr;// NB. remember to share and delete
    const std::string * meshRenewalPath = nullptr;// NB. remember to share and delete
    const std::string * localIntegralPath = nullptr;// NB. remember to share and delete
    const std::string * globalIntegralPath = nullptr;// NB. remember to share and delete
    //    std::ifstream has been let an automatic variable.
//    std::ofstream * append_Sequential_Stream;// write
//    std::ofstream * append_Random_Stream;// write
    // Riemann exponent s in C; s=:(sigma + i*t).
    // copying methoda : not usable->private.
    // hide some Constructors
    Primes( const Primes & original );
    Primes();
    // assignement : operator=
    Primes & operator=   ( const Primes & second );
    // construction helper:
    const std::string * feed_sequentialDumpPath(); // non const : feeds a member.
    const std::string * feed_customDumpPath(); // non const : feeds a member.
    const std::string * feed_meshSpecificationPath(); // non const : feeds a member.
    const std::string * feed_localIntegralPath(); // non const : feeds a member.
    const std::string * feed_globalIntegralPath(); // non const : feeds a member.
    const std::string * getConfigSectionContent( const std::string * sectionNameInFile) const;
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
    void coveringIntegral();
// bool distributionFunction(const char * path);  NO MORE
    bool distributionFunction_fromExistingMesh();
    unsigned long long interpolateOrdinal( unsigned long long candidatePrimeThreshold);
    void mapTraverseForward( std::map<unsigned long long, unsigned long long> * mapOfNaturals );
    LogIntegralPillarPoint * getNearestIntegral( unsigned long long candidatePrimeThreshold);
    // follows : IntegerDecomposition : the Fundamental Thm of Arithmetic.
    SingleFactor * IntegerDecomposition( const unsigned long long dividend);

};// class

}// namespace Cantiere_Primes_2022September01_

#endif // CANTIERE_PRIMES_2022SEPTEMBER01__H_INCLUDED

/* cantina

*/
