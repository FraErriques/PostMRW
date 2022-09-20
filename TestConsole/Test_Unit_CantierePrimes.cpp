#include "Test_Unit_CantierePrimes.h"

Test_Unit_CantierePrimes::Test_Unit_CantierePrimes()
{//ctor
    this->tested_Class = new Cantiere_Primes_2022September01_::Primes();
}

Test_Unit_CantierePrimes::~Test_Unit_CantierePrimes()
{//dtor
    if( nullptr != this->tested_Class)
    {
        delete this->tested_Class;
        this->tested_Class = nullptr;
    }// else already deleted.
}//dtor

bool Test_Unit_CantierePrimes::sequentialDump( unsigned long long untilThreshol)
{
    bool testResult = false;// init
    if(nullptr != this->tested_Class)
    {
        testResult = this->tested_Class->SequentialCalcInterface( untilThreshol);
    }// else do nothing.
    // ready.
    return testResult;
}// sequentialDump

bool Test_Unit_CantierePrimes::randomDump( unsigned long long infLeft, unsigned long long maxRight )
{
    bool testResult = false;// init
    if(nullptr != this->tested_Class)
    {
        testResult = this->tested_Class->RandomCalcInterface(
            infLeft
            , maxRight
        );
    }// else do nothing.
    // ready.
    return testResult;
}// randomDump


bool Test_Unit_CantierePrimes::readSequencialDump(
                    int acquireRecordNextToOffset
                    ,int recArray_seek_START, int recArray_seek_END
            )
 {
     this->tested_Class->ReadSequentialDumpInterface(
         acquireRecordNextToOffset/** read the very last record {Ordinal,Prime} from sequentialDump */
         ,recArray_seek_START, recArray_seek_END );/** read an array of record {Ordinal,Prime} from sequentialDump */
 }//readSequencialDump
