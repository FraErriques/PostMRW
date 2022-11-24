#ifndef TEST_UNIT_PRIMES_CANTIERE_H
#define TEST_UNIT_PRIMES_CANTIERE_H

#include "../../NumericalAnalysis/PrimesFinder/Cantiere_Primes_2022September01_.h"

class Test_Unit_CantierePrimes
{
    public:
        /** Constructor */
        Test_Unit_CantierePrimes(  unsigned semiAmplitudeOfEachMapSegment );
        /** Default destructor */
        ~Test_Unit_CantierePrimes();

        /** call the sequentialDump_Interface    */
        bool sequentialDump( unsigned long long untilThreshol);
        /** call the randomDump_Interface       */
        bool randomDump( unsigned long long infLeft, unsigned long long maxRight );
        /** read the scalar LAST record {Ordinal,Prime} from sequentialDump */
        bool readSequentialDump_nextRec( unsigned long long acquireRecordNextToOffset);
        /** read an array of record {Ordinal,Prime} from sequentialDump */
        bool readSequentialDump_arrayOfRec_anywhere(
            unsigned long long recArray_seek_START
            , unsigned long long recArray_seek_END  );
        /** read by means of operator[] */
        bool readBy_OperatorSquares( unsigned long long desiredOrdinal );
        bool dumpTailReaderByChar( );
        bool lastRecordReaderByChar( );
        bool recoverLastRecord( ); // scalar
        bool recoverDumpTail( );   // array
        bool acquireNextRecord( );
        bool acquireSequenceOfRecord( );
        // operating entry point:
        void Primes_Cantiere_managementMethod();

    protected:

    private:
        Cantiere_Primes_2022September01_::Primes * test_CANTIERE_;
};

#endif // TEST_UNIT_PRIMES_CANTIERE_H
