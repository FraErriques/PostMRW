#include <iostream>
#include <string>
#include <math.h>
#include "Primes.h"
#include "../../Common/Config_wrap/Config_wrap.h"
#include "../../Common/StringBuilder/StringBuilder.h"
#include "../../Common/LogFs_wrap/LogFs_wrap.h"


    /*
    this Construction path is devoted to log the results on the default IntegralFile; the one that starts from origin(i.e. +2).
    Another Ctor will be provided, to log on a partial-File, which consists in a custom analysis, in [min, max]. For such
    Ctor the params will be Ctor( min, max, desiredConfigSectionName)
    */
    PrimesFinder::Primes::Primes()
    {
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


// from email #
const char * PrimesFinder::Primes::getDefaultPrimeDumpFullPath( const std::string & sectionNameInFile)
{
    const char *  res = nullptr;
    Common::ConfigurationService * primeNamedConfig = new
    Common::ConfigurationService( "./PrimeConfig.txt");
    const std::string * desiderSectionContent =
    primeNamedConfig->getValue( sectionNameInFile);
    res = desiderSectionContent->c_str();
    return res;
}// getDefaultPrimeDumpFullPath

// from email #
void PrimesFinder::Primes::createOrAppend( const std::string & fullPath)
{
    ofstream createOrApp(fullPath, std::fstream::out |
    std::fstream::app);
    createOrApp.close();
}// dumpFile createOrAppend

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
     lastrecReader.seekg( -1, ios_base::end);//########## NB. to do not get EOF on first read, it's necessary to seek(-1,end).
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
         int readASCIIcode = lastrecReader.get();
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



    unsigned long PrimesFinder::Primes::getActualLength()
    {
        return this->actualPrimaryFileLength;
    }

   // it's a read-only utility; syntax: Prime[ordinal]==...
   unsigned long   PrimesFinder::Primes::operator[] ( const unsigned long & requiredOrdinal ) const
   {// TODO linear bisection on IntegralFile.
       return 2UL;// TODO
   }


/*
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
