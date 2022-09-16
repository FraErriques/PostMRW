#include <iostream>
//--------------------#include <boost/lambda/lambda.hpp>
//#include "../Common/DbConnectionService/dbCall.h"
//#include "../Common/DbConnectionService/mysql_connection.h"
//#include "../Common/DbConnectionService/DbConnectionService.h"
#include "../Common/StringBuilder/StringBuilder.h"
#include "../Common/Stream/stream_io_.h"
#include "../Process/MonteCarlo_wrap/MonteCarlo_wrap.h"
#include "../Common/MonteCarlo/BaseConverter.h"
#include "../Common/MonteCarlo/ClassicalDiscreteGenerator.h"
#include "../Common/MonteCarlo/ClassicalContinuousGenerator.h"
#include "../Common/Config_wrap/Config_wrap.h"
#include "../Common/Stream/stream_io_.h"
#include "../Entity/Integration/Integration.h"
#include "../Entity/Integration/Integrate.h"
#include "../Entity/PrimesFinder/Primes.h"
#include "../Entity/PrimesFinder/Cantiere_Primes_2022September01_.h"
#include "../Entity/Complex/Complex.h"
#include "../Common/Dictionary/MapOperation.h"

namespace UnderTest
{


namespace internalAlgos
{


// an internal helper, which is the coChain of LogIntegral. Used for ordinal estimates.
double LogIntegral_coChain( double x)
{// an internal helper, which is the coChain of LogIntegral. Used for ordinal estimates.
    return +1.0/log(x);
}// LogIntegral_coChain


unsigned long factorial( unsigned int par)
{
    unsigned long res = +1UL;
    for( int c=par; c>+1; c--)
    {
        res *= c;
    }
    return res;
}// factorial


// TODO:
    void ExpIntegralEi_Ramanujan( double x)//( Complex x)
    {// notes on a convergent series
//
//        N[(+EulerGamma + Log[x]) +
//          Exp[x/2]*Sum[((-1)^(n - 1) (+x)^n)/((n!)*(2^(n - 1)))*
//             Sum[+1/(2*k + 1), {k, 0, Floor[(n - 1)/2]}], {n, +1, +99}]]
//
//
//        N[(+EulerGamma + Log[x]) +
//          Exp[x/2]*Sum[((-1)^(n - 1) (+x)^n)/((n!)*(2^(n - 1)))*
//             Sum[+1/(2*k + 1), {k, 0, Floor[(n - 1)/2]}], {n, +1, +99}]]
//

//        double internalFactor = 0.0;
//        double externalFactor = 0.0;
//        for( int n=+1; n<=+99; n++)
//        {
//            for(int k=0; k<floor(((double)n - 1.0)/2.0); k++)
//            {
//                internalFactor += +1.0/(2.0*(double)k + 1.0);
//            }
//            externalFactor += ((-1.0)^((double)n - 1.0) (+x)^(double)n)/((n!)*(2.0^(n - 1))) * internal_factor;
//            internalFactor = 0.0;// reset.
//        }
//        externalFactor += EulerGamma + Log[x];

        const double EulerGamma = +0.577216;
        double internalFactor = 0.0;
        double externalFactor = 0.0;
        for( int n=+1; n<=+99; n++)
        {
            for(int k=0; k< floor(((double)n - 1.0)/2.0); k++ )
            {
                internalFactor += +1.0/(2.0*(double)k + 1.0);
            }

            externalFactor += pow((-1.0),((double)n - 1.0)) * pow(+x,(double)n) /(factorial(n)* pow(2.0,((double)n - 1)) ) * internalFactor;
            internalFactor = 0.0;// reset.
        }
        externalFactor += EulerGamma + log(x);

  }// Ramanujan series

}// end nmsp internalAlgos

class Primes
{
    public:
    /// Data
    struct SingleFactor
    {
        unsigned long  factorBase;//pi;
        unsigned long  factorMultiplicity;//ai;
    };
    struct DumpElement
    {
        unsigned long  ordinal;
        unsigned long  prime;
    };
     struct AsinglePointInStream
     {
         long Ordinal;
         long Prime;
         long startPositionOfRecord;
         long endPositionOfRecord;
     };


    //~Primes  ( void );
    /// method
//    unsigned long getActualLength();
//    unsigned long getLastOrdinal();
//    unsigned long getLastPrime();
//    // NON const; updates the members {lastOrdinal, lastPrime}.syntax: Prime[ordinal]==...
//    unsigned long operator[] (const unsigned long requiredOrdinal);
//    const char * getPrimeDumpFullPath( const std::string & sectionNameInFile) const;
//    void  createOrAppend( const std::string & );
//    const char * dumpTailReader( const std::string & fullPath);// --- no more used  -------
//    const char * dumpTailReaderByChar( const std::string & fullPath);
//    const char * lastRecordReaderByChar( const std::string & fullPath);
//    SingleFactor * IntegerDecomposition( const unsigned long dividend);
// TODO no more : const char * theDumpPath = nullptr;// NB. remember to share and delete[].

 // TODO let private after test
    /// Data
//    unsigned long lastOrdinal;
//    unsigned long lastPrime;
//    unsigned long desiredOrdinal;
//    unsigned long desiredThreshold;// in R+
//    unsigned long actualPrimaryFileLength;
//    const int tailRecordSize = 60;

//    char * theDumpTailStr = nullptr;// NB. remember to share and delete[]. NB. cannot be CONST.
//    int actualCoupleCardinality = 0;//NB cardinality of dumpTail[]
//    DumpElement * dumpTail = nullptr;// NB. remember to share and delete[] // set up by Ctor

    bool isHealthlyConstructed = false;
    bool canOperate = false;
    // methods:{bisection,getToNextRecord,getToPrevRecord,StepThroughNrecordsFFWD,StepThroughNrecordsBKWD} all share the stream.
    //
    const char * sequentialDumpPath = nullptr;// NB. remember to share and delete[].
    const char * randomDumpPath = nullptr;// NB. remember to share and delete[].
    std::ifstream * sharedReader;// read
    std::ofstream * append_Sequential_Stream;// write
    std::ofstream * append_Random_Stream;// write
    // Riemann exponent s in C; s=:(sigma + i*t).
//    double sigma;
//    double t;
    /// Ctor
    // Primes();// no more hiding of empty Ctor.
    // copying methoda : not usable->private.
    // hide Copy Constructor
    Primes   ( const Primes & original );
    // construction helper:
    //const char * feed_CustomDumpPath(); // non const
    // assignement : operator=
    Primes & operator=   ( const Primes & second );
    /// method
    /// method /// algo ///////////////////////////////////////////////////////////////////////

    // state of the art.
//    void Start_PrimeDump_FileSys(
//                                    unsigned long Left
//                                    ,unsigned long Right
//                                    ,std::ofstream * appendStream
//                                );
//    const char * feedDumpPath(); // non const
//    const std::string & tokenEncoder( unsigned long ordinal, unsigned long prime ) const;
//    void recoverLastRecord( const char * fromFile);// cannot be const: fills lastOrdinal, lastPrime.
//    bool getLastCoupleInDefaultFile();
//    DumpElement * recoverDumpTail( const char * dumpTail);
//    int shiftInSumTissue( const  long requiredOrdinal, const  long initialization, bool wantInitialization );
//    int Bisection( const  long requiredOrdinal, const  long initialization, bool wantInitialization );
    //int currentOperatorSquare( const  long requiredOrdinal, const  long initialization, bool wantInitialization );



// empty Ctor : reads both the sequentialFile and randomFile fullpath
Primes( )
{
    bool dumpPathAcquisitionFromConfig = false;// init to invalid
    //---start sequential_file treatment
    this->feedDumpPath();// SEQUENTIAL : default section, in default file.
    if( nullptr != this->sequentialDumpPath )
    {
        dumpPathAcquisitionFromConfig = true;// from the init=false this is the first reset. Subsequent ones will be &=
    } // else dumpPathAcquisitionFromConfig already false from init.
    else
    {
        dumpPathAcquisitionFromConfig = false;
    }
    //
    //----end of SequentialPath --- start of RandomPath -------
    this->feed_CustomDumpPath();// CUSTOM section, in default file.
    if( nullptr != this->randomDumpPath )
    {
        this->createOrAppend( this->randomDumpPath);
        dumpPathAcquisitionFromConfig &= true;// NB &= only if both files are found, the flag gets true.
    }// else :  not-healthly built:  dumpPathAcquisitionFromConfig already false from init.
    else
    {
        dumpPathAcquisitionFromConfig = false;
    }
    //---check operations' result and document the error for the user.
    if( false==dumpPathAcquisitionFromConfig)// something wrong reading from config files.
    {// not-healthly built.
        this->isHealthlyConstructed = false;
        this->canOperate = false;
        this->append_Sequential_Stream = nullptr;
        this->append_Random_Stream = nullptr;
        this->sharedReader = nullptr;
    }// not-healthly built.
}// empty Ctor : reads both the sequentialFile and randomFile fullpath



bool SequentialCalcInterface( unsigned long Threshold )
{
    bool hasSequentialDumpBeenReset = false;// it's true on filesize<1k and of course on non existing file
    // ---call with params
    const char * stringDumpTail = this->newDeal_dumpTailReaderByChar( this->sequentialDumpPath );// last few records in a string.
    if(nullptr==stringDumpTail)
    {// unable to read the tail of the sequential dump in a string -> reset the file.
        hasSequentialDumpBeenReset = true;
    }// else it has been initialized to false.
    UnderTest::Primes::DumpElement * lastRec = nullptr;// the very last record, deciphered from a func:newDeal_recoverLastRecord
    long LastOrdinal = 0UL;
    long LastPrime = 0UL;
    if(nullptr!=stringDumpTail)
    {// i.e. if we have the string of dump-tail :
        lastRec = this->newDeal_recoverLastRecord( stringDumpTail);
        if(nullptr != lastRec)
        {// the dump tail string content was valid.
            LastOrdinal = lastRec->ordinal;
            LastPrime = lastRec->prime;
        }// else : stringDumpTail is not valid, i.e. sequentialDump needs a reset.
        else
        {// the sequential dump-tail is readable but not valid  -> reset the file.
            hasSequentialDumpBeenReset = true;
        }
        this->append_Sequential_Stream = new std::ofstream( this->sequentialDumpPath, std::fstream::out | std::fstream::app);
        this->sharedReader = new std::ifstream( this->sequentialDumpPath, std::fstream::in);
        this->sharedReader->close();
        delete this->sharedReader;
        this->sharedReader = nullptr;
    }// else sequentialFile not present.
    if (hasSequentialDumpBeenReset)
    {// start a new dump from scratch :
        LastOrdinal = 0;
        LastPrime = 0;
        this->append_Sequential_Stream = new std::ofstream( this->sequentialDumpPath, std::fstream::out);// reset.
        hasSequentialDumpBeenReset = true;
    }
    //---call with appropriate params---------
    this->Start_PrimeDump_FileSys(
                                  LastPrime
                                  , Threshold , append_Sequential_Stream, LastOrdinal );
    this->append_Sequential_Stream->close();
    delete this->append_Sequential_Stream;
    this->append_Sequential_Stream = nullptr;
    delete[] stringDumpTail;
    delete lastRec;
    // ready.
    return hasSequentialDumpBeenReset;
}//

bool ReadSequentialDumpInterface()
{
    bool res = false;
    this->sharedReader = new std::ifstream( this->sequentialDumpPath, std::fstream::in);
    if( nullptr != this->sharedReader)
    {
        res = true;
    }// else stay false.
    //---do the job here----START
    int seek_START = 199;
    int seek_END = 311;
    this->sharedReader->seekg( seek_START , std::ios::beg );
    // test-sigle-record UnderTest::Primes::AsinglePointInStream * nextRecord = this->acquireNextRecord( 50);// pass file-seek-offset.
    int cardinalityOfRecordSequence = 0;
    // next line : test-multirecord
    UnderTest::Primes::DumpElement * recSequence = this->acquireSequenceOfRecord(
        seek_START
        , seek_END
        , &cardinalityOfRecordSequence );
    for( int c=0; c<cardinalityOfRecordSequence; c++)
    {
        std::cout<<"\n\t Record "<<recSequence[c].ordinal<<"_"<<recSequence[c].prime;
    }
    std::cout<<"\n\n";
    //---do the job here----END
    this->sharedReader->close();
    delete this->sharedReader;
    this->sharedReader = nullptr;
    // ready
    return res;
}// ReadSequentialDumpInterface

bool RandomCalcInterface( unsigned long infLeft, unsigned long maxRight )
{
    bool res = false;
    //
    // NB. no {dumpTailReader, recoverLastRecord,...} -> work in [infLeft, maxRight].
    Entity::Integration::FunctionalForm LogIntegral = internalAlgos::LogIntegral_coChain;// function pointer.
    double LogIntegral_ofInfPar = Entity::Integration::trapezi( +2.0, (double)infLeft, ((double)infLeft-2.0)*4, LogIntegral );
    unsigned long extimatedOrdinal= (unsigned long)LogIntegral_ofInfPar;//TODO stima !
    //this->lastPrime = infLeft;//##### the first integer analyzed will be infLeft+1; the last will be "maxRight" parameter.##
    //this->desiredThreshold = maxRight;
    // write a stamp, about what we're doing and when.
    time_t ttime = time(0);
    char* dt = ctime(&ttime);
    //   NB. for UTC Greenwich tm *gmt_time = gmtime(&ttime);
    //   NB. for UTC Greenwich dt = asctime(gmt_time);
    //
    this->append_Random_Stream = new std::ofstream( this->randomDumpPath , std::fstream::out | std::fstream::app);
    *(this->append_Random_Stream) << "\n\n Custom Interval ("<<infLeft<<", "<<maxRight<<"] ,worked on: "<<dt; //test<<"\n";
    *(this->append_Random_Stream) << " Ordinals are extimated by LogIntegral; so the ordinal appears usually bigger than the correct one.\n";
    // ---call with params
    Start_PrimeDump_FileSys( infLeft, maxRight, this->append_Random_Stream, extimatedOrdinal );
    //
    this->append_Random_Stream->close();
    delete this->append_Random_Stream;
    this->append_Random_Stream = nullptr;
    return res;
}// RandomCalcInterface





const char * feedDumpPath() // non const
{// default section, in default file.
    if( nullptr==this->sequentialDumpPath )
    {
        this->sequentialDumpPath = this->getPrimeDumpFullPath( "primeDefaultFile");// Default Section Name.
    }//else ready.
    return this->sequentialDumpPath;
}// feedDumpPath()

const char * feed_CustomDumpPath() // non const
{// custom section, in default file.
    if( nullptr==this->randomDumpPath )
    {
        this->randomDumpPath = this->getPrimeDumpFullPath( "primeCustomFile");// CUSTOM Section Name, for non complete dumping.
    }//else ready.
    return this->randomDumpPath;
}// feed_CustomDumpPath()

const char * getPrimeDumpFullPath( const std::string & sectionNameInFile) const
{
    const char *  res = nullptr;
    Common::ConfigurationService * primeNamedConfig = new Common::ConfigurationService( "./PrimeConfig.txt");// default Prime-configuration-file. All configurations for  in this file.
    const std::string * desiredSectionContent = primeNamedConfig->getValue( sectionNameInFile);// configSectionNames can be added.
    res = desiredSectionContent->c_str();
    delete primeNamedConfig;
//delete desiredSectionContent;  TODO study how to delete it.
    return res;// caller has to delete.
}// getPrimeDumpFullPath

    /// Dtor()
    ~Primes()
    {/// Dtor() : closes the append_handle.
        if( nullptr != this->sequentialDumpPath )
        {
            delete[] this->sequentialDumpPath;
            this->sequentialDumpPath = nullptr;
        }
        if( nullptr != this->randomDumpPath )
        {
            delete[] this->randomDumpPath;
            this->randomDumpPath = nullptr;
        }
//        if( nullptr != this->theDumpTailStr ) TODO buggy : let it local and delete after usage
//        {
//            delete[] this->theDumpTailStr;
//            this->theDumpTailStr = nullptr;
//        }
//        if( nullptr != this->dumpTail )
//        {
//            delete[] this->dumpTail;
//            this->dumpTail = nullptr;
//        }
        /*
        if( nullptr != this->appendStream)  no more a global class::variable.
        {
            this->appendStream->close();
            this->appendStream = nullptr;
        }// else already closed.
        */
    }// Dtor(





//bool getLastCoupleInDefaultFile()
//{
//    bool res = false;// init to invalid.
//    this->feedDumpPath();
//    if( nullptr != this->sequentialDumpPath )
//    {
//        this->createOrAppend( this->sequentialDumpPath);
//    }// else : TODO not-healthly built.
//    else {return res;}// which is still "false".
//    this->dumpTailReaderByChar( this->sequentialDumpPath);
//    if( nullptr != this->theDumpTailStr)
//    {
//        recoverLastRecord( this->theDumpTailStr);// members should be in place, now: lastOrdinal, lastPrime.
//    }// else : no valid last record : start from zero!
//    else {return res;}// which is still "false".
//    // ready:
//    res = true;// all ok.
//    return res;
//}// getLastCoupleInDefaultFile







void createOrAppend( const std::string & fullPath)
{
    ofstream createOrApp(fullPath, std::fstream::out | std::fstream::app);
    createOrApp.close();
}// dumpFile createOrAppend



/*   no more used
const char * dumpTailReader( const std::string & fullPath)
{
    if(nullptr!=this->theDumpTailStr)
    {
        return this->theDumpTailStr;
    }// else build it.
    ifstream lastrecReader(fullPath, std::fstream::in );
    lastrecReader.seekg( 0, lastrecReader.end);
    int streamSize = lastrecReader.tellg();
    int lastTokenHypothesizedLength;// f(streamSize).
    // cannot use runtime-expressions in switch-case: case-labels have to be compile-time constants.
    if( streamSize<4)
        {// no valid last-record in place.
            this->lastOrdinal=0UL;
            this->lastPrime=+1UL;//##### +2 first of the Primes p in P in N. this->lastPrime gets a ++ before start.
            return nullptr;// NB ##### do NOT call recoverLastRecord on a null ptr !!! ####################
        }
    else if( streamSize>=4 && streamSize<=10)
        {
            lastTokenHypothesizedLength = streamSize;
        }
    else if( streamSize>10 && streamSize<=200)
        {
            lastTokenHypothesizedLength = 10;
        }
    else// i.e. case >200
        {
            lastTokenHypothesizedLength = this->tailRecordSize;// 60 is suitable for primes in magnitude-order of 10^9.
        }// end of if( streamsize..)->seek( howMuch, end).
    //
    int multeplicity = 5;// less two times backwards, from EOF.
    lastrecReader.seekg( -1*multeplicity*lastTokenHypothesizedLength, lastrecReader.end);// seek(-size,end)=:goBack(size,fromEOF).
    this->theDumpTailStr = new char[multeplicity*lastTokenHypothesizedLength+1];// TODO test
    lastrecReader.read( this->theDumpTailStr, multeplicity*lastTokenHypothesizedLength);// fill this->theDumpTailStr from the stream-tail.
    this->theDumpTailStr[multeplicity*lastTokenHypothesizedLength]=0;//terminate.
    lastrecReader.close();
    // ready.
    return this->theDumpTailStr;
}// dumpTailReader
*/

//
const char * lastRecordReaderByChar( const std::string & fullPath)
{
    char * directTailDump = new char[120];// tune it.
    ifstream lastrecReader(fullPath, std::fstream::in );
    lastrecReader.seekg( 0, lastrecReader.end);
    int streamSize = lastrecReader.tellg();// filesize
    if( 1024>streamSize)
    {
        this->append_Sequential_Stream = new std::ofstream( this->sequentialDumpPath, std::fstream::out );// NO  | std::fstream::app rewrite from scratch.
        sprintf( directTailDump, "1_2");// means start from sratch.
        this->append_Sequential_Stream->close();
        delete this->append_Sequential_Stream;
        this->append_Sequential_Stream = nullptr;
        return directTailDump;
    }// else continue.
    Common::StringBuilder * sb = new Common::StringBuilder( 100);// forecasted size.
    lastrecReader.seekg( -100, lastrecReader.end);// get in place to read last char[100].
    int currentPosition;
    int step = 0;
    int howManyLineEndings = 0;
    for( char c=0; ! lastrecReader.eof(); )
    {
        lastrecReader.get( c);
        sb->append(c);
        step++;
        currentPosition = lastrecReader.tellg();
    }// for
    std::string bufferedReverseTail = sb->str();
    int bufferedReverseTail_len = bufferedReverseTail.length();
    const char * const_directTailDump = sb->str().c_str();
    directTailDump = (char *)const_directTailDump;
    // delete sb; ?really?
    // ready
    return directTailDump;// caller has to delete
}// lastRecordReaderByChar

const char * newDeal_dumpTailReaderByChar( const std::string & fullPath)
{
    const char * sequentialFile_tail = new char[101];// caller has to delete
    ifstream lastrecReader(fullPath, std::fstream::in );
    lastrecReader.seekg( -1, ios::end ); // lastrecReader.end);
    int streamSize = lastrecReader.tellg();
    if( 100>streamSize)// for such small data, it's better to create a new sequeltial file from scratch.
    {
        return nullptr;
    }// else continue.
    lastrecReader.seekg( -100, std::ifstream::end );
    Common::StringBuilder * sb = new Common::StringBuilder( 101);// forecasted size.
    for( char c=0; lastrecReader.get(c); )
    {
        sb->append(c);
    }
    sequentialFile_tail = (char *)(sb->str().c_str());// caller has to delete!
    lastrecReader.close();
    // ready.
    return sequentialFile_tail;// caller has to delete
}// newDeal_dumpTailReaderByChar()


//const char * dumpTailReaderByChar( const std::string & fullPath)
//{
//    if(nullptr!=this->theDumpTailStr)
//    {
//        return this->theDumpTailStr;
//    }// else build it.
//    ifstream lastrecReader(fullPath, std::fstream::in );
//    lastrecReader.seekg( 0, lastrecReader.end);
//    int streamSize = lastrecReader.tellg();
//    int lastTokenHypothesizedLength;// f(streamSize).
//    // cannot use runtime-expressions in switch-case: case-labels have to be compile-time constants.
//    if( streamSize<4)
//        {// no valid last-record in place.
//            this->lastOrdinal=0UL;
//            this->lastPrime=+1UL;//##### +2 first of the Primes p in P in N. this->lastPrime gets a ++ before start.
//            return nullptr;// NB ##### do NOT call recoverLastRecord on a null ptr !!! ####################
//        }
//    else if( streamSize>=4 && streamSize<=10)
//        {
//            lastTokenHypothesizedLength = streamSize;
//        }
//    else if( streamSize>10 && streamSize<=200)
//        {
//            lastTokenHypothesizedLength = 10;
//        }
//    else// i.e. case >200
//        {
//            lastTokenHypothesizedLength = this->tailRecordSize;// 60 is suitable for primes in magnitude-order of 10^9.
//        }// end of if( streamsize..)->seek( howMuch, end).
//    //
//    int multeplicity = 5;// less "multeplicity" times backwards, from EOF.
//// avoid it:lastrecReader.seekg( -1*multeplicity*lastTokenHypothesizedLength, lastrecReader.end);// seek(-size,end)=:goBack(size,fromEOF).
//    lastrecReader.seekg( streamSize-multeplicity*lastTokenHypothesizedLength, lastrecReader.beg );
////this->theDumpTailStr = new char[multeplicity*lastTokenHypothesizedLength+1];// TODO test
////Substituted lastrecReader.read( this->theDumpTailStr, multeplicity*lastTokenHypothesizedLength);// fill this->theDumpTailStr from the stream-tail.
//    Common::StringBuilder * sb = new Common::StringBuilder(multeplicity*lastTokenHypothesizedLength+1);// forecasted size.
//    for( char c=0; lastrecReader.get(c); )
//    {
//        sb->append(c);
//    }
//    this->theDumpTailStr = (char *)(sb->str().c_str());// caller has to delete!
//    lastrecReader.close();
//    // ready.
//    return this->theDumpTailStr;
//}//dumpTailReaderByChar()

//
//unsigned long getLastOrdinal()
//{
//    this->feedDumpPath();// default section, in default file.
//    if( nullptr == this->sequentialDumpPath)
//    {
//        this->isHealthlyConstructed = false;
//        this->canOperate = false;
//        throw;
//    }// else :  healthly built: continue:
//    this->dumpTailReaderByChar( this->sequentialDumpPath);
//    if( nullptr != this->theDumpTailStr)
//    {
//        recoverLastRecord( this->theDumpTailStr);// members should be in place, now: lastOrdinal, lastPrime.
//    }// else : no valid last record : start from zero!
//    else
//    {
//        this->lastOrdinal = 0;
//        this->lastPrime = 0;
//    }
//    return this->lastOrdinal;
//}//getLastOrdinal



//unsigned long getLastPrime()
//{// default section, in default file.
//    this->feedDumpPath();
//    if( nullptr == this->sequentialDumpPath)
//    {
//        this->isHealthlyConstructed = false;
//        this->canOperate = false;
//        throw;
//    }// else :  healthly built: continue:
//    this->dumpTailReaderByChar( this->sequentialDumpPath);
//    if( nullptr != this->theDumpTailStr)
//    {
//        recoverLastRecord( this->theDumpTailStr);// members should be in place, now: lastOrdinal, lastPrime.
//    }// else : no valid last record : start from zero!
//    else
//    {
//        this->lastOrdinal = 0;
//        this->lastPrime = 0;
//    }
//    return this->lastPrime;
//}//getLastPrime



 unsigned strlen_loc( char * par)
 {
     int res = 0;
     if(nullptr==par) {return res;}
     for( ; par[res]!=0; res++);
     return res;
 }




std::string * exadecimalConverterTo( unsigned int par )
{
    const std::string exaFigures("0123456789ABCDEF");
    Common::StringBuilder sb(20);
    sb.append("0x");//start with Hex prefix.
    int abacusLeftMostPosition = log(par)/log(16);
    double dividendo = (double)par;
    for( ;abacusLeftMostPosition>=0; abacusLeftMostPosition--)
    {
        double quotient = dividendo/ pow(+16.0,(double)abacusLeftMostPosition);
        double resto = dividendo - ((int)quotient)*pow(+16.0,(double)abacusLeftMostPosition);
        // update
        sb.append( exaFigures[(int)quotient]);
        dividendo = resto;
    }
    //ready.
    std::string * res = new std::string( sb.str());
    return res;// caller has to delete.
}

/*  IntegerDecomposition : the Fundamental Thm of Arithmetic.
SingleFactor * IntegerDecomposition( const unsigned long dividend)
{
    Entity::Integration::FunctionalForm LogIntegral = internalAlgos::LogIntegral_coChain;// function pointer.
    double LogIntegral_ofInfPar = Entity::Integration::trapezi( +2.0, (double)dividend, ((double)dividend-2.0)*4, LogIntegral );
    unsigned long ordinaleStimato = (unsigned long)LogIntegral_ofInfPar;// approx eccesso: LogIntegral[Soglia]==LastOrdinal_under_Soglia==Cardinalita[sottoSoglia].
    SingleFactor * factorization = new SingleFactor[ordinaleStimato];// stimare #fattoriMaximal.
    // Oss. greatest involved-prime==dividend/2 in a composite, since greatestFactor is the cofactor of the PotentialSmallest(i.e. 2).
    for(int c=0; c<ordinaleStimato; c++)
    {// init to zeroContentMemory.
        factorization[c].factorBase = 0;
        factorization[c].factorMultiplicity = 0;
    }
    // TODO readRange( 1, ordinaleStimato);
    unsigned long * involvedPrimes = new unsigned long[ordinaleStimato];
    for(int c=0; c<ordinaleStimato; c++)
    {
        involvedPrimes[c] = (*this)[c+1];//NB. Prime[1]==2 , Prime[0]==error.
    }// end filling up the candidate prime-factor array.
    unsigned long dividendo, divisore;
    dividendo = dividend;
    double realQuotient;
    unsigned long intQuotient;
    int i=0;// start from +2. indice nel vettore dei primi.
    int acc=0;// indice nel vettore dei risultati.
    divisore=involvedPrimes[i];
    bool lastDivisionWasDiophantine =  false;
    // #### start factorization loop ######################################################################################
    for(  ; +1<dividendo; )
    {// dividendo will be substituted by Quotient, until dividendo==+1.
        realQuotient = (double)dividendo/(double)divisore;
        intQuotient = dividendo/divisore;
        if( realQuotient-intQuotient <+1.0E-80 )// ####### ramo lastDivisionWasDiophantine ##
        {// divisione diofantea : the prime acting as divisor is a factor (i.e. divides dividendo).
            if(  lastDivisionWasDiophantine)
            {//NB. factorization[acc].pi is already correct: do nothing on current factor "pi"
            }//if(  lastDivisionWasDiophantine)
            else if( ! lastDivisionWasDiophantine)
            {// a new divisor gets promoted.
                factorization[acc].factorBase = divisore;// promote current prime and its exponent.
            }// No other else.
            // NB. Actions in common btw curDivDiophantine:
            factorization[acc].factorMultiplicity++;// increment the exponent of this factor: i.e. pi^ai
            lastDivisionWasDiophantine =  true;// that's something we need to track.
            dividendo = intQuotient;// NB. swap the dividendo, after a successful Diophantine-ratio.
        }// if // divisione diofantea : the prime acting as divisor is a factor (i.e. divides dividendo).
        else// ### no Diophantine division ##
        {// ### no Diophantine division ##
            if(  lastDivisionWasDiophantine)
            {// use next divisor slot, in the results array.
                acc++;// next factor slot, in the results array.
            }
            else if( ! lastDivisionWasDiophantine)
            {// NO acc++ we don't have an idoneous factor, yet.
            }// No other else.
            // NB. Actions in common btw lastDivisionWasDiophantine when curDivNOTDiophantine
            i++;// test next prime, as factor.
            lastDivisionWasDiophantine =  false;
            divisore=involvedPrimes[i];// NB. to next prime, if cur one works no more
        }// else// ### no Diophantine division ##
    }// #### start factorization loop ######################################################################################
    //..
    delete[] involvedPrimes;// no more use of them.
    // NB. swap the prudentially oversized array, in a fit-size one.
    SingleFactor * factorization_srk_ = new SingleFactor[acc+2];// the #divisors, counting from zero + a termination_record, zero filled.
    for(int c=0; c<acc+2; c++)
    {
        factorization_srk_[c] = factorization[c];
    }
    if( factorization[acc+1].factorBase != 0) {throw;}// check on the nullity of last record. It's a placeholder.
    delete[] factorization;// NB. delete the prudentially oversized array, after copying it, in a fit-size one.
    // ready.
    return factorization_srk_;// NB. the caller has to delete.
}// IntegerDecomposition : the Fundamental Thm of Arithmetic.
*/

UnderTest::Primes::DumpElement * newDeal_recoverLastRecord( const char * dumpTail)
{
    UnderTest::Primes::DumpElement * lastRecord = new UnderTest::Primes::DumpElement();
    std::string parFromFile(dumpTail);
    int inputParamLength = parFromFile.length();
    std::string filteredLastToken("");
    for( int c=0; c<inputParamLength;c++)
    {
        if( parFromFile[c]>=48 && parFromFile[c]<=57 )// is digit
        {
            filteredLastToken.append( 1, parFromFile[c] );
        }
        else// not digit
        {// this measure affects both '\r' and '\n' and all the non-digits.
            filteredLastToken.append( 1, '_' );// substitute every non-digit with '_'
        }
    }//for: preserve only digits and substitute everything else with underscore. Then split on underscore.
    std::vector<std::string> * tokenArray = Common::StrManipul::stringSplit("_", filteredLastToken.c_str(), true );// remove empty entries.
    int i=+1;// the choice of a reverse_iterator is due the need of reading just lastRecord.
    for( std::vector<std::string>::reverse_iterator it=tokenArray->rbegin(); it!=tokenArray->rend(); ++it, i++)
    {// preserve only the last two semi-records: they are lastOrdinal and LastPrime.
        if( 1==i)// the reverse order lets the first entry be a prime, and the second an ordinal.
        {// last Prime
            lastRecord->prime = Common::StrManipul::stringToUnsignedLong(*it);
        }
        else if( 2==i)
        {// last ordinal
            lastRecord->ordinal = Common::StrManipul::stringToUnsignedLong(*it);
        }// else the token is not interesting; it was included in the read portion, but is not part of the last row.So skip when i>2
    }// the two interesting semi-tokens are the last two; so the reading is in reverse order.
    // ready.
    return lastRecord;// caller has to delete.
}// newDeal_recoverLastRecord

//void  recoverLastRecord( const char * dumpTail)
//{
//    std::string parFromFile(dumpTail);
//    int inputParamLength = parFromFile.length();
//    std::string filteredLastToken("");
//    for( int c=0; c<inputParamLength;c++)
//    {
//        if( parFromFile[c]>=48 && parFromFile[c]<=57 )// is digit
//        {
//            filteredLastToken.append( 1, parFromFile[c] );
//        }
//        else// not digit
//        {
//            filteredLastToken.append( 1, '_' );// subst. with '_'
//        }
//    }//for: preserve only digits and substitute everything else with underscore. Then split on underscore.
//    std::vector<std::string> * tokenArray = Common::StrManipul::stringSplit("_", filteredLastToken.c_str(), true );// remove empty entries.
//    int i=+1;
//    for( std::vector<std::string>::reverse_iterator it=tokenArray->rbegin(); it!=tokenArray->rend(); ++it, i++)
//    {// preserve only the last two semi-records: they are lastOrdinal and LastPrime.
//        if( 1==i)
//        {// last Prime
//            this->lastPrime = Common::StrManipul::stringToUnsignedLong(*it);
//        }
//        else if( 2==i)
//        {// last ordinal
//            this->lastOrdinal = Common::StrManipul::stringToUnsignedLong(*it);
//        }// else the token is not interesting; it was included in the read portion, but is not part of the last row.
//    }// the two interesting semi-tokens are the last two; so the reading is in reverse order.
//}//recoverLastRecord


//// produce an array of couples {ordinal,prime} from the dumpTail.
//DumpElement * recoverDumpTail( const char * dumpTail)
//{
//    if( nullptr != this->dumpTail)
//    {// do not build again.
//        return this->dumpTail;
//    }// else build it; it will be a shared dataMember and Dtor will delete.
//    std::string parFromFile(dumpTail);
//    int inputParamLength = parFromFile.length();
//    std::string filteredLastToken("");
//    for( int c=0; c<inputParamLength;c++)
//    {
//        if( parFromFile[c]>=48 && parFromFile[c]<=57 )// is digit
//        {
//            filteredLastToken.append( 1, parFromFile[c] );
//        }
//        else// not digit
//        {
//            filteredLastToken.append( 1, '_' );// subst. with '_'
//        }
//    }//for: preserve only digits and substitute everything else with underscore. Then split on underscore.
//    std::vector<std::string> * tokenArray = Common::StrManipul::stringSplit("_", filteredLastToken.c_str(), true );// remove empty entries.
//    int entryCardinality = tokenArray->size();
//// already init this->actualCoupleCardinality = 0;//NB: tagliare al massimo dei minoranti pari
//    // NB: stabilire se pari
//    if( (double)entryCardinality/2.0 - entryCardinality/2 <+1E-80 )
//    {// parita'
//        this->actualCoupleCardinality = entryCardinality/2;
//    }
//    else
//    {// DISparita'
//        this->actualCoupleCardinality = (entryCardinality-1)/2;// NB: tagliare al massimo dei minoranti pari
//    }
//    // NB: allocare per tale misura
//    this->dumpTail = new DumpElement[actualCoupleCardinality];// member struct-array {ordinal,prime}.
//    // NB: fill-up reverse
//    int currentCouple=actualCoupleCardinality-1;
//    for( std::vector<std::string>::reverse_iterator it=tokenArray->rbegin();
//     it!=tokenArray->rend() && currentCouple>=0;
//       currentCouple--) // no more increment on the iterator
//    {// preserve the last complete-records: they have to be {Ordinal,Prime}. Use index-parity for this task:
//        // get a Prime from tailEnd, coming back:
//        this->dumpTail[currentCouple].prime =  Common::StrManipul::stringToUnsignedLong(*(it++));
//        // get an ordinal from tailEnd, coming back:
//        this->dumpTail[currentCouple].ordinal =  Common::StrManipul::stringToUnsignedLong(*(it++));// goto next record backwards.
//    }// the interesting semi-tokens are the ones of complete records; so the reading is in reverse order, excluding a partial record, if present.
//    delete tokenArray;
//    //ready.
//    return this->dumpTail;// caller has to delete.
//}//recoverDumpTail



// newDeal_recoverDumpTail : produce an array of couples {ordinal,prime} from a String : dumpTail_String.
DumpElement * newDeal_recoverDumpTail( const char * dumpTail_String , int *recordArrayCardinality)
{
    DumpElement * dumpTail_Records = nullptr;// ret val
    std::string parFromFile( dumpTail_String );
    int inputParamLength = parFromFile.length();
    std::string filteredLastToken("");
    for( int c=0; c<inputParamLength;c++)
    {
        if( parFromFile[c]>=48 && parFromFile[c]<=57 )// is digit
        {
            filteredLastToken.append( 1, parFromFile[c] );
        }
        else// not digit
        {
            filteredLastToken.append( 1, '_' );// subst. with '_'
        }
    }//for: preserve only digits and substitute everything else with underscore. Then split on underscore.
    std::vector<std::string> * tokenArray = Common::StrManipul::stringSplit("_", filteredLastToken.c_str(), true );// remove empty entries.
    int entryCardinality = tokenArray->size();
    int actualCoupleCardinality = 0;// init and then assign as follows:
    // NB. actualCoupleCardinality : tagliare al massimo dei minoranti pari
    // NB: stabilire se pari
    if( (double)entryCardinality/2.0 - entryCardinality/2 <+1E-80 )
    {// parita'
        actualCoupleCardinality = entryCardinality/2;
    }
    else
    {// DISparita'
        actualCoupleCardinality = (entryCardinality-1)/2;// NB: tagliare al massimo dei minoranti pari
    }// then feed "out"-param :
    *recordArrayCardinality = actualCoupleCardinality;
    // NB: allocare per tale misura : it's the return value: caller has to delete[].
    dumpTail_Records = new DumpElement[actualCoupleCardinality];// struct-array {ordinal,prime}.
    // NB: fill-up reverse
    int currentCouple=actualCoupleCardinality-1;
    for( std::vector<std::string>::reverse_iterator it=tokenArray->rbegin();
     it!=tokenArray->rend() && currentCouple>=0;
       currentCouple--) // no more increment on the iterator
    {// preserve the last complete-records: they have to be {Ordinal,Prime}. Use index-parity for this task:
        // get a Prime from tailEnd, coming back:
        dumpTail_Records[currentCouple].prime =  Common::StrManipul::stringToUnsignedLong(*(it++));
        // get an ordinal from tailEnd, coming back:
        dumpTail_Records[currentCouple].ordinal =  Common::StrManipul::stringToUnsignedLong(*(it++));// goto next record backwards.
    }// the interesting semi-tokens are the ones of complete records; so the reading is in reverse order, excluding a partial record, if present.
    delete tokenArray;
    //ready.
    return dumpTail_Records;// caller has to delete.
}// newDeal_recoverDumpTail


// newDeal : state of the art.
void Start_PrimeDump_FileSys(
        unsigned long Left
        ,unsigned long Right
        ,std::ofstream * appendStream
        ,unsigned long ordinal // passed as real xor extimated ordinal of "Left" i.e. Left==Prime[ordinal]
    )
{
    //unsigned long ordinal = this->lastOrdinal;// next Prime to be found, will increase the ordinal.TODO: decide whether to increment the member.
    bool isStillPrime = true;
    double realQuotient;
    unsigned long intQuotient;
    //unsigned long cursor = this->lastPrime+1UL;// start stepping from the Int after the last found Prime.
    unsigned long cursor = Left+1UL;// start stepping from the Int after the last found Prime.
    if( cursor<+2){cursor=+2;}// 1 (i.e. one) is the product-invariant; so, not a prime.
    // NB now a data-member ; ofstream appendStream( this->sequentialDumpPath, std::fstream::out | std::fstream::app);
    //
    for( ; cursor<=Right; cursor++)//NB. cursor==dividend.
    {
        double soglia = sqrt( cursor);// division is a two-operand operator: the bisection of dividend is Sqrt[dividend]
        // when dividend/Sqrt[dividend]==Sqrt[dividend] and when dividend/(Sqrt[dividend]+eps)<Sqrt[dividend]
        // so the stepping into divisor>Sqrt[dividend] leads to divisors<Sqrt[dividend] which have already been explored.
        unsigned long divisor=+2;
        for( ; divisor<=soglia; divisor++)
        {
            realQuotient = (double)cursor/(double)divisor;
            intQuotient = cursor/divisor;
            if( realQuotient-intQuotient <+1.0E-80 )
            {// divisione diofantea
                isStillPrime = false;// NB. #################
                break;// NB. #################
            }// else  continue searching for primality.
        }// the internal for : the one from [+2, cursor]
        // if after all idoneous divisors..
        if( isStillPrime)
        {
            ++ordinal;//another one foud, starting from zero, so that Prime[1]=2
            std::string * ordinalStr = Common::StrManipul::uLongToString(ordinal);
            std::string * primeStr = Common::StrManipul::uLongToString( cursor );
            int forecastedTokenSize = ordinalStr->length()+primeStr->length()+3;//3 stands for '_'+'\n'+'\r'
            Common::StringBuilder * strBuild = new Common::StringBuilder( forecastedTokenSize);
            strBuild->append(ordinalStr->c_str());
            strBuild->append("_");
            strBuild->append(primeStr->c_str());
            strBuild->append("\r");// choose one btw '\r' or '\n'
            delete ordinalStr;
            delete primeStr;
            // instead of returning it, dump it on the file.
            appendStream->write( strBuild->str().c_str(), strBuild->str().length() );
            delete strBuild;// clean up the token-buffer.
            strBuild = nullptr;
        }// else ripristino del flag-primalita' per il candidato divisore successivo.
        else
        {// ripristino della primalita', dopo un composto(i.e. non primo).
            isStillPrime = true;
        }// ripristino della primalita', dopo un composto(i.e. non primo).
    }// external for : the one where cursor cicles from inf to sup, on dividends.
// NO MORE : interface does it : appendStream.close();
    // ready.
}// newDeal IntegralFileFromStartFSproducer




UnderTest::Primes::AsinglePointInStream * acquireNextRecord( unsigned long discriminatingElement_position)
{
    if( nullptr==this->sharedReader || ! this->sharedReader->is_open())
    {
        return nullptr;
    }// else continue
    AsinglePointInStream * nextRecord = new Primes::AsinglePointInStream();
    int c=0;
    int stepDone = 0;
    for( ; ; stepDone++ )
    {// for step until next-record starting point.
        c = this->sharedReader->get();
        if( 13==c || 10==c )
        {
            break;
        }// next record begin-position is set
        if( this->sharedReader->eof() ) {break;}
    }// for step until next-record starting point.
    // then write down next complete record :
    Common::StringBuilder sb(50);// estimate
    int terminator=0;
    int midRecord_separator=0;
    for( stepDone = 0;  ; stepDone++ )
    {
        c = this->sharedReader->get();
        if(13==c || 10==c)
        {
            terminator++;
        }
        else if( '_'==c)
        {
             midRecord_separator++;
             sb.append(c);
        }
        else if( c>=48 && c<=57 )// is digit
        {
            sb.append( c);
        }
        else// not digit
        {// this should never occur, except for errors in the dump.
            sb.append( (int)'_');
        }
        if(1==midRecord_separator && terminator>=1)
        {
            const std::string nextRecord_txt = sb.str();
            int nextRecord_len = nextRecord_txt.length();
            int recordCardinality=0;
            Primes::DumpElement * nextRecPtr =
                newDeal_recoverDumpTail( nextRecord_txt.c_str() , &recordCardinality);
            nextRecord->startPositionOfRecord = discriminatingElement_position;
            nextRecord->endPositionOfRecord = discriminatingElement_position + stepDone;
            nextRecord->Ordinal = nextRecPtr->ordinal;
            nextRecord->Prime = nextRecPtr->prime;
            // TODO delete nextRecPtr
            break;
        }
    }// for step into next complete record
    //ready
    return nextRecord;// caller has to delete.
}// acquireNextRecord


UnderTest::Primes::DumpElement * acquireSequenceOfRecord(
    unsigned long discriminatingElement_position
    , unsigned long until_position
    , int * howMany_RecordInSequence
                                        )
{
    if( nullptr==this->sharedReader || ! this->sharedReader->is_open())
    {
        return nullptr;
    }// else continue
    DumpElement * sequenceRecord = new Primes::DumpElement();
    int c=0;
    int stepDone = 0;
    for( ; ; stepDone++ )
    {// for step until next-record starting point.
        c = this->sharedReader->get();
        if( 13==c || 10==c )
        {
            break;
        }// next record begin-position is set
        if( this->sharedReader->eof() ) {break;}
    }// for step until next-record starting point. Steps done until here cannot be counted, since they are before next record.
    // then write down next complete record :
    Common::StringBuilder sb(until_position-discriminatingElement_position);// estimate
    int terminator=0;
    int midRecord_separator=0;
    for( stepDone = 0;  ; stepDone++ )// start counting steps from here:
    {
        c = this->sharedReader->get();
        if(13==c || 10==c)
        {
            terminator++;
            sb.append('#'); // intra-Record Separator eg. 1_2#2_3#
        }
        else if( '_'==c)
        {
             midRecord_separator++;
             sb.append(c);
        }
        else if( c>=48 && c<=57 )// is digit
        {
            sb.append( c);
        }
        else// not digit
        {// this should never occur, except for errors in the dump.
            sb.append( (int)'_');
        }
        if( discriminatingElement_position + stepDone >= until_position // TODO test : reached endpoint
            && (13==c || 10==c)// as additional condition, step until the end of current record
           )
        {
            const std::string sequenceOfRecord_txt = sb.str();
            int sequenceOfRecord_len = sequenceOfRecord_txt.length();
            // howMany_RecordInSequence is an out parameter, coming from the caller of this func
            sequenceRecord =
                newDeal_recoverDumpTail( sequenceOfRecord_txt.c_str() , howMany_RecordInSequence );
            // TODO delete nextRecPtr
            break;
        }
    }// for step into next complete record
    //ready
    return sequenceRecord;// caller has to delete.
}// acquireSequenceOfRecord


/*
 Bisection( requiredOrdinal)
 {
    for ()
    {
        filesize = tellg
        discriminatingElement_position = filesize/2.0;
        DumpElement * nextRecord = acquireNextRecord( discriminatingElement_position)
        // compare
        if( nextRecord->ordinal < requiredOrdinal)
        {
            left = acquireNextRecord_end;
            right = filesize;
        }
        else if( nextRecord->ordinal > requiredOrdinal)
        {
            left = 0;
            right = acquireNextRecord_start;
        }
        else if( nextRecord->ordinal == requiredOrdinal)
        {
            found -> exit (i.e. break)
        }
    }// for
 }
*/

};// class Primes

}// namespace UnderTest




//---entry point-------------------------
int main()
{
    Common::LogWrappers::SectionOpen("main", 0);
    const std::string customFileConfigSectionName( "primeCustomFile");
    // UnderTest:: NB.
    UnderTest::Primes *p = new UnderTest::Primes();
    bool res = p->SequentialCalcInterface( 5200);
    bool readRes = p->ReadSequentialDumpInterface();

//    res = p->RandomCalcInterface(
//       20
//       ,30 );

//    res = p->SequentialCalcInterface( 11200);
//    const char * stringDumpTail = p->newDeal_dumpTailReaderByChar( p->sequentialDumpPath );
//    //
//    int recordArray_cardinality=0;
//    UnderTest::Primes::DumpElement * recordArray = p->newDeal_recoverDumpTail( stringDumpTail , &recordArray_cardinality );
//    if(nullptr != recordArray)
//    {
//        for(int c=0; c<recordArray_cardinality; c++) // array's cardinality is an out-param.
//        {
//            std::cout<<"\n\t Record "<< recordArray[c].ordinal<<"_"<<recordArray[c].prime;
//        }// for
//    }// else NO recordArray available.
//    //
//    UnderTest::Primes::DumpElement * lastRec = nullptr;
//    long LastOrdinal = 0UL;//lastRec->ordinal; // p->getLastOrdinal();
//    long LastPrime = 0UL;//  lastRec->prime;// p->getLastPrime();
//    if(nullptr!=stringDumpTail)
//    {
//        lastRec = p->newDeal_recoverLastRecord( stringDumpTail);
//        if(nullptr!=lastRec)
//        {
//            LastOrdinal =lastRec->ordinal;
//            LastPrime =lastRec->prime;
//        }// else the string stringDumpTail is invalid -> no record has been recovered.
//    }// else sequentialFile not present.  TODO
//    // p->recoverLastRecord( p->theDumpTailStr);
//    // UnderTest::Primes::DumpElement * lastRecord = p->recoverDumpTail( nullptr );// ???
//    // delete[] lastRecord;
//    //
//    res = p->RandomCalcInterface(
//       999900
//       ,999999 );



//    res = p->SequentialCalcInterface( 200);
//    LastOrdinal = p->getLastOrdinal();
//    LastPrime = p->getLastPrime();

//    res = p->SequentialCalcInterface(  50);
//    res = p->SequentialCalcInterface( 100);
//    //
//    long desPrime = (*p)[25];
//    std::string logMsg = "desPrime = (*p)[25]==";
//    std::string * desinenza = Common::StrManipul::uLongToString( desPrime);
//    logMsg += *desinenza;
//    Common::LogWrappers::SectionContent( logMsg.c_str(), 0 );
//    delete desinenza;
//    //
//    desPrime = (*p)[3];
//    logMsg = "desPrime = (*p)[3]==";
//    desinenza = Common::StrManipul::uLongToString( desPrime);
//    logMsg += *desinenza;
//    Common::LogWrappers::SectionContent( logMsg.c_str(), 0 );
//    delete desinenza;
    //-----
    delete p;
    Common::LogWrappers::SectionClose();


    //---ready---------------
    std::cout<<"\n\n\n\t Strike Enter to leave\t";
    getchar();
    return 0;
}// main





/* --------------cantina----------------------------
//    tryReadForewards();
//    tryReadBackwards();
    // system("pwd");
    // system("dir"); Process_cur_dir: Directory di C:\root\projects\GitHubSandBox\PostMRW\TestConsole
*/
