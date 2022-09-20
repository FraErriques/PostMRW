#ifndef TEST_UNIT_CANTIEREPRIMES_H
#define TEST_UNIT_CANTIEREPRIMES_H

#include "../Entity/PrimesFinder/Cantiere_Primes_2022September01_.h"

class Test_Unit_CantierePrimes
{
    public:
        /** Default constructor */
        Test_Unit_CantierePrimes();
        /** Default destructor */
        virtual ~Test_Unit_CantierePrimes();

        /** call the sequentialDump_Interface
        * \return the outcome of the operation on the sequential dump
        */
        bool sequentialDump( unsigned long long untilThreshol);

        /** call the randomDump_Interface
        * \return the outcome of the operation on the random dump
        */
        bool randomDump( unsigned long long infLeft, unsigned long long maxRight );

        /** read the very last record {Ordinal,Prime} from sequentialDump */
        bool readLastRecord();

        /** read an array of record {Ordinal,Prime} from sequentialDump */
        bool readSequencialDump( int acquireRecordNextToOffset
                                 ,int recArray_seek_START, int recArray_seek_END);

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
        unsigned int m_Counter; //!< Member variable "m_Counter"
        Cantiere_Primes_2022September01_::Primes * tested_Class;
};

#endif // TEST_UNIT_CANTIEREPRIMES_H
