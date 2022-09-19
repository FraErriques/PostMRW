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

bool Test_Unit_CantierePrimes::sequentialDump( unsigned long untilThreshol)
{
    bool testResult = false;// init
    if(nullptr != this->tested_Class)
    {
        testResult = this->tested_Class->SequentialCalcInterface( untilThreshol);
    }// else do nothing.
    // ready.
    return testResult;
}// sequentialDump
