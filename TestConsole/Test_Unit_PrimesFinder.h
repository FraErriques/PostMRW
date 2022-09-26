#ifndef TEST_UNIT_PRIMESFINDER_H
#define TEST_UNIT_PRIMESFINDER_H

#include "../Entity/PrimesFinder/Primes.h"


class Test_Unit_PrimesFinder
{
    public:
        /** Default constructor */
        Test_Unit_PrimesFinder(  unsigned semiAmplitudeOfEachMapSegment );
        /** Default destructor */
        virtual ~Test_Unit_PrimesFinder();

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

        /** Access m_Counter
         * \return The current value of m_Counter
         */
        unsigned long long Getm_Counter() { return m_Counter; }
        /** Set m_Counter
         * \param val New value to set
         */
        void Setm_Counter(unsigned long long val) { m_Counter = val; }

    protected:

    private:
        unsigned long long m_Counter; //!< Member variable "m_Counter"
        PrimesFinder::Primes * tested_Class;// NB. here we test Entity::PrimesFinder::Primes
};

#endif // TEST_UNIT_PRIMESFINDER_H
