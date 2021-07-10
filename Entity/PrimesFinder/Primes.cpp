#include <iostream>
#include <string>
#include <math.h>
#include <ctime>
#include "Primes.h"
#include "../../Common/Config_wrap/Config_wrap.h"
#include "../../Common/StringBuilder/StringBuilder.h"
#include "../../Common/LogFs_wrap/LogFs_wrap.h"
#include "../../Entity/Integration/Integration.h"


    /*
    this Construction path is devoted to log the results on the default IntegralFile; the one that starts from origin(i.e. +2).
    Another Ctor will be provided, to log on a partial-File, which consists in a custom analysis, in [min, max]. For such
    Ctor the params will be Ctor( min, max, desiredConfigSectionName)
    */
    PrimesFinder::Primes::Primes(unsigned long threshold)
    {// default section, in default file.
        this->feedDumpPath();
        if( nullptr != this->theDumpPath)
        {
            this->createOrAppend( this->theDumpPath);
            this->dumpTailReaderByChar( this->theDumpPath);
            if( nullptr != this->theDumpTailStr)
            {
                recoverLastRecord( this->theDumpTailStr);// members should be in place, now: lastOrdinal, lastPrime.
            }// else : no valid last record : start from zero!
            this->desiredThreshold = threshold;// set the upper bound for research, in R+.
        }
        else// else : not-healthly built.
        {// else : not-healthly built.
            this->isHealthlyConstructed = false;
            this->canOperate = false;
        }// else : not-healthly built.
    }// Ctor


const char * PrimesFinder::Primes::feedDumpPath() // non const
{// default section, in default file.
    if( nullptr==this->theDumpPath)
    {
        this->theDumpPath = this->getPrimeDumpFullPath( "primeDefaultFile");// Default Section Name.
    }//else ready.
    return this->theDumpPath;
}// feedDumpPath()

const char * PrimesFinder::Primes::feed_CustomDumpPath() // non const
{// custom section, in default file.
    if( nullptr==this->customDumpPath)
    {
        this->customDumpPath = this->getPrimeDumpFullPath( "primeCustomFile");// CUSTOM Section Name, for non complete dumping.
    }//else ready.
    return this->customDumpPath;
}// feed_CustomDumpPath()


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
//        /*
//        N[(+EulerGamma + Log[x]) +
//          Exp[x/2]*Sum[((-1)^(n - 1) (+x)^n)/((n!)*(2^(n - 1)))*
//             Sum[+1/(2*k + 1), {k, 0, Floor[(n - 1)/2]}], {n, +1, +99}]]
//        */

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



    /*
    This Construction path is devoted to log the results on a CUSTOM IntegralFile.
    This Ctor is devoted to log on a partial-File, which consists in a custom analysis, in (infLeft, maxRight]. For such
    Ctor the params are: Ctor( infLeft, maxRight, desiredConfigSectionName).
    */
    PrimesFinder::Primes::Primes(unsigned long infLeft, unsigned long maxRight, const std::string& desiredConfigSectionName)
    {// CUSTOM section, in default file.
        this->feed_CustomDumpPath();
        if( nullptr != this->customDumpPath)
        {
            this->createOrAppend( this->customDumpPath);
            // NB. no {dumpTailReader, recoverLastRecord,...} -> work in [infLeft, maxRight].
            Entity::Integration::FunctionalForm LogIntegral = internalAlgos::LogIntegral_coChain;// function pointer.
            double LogIntegral_ofInfPar = Entity::Integration::trapezi( +2.0, (double)infLeft, ((double)infLeft-2.0)*4, LogIntegral );
            this->lastOrdinal= (unsigned long)LogIntegral_ofInfPar;//TODO stima !
            this->lastPrime = infLeft;//##### the first integer analyzed will be infLeft+1; the last will be "maxRight" parameter.##
            this->desiredThreshold = maxRight;
            // write a stamp, about what we're doing and when.
            time_t ttime = time(0);
            char* dt = ctime(&ttime);
            //tm *gmt_time = gmtime(&ttime);  NB. for UTC Greenwich
            //dt = asctime(gmt_time);
            ofstream stampWriter( this->customDumpPath, std::fstream::out | std::fstream::app);
            stampWriter<<"\n\n Custom Interval ("<<infLeft<<", "<<maxRight<<"] ,worked on: "<<dt<<"\n";
            stampWriter.close();
        }// else :  not-healthly built.
        else// else : not-healthly built.
        {// else : not-healthly built.
            this->isHealthlyConstructed = false;
            this->canOperate = false;
        }// else : not-healthly built.
    }// Ctor



    /// Dtor()
    PrimesFinder::Primes::~Primes()
    {/// Dtor() : closes the append_handle.
        if( nullptr != this->theDumpPath)
        {
            delete[] this->theDumpPath;
            this->theDumpPath = nullptr;
        }
        if( nullptr != this->customDumpPath)
        {
            delete[] this->customDumpPath;
            this->customDumpPath = nullptr;
        }
        if( nullptr != this->theDumpTailStr )
        {
            delete[] this->theDumpTailStr;
            this->theDumpTailStr = nullptr;
        }
        if( nullptr != this->dumpTail )
        {
            delete[] this->dumpTail;
            this->dumpTail = nullptr;
        }
        /*
        if( nullptr != this->appendStream)  no more a global class::variable.
        {
            this->appendStream->close();
            this->appendStream = nullptr;
        }// else already closed.
        */
    }// Dtor(


bool PrimesFinder::Primes::getLastCoupleInDefaultFile()
{
    bool res = false;// init to invalid.
    this->feedDumpPath();
    if( nullptr != this->theDumpPath)
    {
        this->createOrAppend( this->theDumpPath);
    }// else : TODO not-healthly built.
    else {return res;}// which is still "false".
    this->dumpTailReaderByChar( this->theDumpPath);
    if( nullptr != this->theDumpTailStr)
    {
        recoverLastRecord( this->theDumpTailStr);// members should be in place, now: lastOrdinal, lastPrime.
    }// else : no valid last record : start from zero!
    else {return res;}// which is still "false".
    // ready:
    res = true;// all ok.
    return res;
}// getLastCoupleInDefaultFile


const char * PrimesFinder::Primes::getPrimeDumpFullPath( const std::string & sectionNameInFile) const
{
    const char *  res = nullptr;
    Common::ConfigurationService * primeNamedConfig = new Common::ConfigurationService( "./PrimeConfig.txt");// default Prime-configuration-file. All configurations for Primes:: in this file.
    const std::string * desiredSectionContent = primeNamedConfig->getValue( sectionNameInFile);// configSectionNames can be added.
    res = desiredSectionContent->c_str();
    delete primeNamedConfig;
//delete desiredSectionContent;  TODO study how to delete it.
    return res;// caller has to delete.
}// getPrimeDumpFullPath





void PrimesFinder::Primes::createOrAppend( const std::string & fullPath)
{
    ofstream createOrApp(fullPath, std::fstream::out | std::fstream::app);
    createOrApp.close();
}// dumpFile createOrAppend




const char * PrimesFinder::Primes::dumpTailReader( const std::string & fullPath)
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
    int multeplicity = 2;// less two times backwards, from EOF.
    lastrecReader.seekg( -1*multeplicity*lastTokenHypothesizedLength, lastrecReader.end);// seek(-size,end)=:goBack(size,fromEOF).
    this->theDumpTailStr = new char[multeplicity*lastTokenHypothesizedLength+1];// TODO test
    lastrecReader.read( this->theDumpTailStr, multeplicity*lastTokenHypothesizedLength);// fill this->theDumpTailStr from the stream-tail.
    this->theDumpTailStr[multeplicity*lastTokenHypothesizedLength]=0;//terminate.
    lastrecReader.close();
    // ready.
    return this->theDumpTailStr;
}// dumpTailReader


const char * PrimesFinder::Primes::dumpTailReaderByChar( const std::string & fullPath)
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
    int multeplicity = 4;// less "multeplicity" times backwards, from EOF.
// avoid it:lastrecReader.seekg( -1*multeplicity*lastTokenHypothesizedLength, lastrecReader.end);// seek(-size,end)=:goBack(size,fromEOF).
    lastrecReader.seekg( streamSize-multeplicity*lastTokenHypothesizedLength, lastrecReader.beg );
//this->theDumpTailStr = new char[multeplicity*lastTokenHypothesizedLength+1];// TODO test
//Substituted lastrecReader.read( this->theDumpTailStr, multeplicity*lastTokenHypothesizedLength);// fill this->theDumpTailStr from the stream-tail.
    Common::StringBuilder * sb = new Common::StringBuilder(multeplicity*lastTokenHypothesizedLength+1);// forecasted size.
    for( char c=0; lastrecReader.get(c); )
    {
        sb->append(c);
    }
    this->theDumpTailStr = (char *)(sb->str().c_str());// caller has to delete!
    lastrecReader.close();
    // ready.
    return this->theDumpTailStr;
}//dumpTailReaderByChar()


unsigned long PrimesFinder::Primes::getActualLength()
{
    this->feedDumpPath();
    if( nullptr == this->theDumpPath)
    {
        this->isHealthlyConstructed = false;
        this->canOperate = false;
        throw;
    }// else :  healthly built: continue:
    std::ifstream dumpReader(  this->theDumpPath, std::fstream::in );// read-only.
    dumpReader.seekg( 0, dumpReader.end);
    this->actualPrimaryFileLength = dumpReader.tellg();
    this->secureRightBound = this->actualPrimaryFileLength - this->tailRecordSize;// refresh it now!
    // ready.
    return this->actualPrimaryFileLength;
}//getActualLength

unsigned long PrimesFinder::Primes::getLastOrdinal()
{
    this->feedDumpPath();// default section, in default file.
    if( nullptr == this->theDumpPath)
    {
        this->isHealthlyConstructed = false;
        this->canOperate = false;
        throw;
    }// else :  healthly built: continue:
    this->dumpTailReaderByChar( this->theDumpPath);
    if( nullptr != this->theDumpTailStr)
    {
        recoverLastRecord( this->theDumpTailStr);// members should be in place, now: lastOrdinal, lastPrime.
    }// else : no valid last record : start from zero!
    else
    {
        this->lastOrdinal = 0;
        this->lastPrime = 0;
    }
    return this->lastOrdinal;
}//getLastOrdinal

unsigned long PrimesFinder::Primes::getLastPrime()
{// default section, in default file.
    this->feedDumpPath();
    if( nullptr == this->theDumpPath)
    {
        this->isHealthlyConstructed = false;
        this->canOperate = false;
        throw;
    }// else :  healthly built: continue:
    this->dumpTailReaderByChar( this->theDumpPath);
    if( nullptr != this->theDumpTailStr)
    {
        recoverLastRecord( this->theDumpTailStr);// members should be in place, now: lastOrdinal, lastPrime.
    }// else : no valid last record : start from zero!
    else
    {
        this->lastOrdinal = 0;
        this->lastPrime = 0;
    }
    return this->lastPrime;
}//getLastPrime



 unsigned strlen_loc( char * par)
 {
     int res = 0;
     if(nullptr==par) {return res;}
     for( ; par[res]!=0; res++);
     return res;
 }


 int PrimesFinder::Primes::shiftInSumTissue( const  long requiredOrdinal, const  long initialization, bool wantInitialization )
 {
     long DeltaTessutoSomma;// NB. have to be signed, cause of signedDelta.
     double DeltaTessutoProdotto;
     struct AsinglePointInStream
     {
         long Ordinal;
         long Prime;
         long positionByte;
     };
     AsinglePointInStream beg, decoded, last;
     long LandingPoint;
     // init   beg
     beg.Ordinal = +1;
     beg.Prime = +2;
     beg.positionByte = 0;
     // init  last
     last.Ordinal = +100;// case study
     last.Prime = +541;
     last.positionByte = +600;//per eccesso
     // init
     if( ! wantInitialization)
     {
         LandingPoint = (long)(  (double)requiredOrdinal/(double)last.Ordinal * (double)last.positionByte  );
     }
     else
     {
         LandingPoint = initialization;
     }
     // init   decoded
     decoded.Ordinal = LandingPoint * +1/+1.91; // [Dim]==[ordinal]
     //Hypothesis: position==f(ordinal)==+1.91*ordinal
     decoded.positionByte = +1.91 * LandingPoint;
     decoded.Prime = 0;
     // ####
     int acc=0;// accumulator of steps, needed to converge.
     for( ; requiredOrdinal!=decoded.Ordinal; acc++)
     {
         DeltaTessutoSomma = requiredOrdinal - decoded.Ordinal;
         DeltaTessutoProdotto = (double)DeltaTessutoSomma/(double)(last.positionByte);
         LandingPoint =  decoded.positionByte + DeltaTessutoProdotto*last.positionByte;// [Dim]==[position]
         // simulation
         decoded.positionByte = LandingPoint;
         decoded.Ordinal = LandingPoint * +1/+1.91; // [Dim]==[ordinal]
     }// for
     //###
     //ready.
     return acc;
 }// PropostaBisezione


 PrimesFinder::Primes::AsinglePointInStream  PrimesFinder::Primes::readRecordAt(std::ifstream & dumpReader, long offsetFromBeg)
 {
     AsinglePointInStream res;
    this->getLastCoupleInDefaultFile();// this call writes into members: {lastOrdinal, lastPrime}.
    long target = offsetFromBeg;
    const int tokenSize = this->tailRecordSize;// globally defined.
    char partialToken[tokenSize];
    char secondToken[tokenSize];

        if( secureRightBound<target)// required a landing-point, after the secureRightBound
        {
//            this->dumpTailReaderByChar( this->theDumpPath);
//            this->recoverDumpTail( this->theDumpTailStr);
//            for(int c=0; ; c++)
//            {// scan the dumpTailArray
//                if( c>this->actualCoupleCardinality-1)
//                {
//                    throw "element not found in file tail.";
//                }
//                if( requiredOrdinal==dumpTail[c].ordinal)
//                {
//                    decodedOrdinal = dumpTail[c].ordinal;// exit condition
//                    requiredPrime = dumpTail[c].prime;
//                    dumpReader.close();// TODO evaluate if leave open for ReadRange()
//                    return requiredPrime;// NB. break is not enough!
//                }// else continue.
//            }// scan the dumpTailArray
        }// required a landing-point, after the secureRightBound
        else
        {// NB. only seek to safe locations(i.e. <=secureRightBound) otherwise the flag-family isBad,isEOF will throw something.
            dumpReader.seekg( target, dumpReader.beg);// GOTO required %.##################################### crucial action #####
        }// after having landed, evaluate:
        // first Token has to be thrown away, since it is likely to be truncated before the beginning, due to
        // random access to seek(bisection); next record will be complete.
        dumpReader.getline( partialToken, tokenSize, '\r' );
        dumpReader.getline( secondToken, tokenSize, '\r' );// read the whole line, until newline.
        if(0==target)//if the landing-point is the beginning of stream, then the useful token is the first one, since there's no previous one.
        {// it's needed only when searching for the first record in the dump, since it has no previous record.
            for( int c=0; c<tokenSize; c++)
            {// mustSwapTokens
                secondToken[c] = partialToken[c];
            }// mustSwapTokens
        }// no else; when searching for records different from the absolute first, there's no need for this swap.
        int partialToken_Length = strlen_loc( partialToken);
        int secondToken_Length = strlen_loc( secondToken);
        int totalReadTokenLength = partialToken_Length+secondToken_Length+2;// +the two '\r' that are descarded.
        /* in case of need to Debug the stream seeking.
        //## functions to check state flags:
        bool isGood = dumpReader.good();
        bool isEOF = dumpReader.eof();
        bool isFailure = dumpReader.fail();
        bool isBad = dumpReader.bad();
        bool isRdState = dumpReader.rdstate();
        if( !isGood
            ||isEOF
            ||isFailure
            ||isBad
            ||isRdState )
        {
            return -1UL;// as an error code, since the correct response has to be >0.
        }// else continue:
        //## end: functions to check state flags.
        */
        char cStringDivisorSequence[2];
        cStringDivisorSequence[0] = '_';
        cStringDivisorSequence[1] = 0;
        std::vector<std::string> * splittedTokens = Common::StrManipul::stringSplit( cStringDivisorSequence, secondToken, true);
        int hmFoundToken = splittedTokens->size();
        const char * decodedOrdinal_str = nullptr;
        const char * decodedPrime_str = nullptr;
        if(2<=hmFoundToken)// at least ordinal_prime ,i.e. 2 token.
        {
            decodedOrdinal_str = (*splittedTokens)[0].c_str();
            decodedPrime_str = (*splittedTokens)[1].c_str();
        }
        else
        {// TODO: evaluate a throw, due to inconsistent dumpIntegralFile.
            throw;// TODO : manage the error case.
        }
        // TODO : manage exception on parsing.
        res.Ordinal = Common::StrManipul::stringToUnsignedLong( decodedOrdinal_str);// TODO : manage exception on parsing. test:
        if( res.Ordinal>this->lastOrdinal)
        {
            throw;// TODO : manage the error case.
        }
        dumpReader.seekg( -1*totalReadTokenLength, dumpReader.cur );// GO back, of the read amount.########### crucial action #####
        long presentPosition = dumpReader.tellg();//#### NB. ####
     // trascrizione dei risultati:
     res.Prime = (long)Common::StrManipul::stringToUnsignedLong( decodedPrime_str);
     res.positionByte = presentPosition;
    // clanup:
    delete splittedTokens;
    //delete decodedOrdinal_str;//NB. already deleted, as parts of splittedTokens.
    //delete decodedPrime_str;
     // ready.
     return res;
 }// readRecordAt(


  int PrimesFinder::Primes::Bisection( const  long requiredOrdinal, const  long initialization, bool wantInitialization )
 {
     long MassimoMinoranti, MinimoMaggioranti;// NB. have to be signed.
     double DeltaTessutoProdotto;
    AsinglePointInStream beg, decoded, last;
    long LandingPoint;
    unsigned long decodedOrdinal = -1UL;
    long leftBoundary = 0;
    // init  last : read last record
    std::ifstream dumpReader( this->theDumpPath, std::fstream::in );// read-only.
    this->getActualLength();// this call contains three actions:
    // NB. the previous line refreshes what follows: this->secureRightBound = this->actualPrimaryFileLength - this->tailRecordSize;
    last.Ordinal = this->lastOrdinal;// TODO monitor the compatibility signed-unsigned.
    last.Prime = this->lastPrime;// TODO monitor the compatibility signed-unsigned.
    last.positionByte = this->actualPrimaryFileLength;// TODO monitor the compatibility signed-unsigned.
    long usefulPartOfDump_measure = (long)this->actualPrimaryFileLength;// init. It will be updated bisecting.    
    MassimoMinoranti = leftBoundary;// init.
    long rightBoundary = this->actualPrimaryFileLength;
    MinimoMaggioranti = rightBoundary;// init.
    // init   beg : beg is certain; no need to read.
    beg.Ordinal = +1;
    beg.Prime = +2;
    beg.positionByte = 0;
     // init
     if( ! wantInitialization)
     {
         LandingPoint = (long)(  0.5 * (double)last.positionByte  );//bisect
     }
     else
     {
         LandingPoint = initialization;
     }
     // init   decoded
     decoded.Ordinal = -1;// init to invalid, to enter the loop.test.Ordinal;
     decoded.Prime =  -1;// init to invalid, to enter the loop.
     decoded.positionByte =  -1;// init to invalid, to enter the loop.
     // ####
     int acc=0;// accumulator of steps, needed to converge.
     for( ; requiredOrdinal!=decoded.Ordinal; acc++)
     {
         // here do: seekg #############
         AsinglePointInStream test = this->readRecordAt( dumpReader, LandingPoint);
         decoded.Ordinal = test.Ordinal; //LandingPoint * +1/+1.91; // [Dim]==[ordinal] //Hypothesis: position==f(ordinal)==+1.91*ordinal
         decoded.Prime = test.Prime;
         decoded.positionByte = test.positionByte; // +1.91 * LandingPoint;
         //###
            //DeltaTessutoSomma = requiredOrdinal - decoded.Ordinal; #### descarded as not converging
            //DeltaTessutoProdotto = (double)DeltaTessutoSomma/(double)(last.positionByte);
         //###
        if( decoded.Ordinal<requiredOrdinal)// #### landingPoint evaluation #####
        {// bisection forward : right leaf
           if( decoded.positionByte > MassimoMinoranti)
            {
                MassimoMinoranti = decoded.positionByte;
            }// else MassimoMinoranti is already better than that.
            // MinimoMaggioranti =  this->actualPrimaryFileLength; already got it from init.
        }
        else if( decoded.Ordinal > requiredOrdinal)
        {// bisection backward : left leaf
            // leftBoundary = 0; already got it from init.
            if(decoded.positionByte-this->tailRecordSize < MinimoMaggioranti)
            {
                MinimoMaggioranti = decoded.positionByte-this->tailRecordSize; //-totalReadTokenLength;
            }// else MinimoMaggioranti is already better than that.
        }
        else// i.e.  decodedOrdinal == requiredOrdinal
        {
            // restituire  decoded.Prime
            break;
        }
        // common factors:
        leftBoundary = MassimoMinoranti;// keep memory of previous narrowings.
        rightBoundary = MinimoMaggioranti;// keep memory of previous narrowings.
        usefulPartOfDump_measure = rightBoundary - leftBoundary;
        LandingPoint = (long)( 0.5*usefulPartOfDump_measure+leftBoundary);//NB. add "leftBoundary", to fit the actual stream.
        if(LandingPoint <0) {LandingPoint=0;}
     }// for
     //###
     dumpReader.close();
     //ready.
     return acc;
 }// Bisection


   int PrimesFinder::Primes::NpartSection( const  long requiredOrdinal, const  long initialization, bool wantInitialization )
 {
     long MassimoMinoranti, MinimoMaggioranti;// NB. have to be signed.
    AsinglePointInStream beg, decoded, last;
    long LandingPoint;
    unsigned long decodedOrdinal = -1UL;
    long leftBoundary = 0;
    long rightBoundary = this->actualPrimaryFileLength;
    // init   beg : beg is certain; no need to read.
    beg.Ordinal = +1;
    beg.Prime = +2;
    beg.positionByte = 0;
    // init  last : read last record
    std::ifstream dumpReader( this->theDumpPath, std::fstream::in );// read-only.
    this->getActualLength();// this call contains three actions:
    // NB. the previous line refreshes what follows: this->secureRightBound = this->actualPrimaryFileLength - this->tailRecordSize;
    last.Ordinal = this->lastOrdinal;// TODO monitor the compatibility signed-unsigned.
    last.Prime = this->lastPrime;// TODO monitor the compatibility signed-unsigned.
    last.positionByte = this->actualPrimaryFileLength;// TODO monitor the compatibility signed-unsigned.
    long usefulPartOfDump_measure = (long)this->actualPrimaryFileLength;// init. It will be updated bisecting.
     // init
     if( ! wantInitialization)
     {
         LandingPoint = (long)(  0.5 * (double)last.positionByte  );//bisect
     }
     else
     {
         LandingPoint = initialization;
     }
     // init   decoded
     decoded.Ordinal = -1;// init to invalid, to enter the loop.test.Ordinal;
     decoded.Prime =  -1;// init to invalid, to enter the loop.
     decoded.positionByte =  -1;// init to invalid, to enter the loop.
     // ####
     int acc=0;// accumulator of steps, needed to converge.
     for( ; requiredOrdinal!=decoded.Ordinal; acc++)
     {
         // here do: seekg #############
         AsinglePointInStream test = this->readRecordAt( dumpReader, LandingPoint);
         decoded.Ordinal = test.Ordinal; //LandingPoint * +1/+1.91; // [Dim]==[ordinal] //Hypothesis: position==f(ordinal)==+1.91*ordinal
         decoded.Prime = test.Prime;
         decoded.positionByte = test.positionByte; // +1.91 * LandingPoint;
         //###
            //DeltaTessutoSomma = requiredOrdinal - decoded.Ordinal; #### descarded as not converging
            //DeltaTessutoProdotto = (double)DeltaTessutoSomma/(double)(last.positionByte);
         //###
        if( decoded.Ordinal<requiredOrdinal)// #### landingPoint evaluation #####
        {// bisection forward : right leaf
            leftBoundary = decoded.positionByte;
            rightBoundary = this->actualPrimaryFileLength;
        }
        else if( decoded.Ordinal > requiredOrdinal)
        {// bisection backward : left leaf
            leftBoundary = 0;
            rightBoundary = decoded.positionByte-this->tailRecordSize; //-totalReadTokenLength;
        }
        else// i.e.  decodedOrdinal == requiredOrdinal
        {
            // restituire  decoded.Prime
            break;
        }
        // common factors:
        usefulPartOfDump_measure = rightBoundary - leftBoundary;
        LandingPoint = (long)( 0.5*usefulPartOfDump_measure+leftBoundary);//NB. apply an addition of "leftBoundary", to fit the actual stream.
     }// for
     //###
     dumpReader.close();
     //ready.
     return acc;
 }// NpartSection


 // suggestions for bug-fixing on index[1]
 // on seekg(0, begin) swap partial_token and second_token, since there's no previous token, with respect to the first one. DONE
 // on seekg(0, end ) : cannot read more than the remaining bytes to EOF. So cannot seekg past (filesize-recordLength). DONE.
 // on assignement const int tokenSize = 60; evaluate a dynamic size. DONE: global dataMember tailRecordSize==60.
 //
// it's a utility; syntax: Prime[ordinal]==...
unsigned long   PrimesFinder::Primes::operator[] ( const unsigned long requiredOrdinal )
{// linear bisection on IntegralFile.
    this->feedDumpPath();
    if( nullptr == this->theDumpPath)
    {
        return -1UL;// as an error code, since the correct response has to be >0.
    }// else continue:
    unsigned long requiredPrime;
    std::ifstream dumpReader( this->theDumpPath, std::fstream::in );// read-only.
    this->getActualLength();// ths call contains the three actions, deleted below.
    // NB. the previous line refreshes what follows: this->secureRightBound = this->actualPrimaryFileLength - this->tailRecordSize;
//    dumpReader.seekg( 0, dumpReader.end);
//    long dumpSize = dumpReader.tellg();
//long secureRightBound = dumpSize - this->tailRecordSize;
    long usefulPartOfDump_measure = this->actualPrimaryFileLength;// init. It will be updated bisecting.
    long leftBoundary = 0;
    long rightBoundary = this->actualPrimaryFileLength;
    // start bisecting:
    this->getLastCoupleInDefaultFile();// this call writes into members: {lastOrdinal, lastPrime}.
    if( requiredOrdinal>this->lastOrdinal// NB. do NOT attempt reading after EOF, which throws.
        || requiredOrdinal<=0 )
    {
        return -1UL;// as an error code, since the correct response has to be >0.
    }// else continue:
    double requiredLandingPoint = ( (double)requiredOrdinal / (double)(this->lastOrdinal) ) * usefulPartOfDump_measure;// NB. crucial ####
    int target;
    const int tokenSize = this->tailRecordSize;// globally defined.
    char partialToken[tokenSize];
    char secondToken[tokenSize];
    unsigned long decodedOrdinal = -1UL;
    //
    for( ; requiredOrdinal!= decodedOrdinal;)
    {// loop della bisezione:
        target = (int)(requiredLandingPoint);// find required %.
        if( secureRightBound<target)// required a landing-point, after the secureRightBound
        {
            this->dumpTailReaderByChar( this->theDumpPath);
            this->recoverDumpTail( this->theDumpTailStr);
            for(int c=0; ; c++)
            {// scan the dumpTailArray
                if( c>this->actualCoupleCardinality-1)
                {
                    throw "element not found in file tail.";
                }
                if( requiredOrdinal==dumpTail[c].ordinal)
                {
                    decodedOrdinal = dumpTail[c].ordinal;// exit condition
                    requiredPrime = dumpTail[c].prime;
                    dumpReader.close();// TODO evaluate if leave open for ReadRange()
                    return requiredPrime;// NB. break is not enough!
                }// else continue.
            }// scan the dumpTailArray
        }// required a landing-point, after the secureRightBound
        else
        {// NB. only seek to safe locations(i.e. <=secureRightBound) otherwise the flag-family isBad,isEOF will throw something.
            dumpReader.seekg( target, dumpReader.beg);// GOTO required %.##################################### crucial action #####
        }// after having landed, evaluate:
        // first Token has to be thrown away, since it is likely to be truncated before the beginning, due to
        // random access to seek(bisection); next record will be complete.
        dumpReader.getline( partialToken, tokenSize, '\r' );
        dumpReader.getline( secondToken, tokenSize, '\r' );// read the whole line, until newline.
        if(0==target)//if the landing-point is the beginning of stream, then the useful token is the first one, since there's no previous one.
        {// it's needed only when searching for the first record in the dump, since it has no previous record.
            for( int c=0; c<tokenSize; c++)
            {// mustSwapTokens
                secondToken[c] = partialToken[c];
            }// mustSwapTokens
        }// no else; when searching for records different from the absolute first, there's no need for this swap.
        int partialToken_Length = strlen_loc( partialToken);
        int secondToken_Length = strlen_loc( secondToken);
        int totalReadTokenLength = partialToken_Length+secondToken_Length+2;// +the two '\r' that are descarded.
        /* in case of need to Debug the stream seeking.
        //## functions to check state flags:
        bool isGood = dumpReader.good();
        bool isEOF = dumpReader.eof();
        bool isFailure = dumpReader.fail();
        bool isBad = dumpReader.bad();
        bool isRdState = dumpReader.rdstate();
        if( !isGood
            ||isEOF
            ||isFailure
            ||isBad
            ||isRdState )
        {
            return -1UL;// as an error code, since the correct response has to be >0.
        }// else continue:
        //## end: functions to check state flags.
        */
        char cStringDivisorSequence[2];
        cStringDivisorSequence[0] = '_';
        cStringDivisorSequence[1] = 0;
        std::vector<std::string> * splittedTokens = Common::StrManipul::stringSplit( cStringDivisorSequence, secondToken, true);
        int hmFoundToken = splittedTokens->size();
        const char * decodedOrdinal_str = nullptr;
        const char * decodedPrime_str = nullptr;
        if(2<=hmFoundToken)// at least ordinal_prime ,i.e. 2 token.
        {
            decodedOrdinal_str = (*splittedTokens)[0].c_str();
            decodedPrime_str = (*splittedTokens)[1].c_str();
        }
        else
        {// TODO: evaluate a throw, due to inconsistent dumpIntegralFile.
            return -1UL;// as an error code, since the correct response has to be >0.
        }
        // TODO : manage exception on parsing.
        decodedOrdinal = Common::StrManipul::stringToUnsignedLong( decodedOrdinal_str);// TODO : manage exception on parsing. test:
        if( decodedOrdinal>this->lastOrdinal)
        {
            return -1UL;// as an error code, since the correct response has to be >0.
        }
        long presentPosition = dumpReader.tellg();//#### NB. ####
        if( decodedOrdinal<requiredOrdinal)// #### landingPoint evaluation #####
        {// bisection forward : right leaf
            leftBoundary = presentPosition;
            rightBoundary = this->actualPrimaryFileLength;
        }
        else if( decodedOrdinal > requiredOrdinal)
        {// bisection backward : left leaf
            leftBoundary = 0;
            rightBoundary = presentPosition-totalReadTokenLength;
        }
        else// i.e.  decodedOrdinal == requiredOrdinal
        {
            requiredPrime =  Common::StrManipul::stringToUnsignedLong( decodedPrime_str);
            break;
        }
        // common factors:
        usefulPartOfDump_measure = rightBoundary - leftBoundary;
        requiredLandingPoint = ( (double)requiredOrdinal / (double)decodedOrdinal ) * usefulPartOfDump_measure;
        // clanup:
        delete splittedTokens;
        //delete decodedOrdinal_str;//NB. already deleted, as parts of splittedTokens.
        //delete decodedPrime_str;
    }// loop della bisezione.
    // ready.
    dumpReader.close();// TODO evaluate if leave open for ReadRange()
    return requiredPrime;
}// operator[]




// IntegerDecomposition : the Fundamental Thm of Arithmetic.
PrimesFinder::Primes::SingleFactor * PrimesFinder::Primes::IntegerDecomposition( const unsigned long dividend)
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


void  PrimesFinder::Primes::recoverLastRecord( const char * dumpTail)
{
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
        {
            filteredLastToken.append( 1, '_' );// subst. with '_'
        }
    }//for: preserve only digits and substitute everything else with underscore. Then split on underscore.
    std::vector<std::string> * tokenArray = Common::StrManipul::stringSplit("_", filteredLastToken.c_str(), true );// remove empty entries.
    int i=+1;
    for( std::vector<std::string>::reverse_iterator it=tokenArray->rbegin(); it!=tokenArray->rend(); ++it, i++)
    {// preserve only the last two semi-records: they are lastOrdinal and LastPrime.
        if( 1==i)
        {// last Prime
            this->lastPrime = Common::StrManipul::stringToUnsignedLong(*it);
        }
        else if( 2==i)
        {// last ordinal
            this->lastOrdinal = Common::StrManipul::stringToUnsignedLong(*it);
        }// else the token is not interesting; it was included in the read portion, but is not part of the last row.
    }// the two interesting semi-tokens are the last two; so the reading is in reverse order.
}//recoverLastRecord


// produce an array of couples {ordinal,prime} from the dumpTail.
PrimesFinder::Primes::DumpElement * PrimesFinder::Primes::recoverDumpTail( const char * dumpTail)
{
    if( nullptr != this->dumpTail)
    {// do not build again.
        return this->dumpTail;
    }// else build it; it will be a shared dataMember and Dtor will delete.
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
        {
            filteredLastToken.append( 1, '_' );// subst. with '_'
        }
    }//for: preserve only digits and substitute everything else with underscore. Then split on underscore.
    std::vector<std::string> * tokenArray = Common::StrManipul::stringSplit("_", filteredLastToken.c_str(), true );// remove empty entries.
    int entryCardinality = tokenArray->size();
// already init this->actualCoupleCardinality = 0;//NB: tagliare al massimo dei minoranti pari
    // NB: stabilire se pari
    if( (double)entryCardinality/2.0 - entryCardinality/2 <+1E-80 )
    {// parita'
        this->actualCoupleCardinality = entryCardinality/2;
    }
    else
    {// DISparita'
        this->actualCoupleCardinality = (entryCardinality-1)/2;// NB: tagliare al massimo dei minoranti pari
    }
    // NB: allocare per tale misura
    this->dumpTail = new DumpElement[actualCoupleCardinality];// member struct-array {ordinal,prime}.
    // NB: fill-up reverse
    int currentCouple=actualCoupleCardinality-1;
    for( std::vector<std::string>::reverse_iterator it=tokenArray->rbegin();
     it!=tokenArray->rend() && currentCouple>=0;
       currentCouple--) // no more increment on the iterator
    {// preserve the last complete-records: they have to be {Ordinal,Prime}. Use index-parity for this task:
        // get a Prime from tailEnd, coming back:
        this->dumpTail[currentCouple].prime =  Common::StrManipul::stringToUnsignedLong(*(it++));
        // get an ordinal from tailEnd, coming back:
        this->dumpTail[currentCouple].ordinal =  Common::StrManipul::stringToUnsignedLong(*(it++));// goto next record backwards.
    }// the interesting semi-tokens are the ones of complete records; so the reading is in reverse order, excluding a partial record, if present.
    delete tokenArray;
    //ready.
    return this->dumpTail;// caller has to delete.
}//recoverDumpTail


    // state of the art.
void PrimesFinder::Primes::Start_PrimeDump_FileSys() const
{
    unsigned long ordinal = this->lastOrdinal;// next Prime to be found, will increase the ordinal.TODO: decide whether to increment the member.
    bool isStillPrime = true;
    double realQuotient;
    unsigned long intQuotient;
    unsigned long cursor = this->lastPrime+1UL;// start stepping from the Int after the last found Prime.
    ofstream appendStream( this->theDumpPath, std::fstream::out | std::fstream::app);
    //
    for( ; cursor<=this->desiredThreshold; cursor++)//NB. cursor==dividend.
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
            appendStream.write( strBuild->str().c_str(), strBuild->str().length() );
            delete strBuild;// clean up the token-buffer.
            strBuild = nullptr;
        }// else ripristino del flag-primalita' per il candidato divisore successivo.
        else
        {// ripristino della primalita', dopo un composto(i.e. non primo).
            isStillPrime = true;
        }// ripristino della primalita', dopo un composto(i.e. non primo).
    }// external for : the one where cursor cicles from inf to sup, on dividends.
    appendStream.close();
    // ready.
}// IntegralFileFromStartFSproducer


