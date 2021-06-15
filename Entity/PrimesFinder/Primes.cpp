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
        this->theDumpPath = this->getPrimeDumpFullPath( "primeDefaultFile");// Default Section Name.
        if( nullptr != this->theDumpPath)
        {
            this->createOrAppend( this->theDumpPath);
        }// else : TODO not-healthly built.
        char * straightContentOfDumpTail  = this->lastRecordReaderByString( this->theDumpPath);
        if( nullptr != straightContentOfDumpTail)
        {
            recoverLastRecord( straightContentOfDumpTail);// members should be in place, now: lastOrdinal, lastPrime.
        }// else : no valid last record : start from zero!
        this->desiredThreshold = threshold;// set the upper bound for research, in R+.
    }// Ctor


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




    /*
    This Construction path is devoted to log the results on a CUSTOM IntegralFile.
    This Ctor is devoted to log on a partial-File, which consists in a custom analysis, in (infLeft, maxRight]. For such
    Ctor the params are: Ctor( infLeft, maxRight, desiredConfigSectionName).
    */
    PrimesFinder::Primes::Primes(unsigned long infLeft, unsigned long maxRight, const std::string& desiredConfigSectionName)
    {// CUSTOM section, in default file.
        this->theDumpPath = this->getPrimeDumpFullPath( desiredConfigSectionName);// CUSTOM Section Name.
        if( nullptr != this->theDumpPath)
        {
            this->createOrAppend( this->theDumpPath);
        }// else : TODO not-healthly built.
        // NB. no {lastRecordReaderByString, recoverLastRecord,...} -> work in [infLeft, maxRight].
        Entity::Integration::FunctionalForm LogIntegral = LogIntegral_coChain;// function pointer.
        double LogIntegral_ofInfPar = Entity::Integration::trapezi( +2.0, (double)infLeft, ((double)infLeft-2.0)*4, LogIntegral );
        this->lastOrdinal= (unsigned long)LogIntegral_ofInfPar;//TODO stima !
        this->lastPrime = infLeft;//##### the first integer analyzed will be infLeft+1; the last will be "maxRight" parameter.##
        this->desiredThreshold = maxRight;
        // write a stamp, about what we're doing and when.
        time_t ttime = time(0);
        char* dt = ctime(&ttime);
        //tm *gmt_time = gmtime(&ttime);  NB. for UTC Greenwich
        //dt = asctime(gmt_time);
        ofstream stampWriter( this->theDumpPath, std::fstream::out | std::fstream::app);
        stampWriter<<"\n\n Custom Interval ("<<infLeft<<", "<<maxRight<<"] ,worked on: "<<dt<<"\n";
        stampWriter.close();
    }// Ctor



    /// Dtor()
    PrimesFinder::Primes::~Primes()
    {/// Dtor() : closes the append_handle.
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
    this->theDumpPath = this->getPrimeDumpFullPath( "primeDefaultFile");// Default Section Name.
    if( nullptr != this->theDumpPath)
    {
        this->createOrAppend( this->theDumpPath);
    }// else : TODO not-healthly built.
    else {return res;}// which is still "false".
    char * straightContentOfDumpTail  = this->lastRecordReaderByString( this->theDumpPath);
    if( nullptr != straightContentOfDumpTail)
    {
        recoverLastRecord( straightContentOfDumpTail);// members should be in place, now: lastOrdinal, lastPrime.
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
    const std::string * desiderSectionContent = primeNamedConfig->getValue( sectionNameInFile);// configSectionNames can be added.
    res = desiderSectionContent->c_str();
    return res;// caller has to delete.
}// getPrimeDumpFullPath





void PrimesFinder::Primes::createOrAppend( const std::string & fullPath)
{
    ofstream createOrApp(fullPath, std::fstream::out | std::fstream::app);
    createOrApp.close();
}// dumpFile createOrAppend




char *  PrimesFinder::Primes::lastRecordReaderByString( const std::string & fullPath)
{
    ifstream lastrecReader(fullPath, std::fstream::in );
    lastrecReader.seekg( 0, lastrecReader.end);
    int streamSize = lastrecReader.tellg();
    int lastTokenHypothesizedLength;// f(streamSize).
    // cannot use runtime-expressions in switch-case: case-labels have to be complite-time constants.
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
            lastTokenHypothesizedLength = 50;// suitable for primes in magnitude-order of 10^9
        }// end of if( streamsize..)->seek( howMuch, end).

    //
    lastrecReader.seekg( -1*lastTokenHypothesizedLength, lastrecReader.end);
    char * buffer = new char[lastTokenHypothesizedLength+1];
    lastrecReader.read( buffer, lastTokenHypothesizedLength);// fill the buffer from the stream-tail.
    buffer[lastTokenHypothesizedLength]=0;//terminate.
    lastrecReader.close();
    return buffer;
}// lastRecordReaderByString



    unsigned long PrimesFinder::Primes::getActualLength()
    {
        return this->actualPrimaryFileLength;
    }//getActualLength

    unsigned long PrimesFinder::Primes::getLastOrdinal()
    {
        return this->lastOrdinal;
    }//getLastOrdinal

    unsigned long PrimesFinder::Primes::getLastPrime()
    {
        return this->lastPrime;
    }//getLastPrime



 unsigned strlen_loc( char * par)
 {
     int res = 0;
     if(nullptr==par) {return res;}
     for( ; par[res]!=0; res++);
     return res;
 }


// it's a utility; syntax: Prime[ordinal]==...
unsigned long   PrimesFinder::Primes::operator[] ( const unsigned long & requiredOrdinal )
{// linear bisection on IntegralFile.
    const char * localDumpPath = new char[400];
    localDumpPath = this->getPrimeDumpFullPath( "primeDefaultFile");// Default Section Name, in default file.
    if( nullptr == localDumpPath)
    {
        return -1UL;// as an error code, since the correct response has to be >0.
    }// else continue:
    unsigned long requiredPrime;
    std::ifstream dumpReader( localDumpPath, std::fstream::in );// read-only.
    dumpReader.seekg( 0, dumpReader.end);
    long dumpSize = dumpReader.tellg();
    long leftBoundary = 0;
    long rightBoundary = dumpSize;
    // start bisecting:
    this->getLastCoupleInDefaultFile();// this call writes into members: {lastOrdinal, lastPrime}.
    if( requiredOrdinal>this->lastOrdinal)
    {
        return -1UL;// as an error code, since the correct response has to be >0.
    }// else continue:
    double requiredLandingPoint = (double)requiredOrdinal / (double)(this->lastOrdinal);
    int target = (int)(requiredLandingPoint*dumpSize);// find required %.
    dumpReader.seekg( target, dumpReader.beg);// GOTO required %.
    const int tokenSize = 60;
    char partialToken[tokenSize];
    char secondToken[tokenSize];
    unsigned long decodedOrdinal = -1UL;
    //
    for( ; requiredOrdinal!= decodedOrdinal;)
    {// loop della bisezione:
        // first one has to be thrown away, since it is likely to be truncated before the beginning, due to
        // random access to seek(bisection); next record will be complete.
        dumpReader.getline( partialToken, tokenSize, '\r' );
        dumpReader.getline( secondToken, tokenSize, '\r' );// read the whole line, until newline.
        int partialToken_Length = strlen_loc( partialToken);
        int secondToken_Length = strlen_loc( secondToken);
        int totalReadTokenLength = partialToken_Length+secondToken_Length+2;// +the two '\r' that are descarded.
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
        {
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
            rightBoundary = dumpSize;
            requiredLandingPoint = (double)requiredOrdinal / (double)(decodedOrdinal);
        }
        else if( decodedOrdinal > requiredOrdinal)
        {// bisection backward : left leaf
            leftBoundary = 0;
            rightBoundary = presentPosition-totalReadTokenLength;
            requiredLandingPoint = (double)requiredOrdinal / (double)(decodedOrdinal);
        }
        else// i.e.  decodedOrdinal == requiredOrdinal
        {
            requiredPrime =  Common::StrManipul::stringToUnsignedLong( decodedPrime_str);
            break;
        }
        int target = (int)(requiredLandingPoint*rightBoundary);// find required %.
        dumpReader.seekg( target, dumpReader.beg);// GOTO required %.
    }// loop della bisezione.
    // ready.
    dumpReader.close();
    delete[] localDumpPath;
    return requiredPrime;
}// operator[]



void  PrimesFinder::Primes::recoverLastRecord( const char * fromFile)
{
    std::string parFromFile(fromFile);
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






/*


    void PrimesFinder::Primes::old_lastRecordReader( const std::string & theDumpPath)
    {// get last record START
        //char * buf = new char[200];// stay huge.
        Common::StringBuilder * buf = new Common::StringBuilder(900);
        char * cleanToken = nullptr;
        Common::StringBuilder * unusefulToken = new Common::StringBuilder(100);// TODO forecast a size.
        Common::StringBuilder * primeToken = new Common::StringBuilder(100);// TODO forecast a size.
        Common::StringBuilder * ordinalToken = new Common::StringBuilder(100);// TODO forecast a size.
        int underscoreSymbolAccumulator=0;
        int hashSymbolAccumulator=0;
        ifstream lastRecordReader( theDumpPath, std::ios::in );// read-only; to get the last record.
        lastRecordReader.seekg (-1, lastRecordReader.end);
        int length = lastRecordReader.tellg();
        bool isDumpFileInGoodCondition = lastRecordReader.good();
        if( ! isDumpFileInGoodCondition || length<3)
        {// if file does not contain a single record, skip reading last-record.
            this->lastOrdinal=0;
            this->lastPrime=0;
        }
        else// can read last record
        {
            lastRecordReader.seekg( -1, lastRecordReader.end);// position right before EOF, i.e. -1,end.
            isDumpFileInGoodCondition = lastRecordReader.good();
            bool isTokenStart = true;
            bool isHalfToken = false;
            bool isTokenEnd = false;
            char singleChar[2];// one used, the other for termination.
            int hmUnderscore = 0;
            int c=0, acc=0;
            for( ; hmUnderscore<2 ; c++)
            {
                //lastRecordReader.get( singleChar);
                lastRecordReader.read( singleChar,1);
                singleChar[2]=0;// term.
                isDumpFileInGoodCondition = lastRecordReader.good();
                lastRecordReader.seekg( -2, lastRecordReader.cur);// each "get()" call seeks(+1), so to get a char backwards do a seek(-2).
                isDumpFileInGoodCondition = lastRecordReader.good();
                if( singleChar[0]<48 || singleChar[0]>57 )// is-NOT-digit
                {// is-NOT-digit
                    if( singleChar[0]==95) // isHalfToken=='_'==underscore
                    {// isHalfToken
                        buf->append(singleChar[0]);
                        //buf[acc++] = singleChar;
                        hmUnderscore++;// count the separators
                    }
                    else// new line or invalid chars: \r \n ...
                    {// new line or invalid chars: \r \n ...
                        buf->append('#');
                        //buf[acc++] = '#';
                    }// else// new line or invalid chars: \r \n ...
                    // invalid char->skip.
                }// if is NOT-digit
                if( singleChar[0]>=48 && singleChar[0]<=57 )// is digit
                {// is_digit
                    buf->append(singleChar[0]);
                    //buf[acc++] = singleChar;
                }// no else.
            }// for: exit on hmUnderscore==2.
            //
            for( int d=acc; d>=0; d--)
            {
                if( buf->str()[d]=='_' )
                {
                    underscoreSymbolAccumulator++;
                }
                else if( buf->str()[d]=='#' )
                {
                    hashSymbolAccumulator++;
                }
                if( buf->str()[d]>=48 && buf->str()[d]<=57 )
                {
                    if( +1==underscoreSymbolAccumulator && 0==hashSymbolAccumulator)
                    {
                        unusefulToken->append(buf->str()[d]);
                    }
                    else if( +1==underscoreSymbolAccumulator && +1==hashSymbolAccumulator)
                    {
                        ordinalToken->append(buf->str()[d]);
                    }
                    else if( +2==underscoreSymbolAccumulator && +1==hashSymbolAccumulator)
                    {
                        primeToken->append(buf->str()[d]);
                    }
                }// else skip non inherent char, in parsing.
            }// reparse the reverted string, which contains one and half token.
        }// read last record activity
        const char * unusefulTokenResult = unusefulToken->str().c_str();
        const char * ordinalTokenResult = ordinalToken->str().c_str();
        const char * primeTokenResult = primeToken->str().c_str();
        this->lastOrdinal = Common::StrManipul::stringToUnsignedLong( ordinalTokenResult);
        this->lastPrime = Common::StrManipul::stringToUnsignedLong( primeTokenResult);
        delete unusefulToken;
        delete ordinalToken;
        delete primeToken;
        delete buf;
        lastRecordReader.close();// close input stream.
        // get last record END
    }//  PrimesFinder::Primes::old_lastRecordReader()



// from email #
const char *  PrimesFinder::Primes::lastRecordReader( const std::string & fullPath)
{
     ifstream lastrecReader(fullPath, std::fstream::in );
     std::string * buf = new std::string();
     char curChar;
     lastrecReader.seekg( 0, lastrecReader.end);
     int streamSize = lastrecReader.tellg();
     std::cout<<"file length=="<<streamSize<<std::endl;
     bool ifstreamStatus = true;// TODO dbg
     int readChars = 0;
     int curPositionInStream = -1;
     lastrecReader.seekg( -1, lastrecReader.end); //ios_base::end);//########## NB. to do not get EOF on first read, it's necessary to seek(-1,end).
     for( readChars = 0; streamSize>=readChars; readChars++ )
     {
        // functions to check state flags
        bool isGood = lastrecReader.good();
        bool isEOF = lastrecReader.eof();
        bool isFailure = lastrecReader.fail();
        bool isBad = lastrecReader.bad();
        bool isRdState = lastrecReader.rdstate();
        // ### dbg ####
        //
        curPositionInStream = lastrecReader.tellg();
        std::cout<<"position before Get()  "<<curPositionInStream<<std::endl;
        //
        // int readASCIIcode = lastrecReader.get();
        int readASCIIcode = lastrecReader.peek();
        //
        curPositionInStream = lastrecReader.tellg();
        std::cout << "position after Get()" << curPositionInStream << std::endl;
        //
         lastrecReader.seekg( -2, ios_base::cur);//####### NB. each stream.get() seeks(+1,cur) so to read backwards I need seek(-2,cur).
        //
        curPositionInStream = lastrecReader.tellg();
        std::cout << "position after Seek(-3,cur)" << curPositionInStream << std::endl;
        //
         curChar = (char)readASCIIcode;
        // functions to check state flags
        isGood = lastrecReader.good();
        isEOF = lastrecReader.eof();
        isFailure = lastrecReader.fail();
        isBad = lastrecReader.bad();
        isRdState = lastrecReader.rdstate();
        ifstreamStatus = (isGood && !isEOF &&!isFailure && !isBad && !isRdState);
        //
         buf->append( 1, curChar);// append #one time, character curChar.
         if( !ifstreamStatus)
         {
             break;
         }
     }
     lastrecReader.close();//######### NB. close the stream #################
     const char * revertedBuf =  buf->c_str();
     int revertedLength = buf->length();
     char * straightBuf = new char[revertedLength];
     for( int c=buf->length()-1; c>=0; c-- )
     {
         straightBuf[(buf->length()-1)-c] = revertedBuf[c];
     }
     return straightBuf;
}// lastRecordReader



// state of the art.
void PrimesFinder::Primes::IntegralFileFromStartFSproducer( unsigned long sup) const
{
    unsigned long ordinal = 0UL;
    bool isStillPrime = true;
    double realQuotient;
    unsigned long intQuotient;
    unsigned long cursor=+2UL;
    //
    for( ; cursor<=sup; cursor++)//NB. cursor==dividend.
    {
        Common::StringBuilder * strBuild = nullptr;
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
    // ready.
}// IntegralFileFromStartFSproducer
*/



/*
void PrimesFinder::Primes::IntegralFileFromAnywhereFSproducer( unsigned long inf, unsigned long sup) const
{
    Common::LogWrappers::SectionOpen("TestConsole::LoggerSinkFS_example()", 0);
    bool isStillPrime = true;
    double realQuotient;
    unsigned long intQuotient;
    std::string * realQuotientStr = nullptr;
    std::string * intQuotientStr = nullptr;
    for( unsigned long cursor=inf; cursor<=sup; cursor++)//NB. cursor==dividend.
    {
        double soglia = sqrt( cursor);// division is a two-operand operator: the bisection of dividend is Sqrt[dividend]
        // when dividend/Sqrt[dividend]==Sqrt[dividend] and when dividend/(Sqrt[dividend]+eps)<Sqrt[dividend]
        // so the stepping into divisor>Sqrt[dividend] leads to divisors<Sqrt[dividend] which have already been explored.
        unsigned long divisor=+2;
        for( ; divisor<=soglia; divisor++)
        {
            realQuotient = (double)cursor/(double)divisor;
            intQuotient = cursor/divisor;
            realQuotientStr = Common::StrManipul::doubleToString(realQuotient);
            intQuotientStr = Common::StrManipul::uLongToString(intQuotient);
            if( realQuotient-intQuotient <+1.0E-80 )
            {// divisione diofantea
                isStillPrime = false;// NB. #################
                break;// NB. #################
            }
            else
            {// continue searching for primality
            }// else : // continue searching for primality
            delete realQuotientStr;// a new step allocates new memory, for each of those pointers.
            delete intQuotientStr;// a new step allocates new memory, for each of those pointers.
        }// the internal for : the one from [+2, cursor]
        Common::StringBuilder strBuild( 200);// on the stack
        if(isStillPrime)
        {
            strBuild.append("  ## Primo individuato ## : ");
            strBuild.append( *Common::StrManipul::uLongToString( cursor) );
            strBuild.append("  ###");
        }// else ripristino.
        else
        {// ripristino della primalita', dopo un composto(i.e. non primo).
            isStillPrime = true;
        }// ripristino della primalita', dopo un composto(i.e. non primo).
        std::string logBuf = strBuild.str();
        const char* buf = logBuf.c_str();
        Common::LogWrappers::SectionContent( buf, 0);
    }// external for : the one where cursor cicles from inf to sup, on dividends.
    // ready.
    Common::LogWrappers::SectionClose();
}// IntegralFileFromAnywhereFSproducer
*/


/* #cantina


    void PrimesFinder::Primes::lastRecordReader()
    {// get last record START
        //char * buf = new char[200];// stay huge.
        Common::StringBuilder * buf = new Common::StringBuilder(900);
        char * cleanToken = nullptr;
        Common::StringBuilder * unusefulToken = new Common::StringBuilder(100);// TODO forecast a size.
        Common::StringBuilder * primeToken = new Common::StringBuilder(100);// TODO forecast a size.
        Common::StringBuilder * ordinalToken = new Common::StringBuilder(100);// TODO forecast a size.
        int underscoreSymbolAccumulator=0;
        int hashSymbolAccumulator=0;
        ifstream lastRecordReader( theDumpPath, std::ios::in );// read-only; to get the last record.
        lastRecordReader.seekg (0, lastRecordReader.end);
        int length = lastRecordReader.tellg();
        bool isDumpFileInGoodCondition = lastRecordReader.good();
        if( ! isDumpFileInGoodCondition || length<3)
        {// if file does not contain a single record, skip reading last-record.
            this->ordinal=0;
            this->prime=0;
        }
        else// can read last record
        {
            lastRecordReader.seekg( -1, lastRecordReader.end);// position right before EOF, i.e. -1,end.
            isDumpFileInGoodCondition = lastRecordReader.good();
            bool isTokenStart = true;
            bool isHalfToken = false;
            bool isTokenEnd = false;
            char singleChar;
            int hmUnderscore = 0;
            int c=0, acc=0;
            for( ; hmUnderscore<2 ; c++)
            {
                lastRecordReader.get( singleChar);
                isDumpFileInGoodCondition = lastRecordReader.good();
                lastRecordReader.seekg( -2, lastRecordReader.cur);// each "get()" call seeks(+1), so to get a char backwards do a seek(-2).
                isDumpFileInGoodCondition = lastRecordReader.good();
                if( singleChar<48 || singleChar>57 )// is-NOT-digit
                {// is-NOT-digit
                    if( singleChar==95) // isHalfToken=='_'==underscore
                    {// isHalfToken
                        buf->append(singleChar);
                        //buf[acc++] = singleChar;
                        hmUnderscore++;// count the separators
                    }
                    else// new line or invalid chars: \r \n ...
                    {// new line or invalid chars: \r \n ...
                        buf->append('#');
                        //buf[acc++] = '#';
                    }// else// new line or invalid chars: \r \n ...
                    // invalid char->skip.
                }// if is NOT-digit
                if( singleChar>=48 && singleChar<=57 )// is digit
                {// is_digit
                    buf->append(singleChar);
                    //buf[acc++] = singleChar;
                }// no else.
            }// for: exit on hmUnderscore==2.
            //
            for( int d=acc; d>=0; d--)
            {
                if( buf->str()[d]=='_' )
                {
                    underscoreSymbolAccumulator++;
                }
                else if( buf->str()[d]=='#' )
                {
                    hashSymbolAccumulator++;
                }
                if( buf->str()[d]>=48 && buf->str()[d]<=57 )
                {
                    if( +1==underscoreSymbolAccumulator && 0==hashSymbolAccumulator)
                    {
                        unusefulToken->append(buf->str()[d]);
                    }
                    else if( +1==underscoreSymbolAccumulator && +1==hashSymbolAccumulator)
                    {
                        ordinalToken->append(buf->str()[d]);
                    }
                    else if( +2==underscoreSymbolAccumulator && +1==hashSymbolAccumulator)
                    {
                        primeToken->append(buf->str()[d]);
                    }
                }// else skip non inherent char, in parsing.
            }// reparse the reverted string, which contains one and half token.
        }// read last record activity
        const char * unusefulTokenResult = unusefulToken->str().c_str();
        const char * ordinalTokenResult = ordinalToken->str().c_str();
        const char * primeTokenResult = primeToken->str().c_str();
        this->ordinal = Common::StrManipul::stringToUnsignedLong( ordinalTokenResult);
        this->prime = Common::StrManipul::stringToUnsignedLong( primeTokenResult);
        delete unusefulToken;
        delete ordinalToken;
        delete primeToken;
        delete buf;
        lastRecordReader.close();// close input stream.
        // get last record END
    }//  PrimesFinder::Primes::lastRecordReader()




    void PrimesFinder::Primes::dumper()
    {
        if( nullptr != this->appendStream)
        {
            for( unsigned long ul=+99; ul<+199; ul++)
            {
                std::string * buf = Common::StrManipul::uLongToString(ul);
                Common::StringBuilder strBuild(6000);
                strBuild.append(buf->c_str());
                strBuild.append("_");
                strBuild.append(buf->c_str());
                strBuild.append("\r");

                const std::string & tmp = tokenEncoder(ul,ul).c_str();
                int len = tmp.length();
                appendStream->write( tmp.c_str(), len );
                delete buf;
                buf = nullptr;
            }
        }// else unable to write.
    }// dumper(


    /// caller DOES NOT have to delete.
    const std::string & PrimesFinder::Primes::tokenEncoder( unsigned long ordinal, unsigned long prime ) const
    {
        std::string * ordinalStr = Common::StrManipul::uLongToString(ordinal);
        std::string * primeStr = Common::StrManipul::uLongToString(prime);
        int forecastedTokenSize = ordinalStr->length()+primeStr->length()+3;//3 stands for '_'+'\n'+'\r'
        Common::StringBuilder * strBuild = new Common::StringBuilder( forecastedTokenSize);
        strBuild->append(ordinalStr->c_str());
        strBuild->append("_");
        strBuild->append(primeStr->c_str());
        strBuild->append("\r");// choose one btw '\r' or '\n'
        delete ordinalStr;
        delete primeStr;
        return strBuild->str();
    }




*/
