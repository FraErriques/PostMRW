#ifndef TEST_UNIT_PRIMES_PRODUCTION_H
#define TEST_UNIT_PRIMES_PRODUCTION_H

#include "../../NumericalAnalysis/PrimesFinder/Primes.h"


class Test_Unit_Primes_Production_
{
    public:
        Test_Unit_Primes_Production_(  unsigned semiAmplitudeOfEachMapSegment );// Ctor
        ~Test_Unit_Primes_Production_();// Dtor

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
        // operating entry point:
        void Primes_Production_managementMethod();


    protected:

    private:
        PrimesFinder::Primes * test_PRODUCTION_;// NB. here we test Entity::PrimesFinder::Primes
};

#endif // TEST_UNIT_PRIMES_PRODUCTION_H
