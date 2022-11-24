#include "Test_Unit_Primes_Cantiere_.h"
#include "../../Process/Config_wrap/Config_wrap.h"

//Ctor
Test_Unit_CantierePrimes::Test_Unit_CantierePrimes(  unsigned semiAmplitudeOfEachMapSegment )
{// NB amplitude==0 è un pattern di test utile in Cantiere
    this->test_CANTIERE_ = new Cantiere_Primes_2022September01_::Primes( semiAmplitudeOfEachMapSegment);
}//Ctor

Test_Unit_CantierePrimes::~Test_Unit_CantierePrimes()
{//Dtor
    if( nullptr != this->test_CANTIERE_)
    {
        delete this->test_CANTIERE_;
        this->test_CANTIERE_ = nullptr;
    }// else already deleted.
}//Dtor


////    ------Unit Test-----Pimes-------CANTIERE----------------------------------------
//----Primes CANTIERE management Method-------------------
void Test_Unit_CantierePrimes::Primes_Cantiere_managementMethod()
{// NB amplitude==0 è un pattern di test utile in Cantiere
    //    //------Unit Test-----CANTIERE------------------------------------------------
    // the Ctor does : new Test_Unit_CantierePrimes( 0);// NB amplitude==0  ####------########
    bool seq = this->sequentialDump( 99390);// required prime==soglia
    seq &= true;
    bool rand = this->randomDump(  15,  20);
    rand &= true;
    rand = this->randomDump( 900, 920);// i.e. dump Primes on custom interval
    rand = this->randomDump( 1500, 1520);
    // NB. heavy test: un-comment just in case:
    //    rand = this->randomDump( 18446744073709550510, 18446744073709551610);
    //
    bool outcome_dumpTailReaderByChar = this->dumpTailReaderByChar();
    bool outcome_lastRecordReaderByChar = this->lastRecordReaderByChar();
    bool outcome_recoverLastRecord = this->recoverLastRecord();
    bool outcome_recoverDumpTail = this->recoverDumpTail();
    outcome_dumpTailReaderByChar &= true;
    outcome_lastRecordReaderByChar &= true;
    outcome_recoverLastRecord &= true;
    outcome_recoverDumpTail &= true;
    bool reader = true; // used with &=
    reader = this->readSequentialDump_nextRec( 60);
    bool outcome_acquireNextRecord = this->acquireNextRecord();
    bool outcome_acquireSequenceOfRecord = this->acquireSequenceOfRecord();
    outcome_acquireNextRecord &= true;// avoid warnings
    outcome_acquireSequenceOfRecord &= true;
    // DBG size_t ulong_size = sizeof( unsigned long long);
    reader &= this->readBy_OperatorSquares( 99);// ask Prime[n]
    for (int c=1; c<1181; c++)
    {
        reader &= this->readBy_OperatorSquares( c);// ask "n" in Prime[n]
    }
    std::cout<<"\n\n\n\t the final outcome is : "<< reader<<"\n\n";
    getchar();
    //bool reader;
    reader = this->readSequentialDump_nextRec( 60);
    reader &= this->readSequentialDump_arrayOfRec_anywhere(
        0
        ,915 // there must be room for just one record Prime<100.
     );
    for( int c=0; c<61; c++)
    {// next Rec
        reader &= this->readSequentialDump_nextRec(c);
    }
    for( int c=0; c<61; c++)
    {// array of Rec
        reader &= this->readSequentialDump_arrayOfRec_anywhere(
            c
            ,c+915 // there must be room for just one record Prime<100.
         );
    }// array of Rec
    // the Destructor takes care of : delete test_CANTIERE_
    ////    ----END--Unit Test-----CANTIERE---------------------------------------------------
    //massive_UnitTest_caller_Cantiere
}// Primes_Cantiere_managementMethod



bool Test_Unit_CantierePrimes::sequentialDump( unsigned long long untilThreshol)
{
    bool testResult = false;// init
    if(nullptr != this->test_CANTIERE_)
    {
        testResult = this->test_CANTIERE_->SequentialCalcInterface( untilThreshol);
    }// else do nothing.
    // ready.
    return testResult;
}// sequentialDump

bool Test_Unit_CantierePrimes::randomDump( unsigned long long infLeft, unsigned long long maxRight )
{
    bool testResult = false;// init
    if(nullptr != this->test_CANTIERE_)
    {
        testResult = this->test_CANTIERE_->RandomCalcInterface(
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
     this->test_CANTIERE_->ReadSequentialDumpInterface_nextRec(
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
     this->test_CANTIERE_->ReadSequentialDumpInterface_arrayOfRec_anywhere(
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
        0 != (*(this->test_CANTIERE_)).operator[]( desiredOrdinal)
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
    const std::string * tail = this->test_CANTIERE_->dumpTailReaderByChar( desiredSectionContent);
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
    const std::string * tail = this->test_CANTIERE_->lastRecordReaderByChar( desiredSectionContent);
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
    const std::string * tail = this->test_CANTIERE_->lastRecordReaderByChar( desiredSectionContent);
    delete desiredSectionContent;
    Cantiere_Primes_2022September01_::Primes::DumpElement * lastRecord =
        this->test_CANTIERE_->recoverLastRecord( tail);
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
    const std::string * tail = this->test_CANTIERE_->lastRecordReaderByChar( desiredSectionContent);
    delete desiredSectionContent;
    int recordArrayCardinality;
    Cantiere_Primes_2022September01_::Primes::DumpElement * dumpTail =
        this->test_CANTIERE_->recoverDumpTail( tail, &recordArrayCardinality);
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
        this->test_CANTIERE_->acquireNextRecord( 125); // position in Stream
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
        this->test_CANTIERE_->acquireSequenceOfRecord( 125, 300, &recordArrayCardinality); // position in Stream
    delete[] anArrayOfRecord;// vectorial i.e. [] deletion of recordArray[]
    is_DumpElementArray_valid = true;// TODO manually verify.
    //
    return is_DumpElementArray_valid;
}// acquireSequenceOfRecord
