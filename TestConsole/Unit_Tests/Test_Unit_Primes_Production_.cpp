#include "Test_Unit_Primes_Production_.h"


Test_Unit_Primes_Production_::Test_Unit_Primes_Production_(  unsigned semiAmplitudeOfEachMapSegment )
{//ctor
    this->test_PRODUCTION_ = new PrimesFinder::Primes( semiAmplitudeOfEachMapSegment);// NB map-amplitude
}//ctor

Test_Unit_Primes_Production_::~Test_Unit_Primes_Production_()
{//dtor
    if( nullptr != this->test_PRODUCTION_)
    {
        delete this->test_PRODUCTION_;
        this->test_PRODUCTION_ = nullptr;
    }// else already deleted.
}//dtor


////    ------Unit Test-----Pimes-------PRODUCTION_----------------------------------------
//----Primes Production management Method-------------------
void Test_Unit_Primes_Production_::Primes_Production_managementMethod()
{// NB amplitude==80 è un pattern di test utile in produzione
    bool p_seq = this->sequentialDump( 9000);
    bool p_rand = this->randomDump( 0, 50);
    bool p_reader = this->readBy_OperatorSquares( +2);// ask Prime[n]
    p_seq &= true;
    p_rand &= true;
    p_reader &= true;
    for (int c=599; c<611; c++)
    {
        p_reader &= this->readBy_OperatorSquares( c);// ask Prime[n]
    }
    std::cout<<"\n\n\n\t the final outcome _PRODUCTION_ is : "<< p_reader<<"\n\n";
    std::cout<<"\n\n\n\t Strike Enter to continue\t";
    getchar();
    p_reader &= this->readSequentialDump_nextRec( 60);
    p_reader &= this->readSequentialDump_arrayOfRec_anywhere(
    23
    ,915 // there must be room for just one record Prime<100.
    );
    for( int c=0; c<61; c++)
    {// next Rec
        p_reader &= this->readSequentialDump_nextRec(c);
    }
    for( int c=0; c<61; c++)
    {// array of Rec
        p_reader &= this->readSequentialDump_arrayOfRec_anywhere(
            c
            ,c+915 // there must be room for just one record Prime<100.
         );
    }// array of Rec
    // ci pensa Destructor : delete test_PRODUCTION_
    //------Unit Test----Primes----PRODUCTION_---------------------------------------------
}//Primes_Production_managementMethod


bool Test_Unit_Primes_Production_::sequentialDump( unsigned long long untilThreshol)
{
    bool testResult = false;// init
    if(nullptr != this->test_PRODUCTION_)
    {
        testResult = this->test_PRODUCTION_->SequentialCalcInterface( untilThreshol);
    }// else do nothing.
    // ready.
    return testResult;
}// sequentialDump

bool Test_Unit_Primes_Production_::randomDump( unsigned long long infLeft, unsigned long long maxRight )
{
    bool testResult = false;// init
    if(nullptr != this->test_PRODUCTION_)
    {
        testResult = this->test_PRODUCTION_->RandomCalcInterface(
            infLeft
            , maxRight
        );
    }// else do nothing.
    // ready.
    return testResult;
}// randomDump

bool Test_Unit_Primes_Production_::readSequentialDump_nextRec( unsigned long long acquireRecordNextToOffset)
{
    bool readSequentialDump_nextRec_outcome =
     this->test_PRODUCTION_->ReadSequentialDumpInterface_nextRec(
         acquireRecordNextToOffset/** read the record next to position acquireRecordNextToOffset */
        );
    if( ! readSequentialDump_nextRec_outcome)
    {
        std::cout<<"\n\t Test_Unit_Primes_Production_::readSequentialDump_nextRec FAILED!";
        std::cout<<"\n\t Offset parameter seems to be incompatible with the stream. \n";
    }
    return readSequentialDump_nextRec_outcome;
}// readSequentialDump_nextRec

bool Test_Unit_Primes_Production_::readSequentialDump_arrayOfRec_anywhere( unsigned long long recArray_seek_START, unsigned long long recArray_seek_END)
{
    bool readSequentialDump_arrayOfRec_anywhere_outcome =
     this->test_PRODUCTION_->ReadSequentialDumpInterface_arrayOfRec_anywhere(
         recArray_seek_START, recArray_seek_END );/** read an array of record {Ordinal,Prime} from sequentialDump */
    if( ! readSequentialDump_arrayOfRec_anywhere_outcome)
    {
        std::cout<<"\n\t Test_Unit_Primes_Production_::readSequentialDump_nextRec FAILED!";
        std::cout<<"\n\t Offset parameter(s) seem to be incompatible with the stream. \n";
    }
    return readSequentialDump_arrayOfRec_anywhere_outcome;
}//readSequentialDump_arrayOfRec_anywhere

bool Test_Unit_Primes_Production_::readBy_OperatorSquares( unsigned long long desiredOrdinal)
{
    bool Test_Unit_CantierePrimes_readBy_OperatorSquares = false;

    Test_Unit_CantierePrimes_readBy_OperatorSquares = (
        0 != (*(this->test_PRODUCTION_)).operator[]( desiredOrdinal)
                                                       );
    return Test_Unit_CantierePrimes_readBy_OperatorSquares;
}// Test_Unit_CantierePrimes::readBy_OperatorSquares
