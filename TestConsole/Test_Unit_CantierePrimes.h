#ifndef TEST_UNIT_CANTIEREPRIMES_H
#define TEST_UNIT_CANTIEREPRIMES_H

#include "../Entity/PrimesFinder/Cantiere_Primes_2022September01_.h"

class Test_Unit_CantierePrimes
{
    public:
        /** Constructor */
        Test_Unit_CantierePrimes(  unsigned semiAmplitudeOfEachMapSegment );
        /** Default destructor */
        virtual ~Test_Unit_CantierePrimes();

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

        /** Access m_Counter
         * \return The current value of m_Counter
         */
        unsigned int GetCounter() { return m_Counter; }
        /** Set m_Counter
         * \param val New value to set
         */
        void SetCounter(unsigned int val) { m_Counter = val; }

    protected:

    private:
        unsigned long long m_Counter; //!< Member variable "m_Counter"
        Cantiere_Primes_2022September01_::Primes * tested_Class;
};

#endif // TEST_UNIT_CANTIEREPRIMES_H
