#include "Test_Unit_CantierePrimes.h"
#include "../Common/Config_wrap/Config_wrap.h"

Test_Unit_CantierePrimes::Test_Unit_CantierePrimes(  unsigned semiAmplitudeOfEachMapSegment )
{//ctor
    this->tested_Class = new Cantiere_Primes_2022September01_::Primes( semiAmplitudeOfEachMapSegment);
}//ctor

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

bool Test_Unit_CantierePrimes::dumpTailReaderByChar( )
{// calls: const std::string * dumpTailReaderByChar( const std::string * fullPath);
    bool is_retrivedString_valid = false;
    // default Prime-configuration-file. All in this file.
    Common::ConfigurationService * primeNamedConfig = new Common::ConfigurationService( "./PrimeConfig.txt");
    const std::string * desiredSectionContent = primeNamedConfig->getValue("primeDefaultFile");// ( *sectionNameInFile)
    delete primeNamedConfig;
    const std::string * tail = this->tested_Class->dumpTailReaderByChar( desiredSectionContent);
    delete desiredSectionContent;
    delete tail;
    is_retrivedString_valid = true;// TODO manually verify.
    //
    return is_retrivedString_valid;
}// dumpTailReaderByChar

bool Test_Unit_CantierePrimes::lastRecordReaderByChar( )
{// calls: const std::string * lastRecordReaderByChar( const std::string * fullPath);
    bool is_retrivedString_valid = false;
    // default Prime-configuration-file. All in this file.
    Common::ConfigurationService * primeNamedConfig = new Common::ConfigurationService( "./PrimeConfig.txt");
    const std::string * desiredSectionContent = primeNamedConfig->getValue("primeDefaultFile");// ( *sectionNameInFile)
    delete primeNamedConfig;
    const std::string * tail = this->tested_Class->lastRecordReaderByChar( desiredSectionContent);
    delete desiredSectionContent;
    delete tail;
    is_retrivedString_valid = true;// TODO manually verify.
    //
    return is_retrivedString_valid;
}// lastRecordReaderByChar

bool Test_Unit_CantierePrimes::recoverLastRecord( )
{// calls: DumpElement * recoverLastRecord( const std::string * dumpTail);
    bool is_DumpElement_valid = false;
    // default Prime-configuration-file. All in this file.
    Common::ConfigurationService * primeNamedConfig = new Common::ConfigurationService( "./PrimeConfig.txt");
    const std::string * desiredSectionContent = primeNamedConfig->getValue("primeDefaultFile");// ( *sectionNameInFile)
    delete primeNamedConfig;
    const std::string * tail = this->tested_Class->lastRecordReaderByChar( desiredSectionContent);
    delete desiredSectionContent;
    Cantiere_Primes_2022September01_::Primes::DumpElement * lastRecord =
        this->tested_Class->recoverLastRecord( tail);
    delete tail;
    delete lastRecord;
    is_DumpElement_valid = true;// TODO manually verify.
    //
    return is_DumpElement_valid;
}// recoverLastRecord


bool Test_Unit_CantierePrimes::recoverDumpTail( )
{// calls: DumpElement * recoverDumpTail( const std::string * dumpTail_String , int *recordArrayCardinality);
    bool is_DumpTail_valid = false;
    // default Prime-configuration-file. All in this file.
    Common::ConfigurationService * primeNamedConfig = new Common::ConfigurationService( "./PrimeConfig.txt");
    const std::string * desiredSectionContent = primeNamedConfig->getValue("primeDefaultFile");// ( *sectionNameInFile)
    delete primeNamedConfig;
    const std::string * tail = this->tested_Class->lastRecordReaderByChar( desiredSectionContent);
    delete desiredSectionContent;
    int recordArrayCardinality;
    Cantiere_Primes_2022September01_::Primes::DumpElement * dumpTail =
        this->tested_Class->recoverDumpTail( tail, &recordArrayCardinality);
    delete tail;
    delete[] dumpTail;
    is_DumpTail_valid = true;// TODO manually verify.
    //
    return is_DumpTail_valid;
}// recoverDumpTail


bool Test_Unit_CantierePrimes::acquireNextRecord( )
{// calls: AsinglePointInStream * acquireNextRecord( unsigned long long discriminatingElement_position);
    bool is_DumpTail_valid = false;
    Cantiere_Primes_2022September01_::Primes::AsinglePointInStream * aSingleRecord =
        this->tested_Class->acquireNextRecord( 125); // position in Stream
    delete aSingleRecord;
    is_DumpTail_valid = true;// TODO manually verify.
    //
    return is_DumpTail_valid;
}// acquireNextRecord

bool Test_Unit_CantierePrimes::acquireSequenceOfRecord( )
{// calls: DumpElement * acquireSequenceOfRecord(from,to, &hmRecords)
    bool is_DumpElementArray_valid = false;
    int recordArrayCardinality;
    Cantiere_Primes_2022September01_::Primes::DumpElement * anArrayOfRecord =
        this->tested_Class->acquireSequenceOfRecord( 125, 300, &recordArrayCardinality); // position in Stream
    delete[] anArrayOfRecord;// vectorial i.e. [] deletion of recordArray[]
    is_DumpElementArray_valid = true;// TODO manually verify.
    //
    return is_DumpElementArray_valid;
}// acquireSequenceOfRecord
