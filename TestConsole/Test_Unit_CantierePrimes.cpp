#include "Test_Unit_CantierePrimes.h"

Test_Unit_CantierePrimes::Test_Unit_CantierePrimes(  unsigned semiAmplitudeOfEachMapSegment )
{//ctor
    this->tested_Class = new Cantiere_Primes_2022September01_::Primes( semiAmplitudeOfEachMapSegment);
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

bool Test_Unit_CantierePrimes::readSequentialDump_nextRec( unsigned long long acquireRecordNextToOffset)
{
    bool readSequentialDump_nextRec_outcome =
     this->tested_Class->ReadSequentialDumpInterface_nextRec(
         acquireRecordNextToOffset/** read the record next to position acquireRecordNextToOffset */
        );
    if( ! readSequentialDump_nextRec_outcome)
    {
        std::cout<<"\n\t Test_Unit_CantierePrimes::readSequentialDump_nextRec FAILED!";
        std::cout<<"\n\t Offset parameter seems to be incompatible with the stream. \n";
    }
    return readSequentialDump_nextRec_outcome;
}// readSequentialDump_nextRec

bool Test_Unit_CantierePrimes::readSequentialDump_arrayOfRec_anywhere( unsigned long long recArray_seek_START, unsigned long long recArray_seek_END)
{
    bool readSequentialDump_arrayOfRec_anywhere_outcome =
     this->tested_Class->ReadSequentialDumpInterface_arrayOfRec_anywhere(
         recArray_seek_START, recArray_seek_END );/** read an array of record {Ordinal,Prime} from sequentialDump */
    if( ! readSequentialDump_arrayOfRec_anywhere_outcome)
    {
        std::cout<<"\n\t Test_Unit_CantierePrimes::readSequentialDump_nextRec FAILED!";
        std::cout<<"\n\t Offset parameter(s) seem to be incompatible with the stream. \n";
    }
    return readSequentialDump_arrayOfRec_anywhere_outcome;
}//readSequentialDump_arrayOfRec_anywhere

bool Test_Unit_CantierePrimes::readBy_OperatorSquares( unsigned long long desiredOrdinal)
{
    bool Test_Unit_CantierePrimes_readBy_OperatorSquares = false;

    Test_Unit_CantierePrimes_readBy_OperatorSquares = (
        0 != (*(this->tested_Class)).operator[]( desiredOrdinal)
                                                       );
    return Test_Unit_CantierePrimes_readBy_OperatorSquares;
}// Test_Unit_CantierePrimes::readBy_OperatorSquares
