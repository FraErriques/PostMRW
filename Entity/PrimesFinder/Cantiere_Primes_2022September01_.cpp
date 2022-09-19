#include "Cantiere_Primes_2022September01_.h"
#include <iostream>
#include <string>
#include <math.h>
#include <ctime>
#include "Primes.h"
#include "../../Common/Config_wrap/Config_wrap.h"
#include "../../Common/StringBuilder/StringBuilder.h"
#include "../../Common/LogFs_wrap/LogFs_wrap.h"
#include "../../Entity/Integration/Integration.h"
#include "InternalAlgos.h"

namespace Cantiere_Primes_2022September01_
{


// empty Ctor : reads both the sequentialFile and randomFile fullpath
Primes::Primes( )
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



bool Primes::SequentialCalcInterface( unsigned long long Threshold )
{
    bool hasSequentialDumpBeenReset = false;// it's true on filesize<1k and of course on non existing file
    // ---call with params
    const char * stringDumpTail = this->newDeal_dumpTailReaderByChar( this->sequentialDumpPath );// last few records in a string.
    if(nullptr==stringDumpTail)
    {// unable to read the tail of the sequential dump in a string -> reset the file.
        hasSequentialDumpBeenReset = true;
    }// else it has been initialized to false.
    Primes::DumpElement * lastRec = nullptr;// the very last record, deciphered from a func:newDeal_recoverLastRecord
    long long LastOrdinal = 0UL;
    long long LastPrime = 0UL;
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

bool Primes::ReadSequentialDumpInterface()
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
    // test-sigle-record Primes::AsinglePointInStream * nextRecord = this->acquireNextRecord( 50);// pass file-seek-offset.
    int cardinalityOfRecordSequence = 0;
    // next line : test-multirecord
    Primes::DumpElement * recSequence = this->acquireSequenceOfRecord(
        seek_START
        , seek_END
        , &cardinalityOfRecordSequence );
    if(nullptr != recSequence)
    {
        for( int c=0; c<cardinalityOfRecordSequence; c++)
        {
            std::cout<<"\n\t Record "<<recSequence[c].ordinal<<"_"<<recSequence[c].prime;
        }
        std::cout<<"\n\n";
        res = true;
    }// else res stays false.
    //---do the job here----END
    this->sharedReader->close();
    delete this->sharedReader;
    this->sharedReader = nullptr;
    // ready
    return res;
}// ReadSequentialDumpInterface



bool Primes::RandomCalcInterface( unsigned long long infLeft, unsigned long long maxRight )
{
    bool res = false;
    //
    // NB. no {dumpTailReader, recoverLastRecord,...} -> work in [infLeft, maxRight].
    Entity::Integration::FunctionalForm LogIntegral = internalAlgos::LogIntegral_coChain;// function pointer.
    double LogIntegral_ofInfPar = Entity::Integration::trapezi( +2.0, (double)infLeft, ((double)infLeft-2.0)*4, LogIntegral );
    unsigned long long extimatedOrdinal= (unsigned long long)LogIntegral_ofInfPar;//TODO stima !
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




const char * Primes::feedDumpPath() // non const
{// default section, in default file.
    if( nullptr==this->sequentialDumpPath )
    {
        this->sequentialDumpPath = this->getPrimeDumpFullPath( "primeDefaultFile");// Default Section Name.
    }//else ready.
    return this->sequentialDumpPath;
}// feedDumpPath()

const char * Primes::feed_CustomDumpPath() // non const
{// custom section, in default file.
    if( nullptr==this->randomDumpPath )
    {
        this->randomDumpPath = this->getPrimeDumpFullPath( "primeCustomFile");// CUSTOM Section Name, for non complete dumping.
    }//else ready.
    return this->randomDumpPath;
}// feed_CustomDumpPath()

const char * Primes::getPrimeDumpFullPath( const std::string & sectionNameInFile) const
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
    Primes::~Primes()
    {/// Dtor() : closes the append_handle.
//        if( nullptr != this->sequentialDumpPath )
//        {
//            delete[] this->sequentialDumpPath;
//            this->sequentialDumpPath = nullptr;
//        }
//        if( nullptr != this->randomDumpPath )
//        {
//            delete[] this->randomDumpPath;
//            this->randomDumpPath = nullptr;
//        }
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




void Primes::createOrAppend( const std::string & fullPath)
{
    ofstream createOrApp(fullPath, std::fstream::out | std::fstream::app);
    createOrApp.close();
}// dumpFile createOrAppend



const char * Primes::lastRecordReaderByChar( const std::string & fullPath)
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

const char * Primes::newDeal_dumpTailReaderByChar( const std::string & fullPath)
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



/*  IntegerDecomposition : the Fundamental Thm of Arithmetic.
SingleFactor * Primes::IntegerDecomposition( const unsigned long long dividend)
{
    Entity::Integration::FunctionalForm LogIntegral = internalAlgos::LogIntegral_coChain;// function pointer.
    double LogIntegral_ofInfPar = Entity::Integration::trapezi( +2.0, (double)dividend, ((double)dividend-2.0)*4, LogIntegral );
    unsigned long long ordinaleStimato = (unsigned long)LogIntegral_ofInfPar;// approx eccesso: LogIntegral[Soglia]==LastOrdinal_under_Soglia==Cardinalita[sottoSoglia].
    SingleFactor * factorization = new SingleFactor[ordinaleStimato];// stimare #fattoriMaximal.
    // Oss. greatest involved-prime==dividend/2 in a composite, since greatestFactor is the cofactor of the PotentialSmallest(i.e. 2).
    for(int c=0; c<ordinaleStimato; c++)
    {// init to zeroContentMemory.
        factorization[c].factorBase = 0;
        factorization[c].factorMultiplicity = 0;
    }
    // TODO readRange( 1, ordinaleStimato);
    unsigned long long * involvedPrimes = new unsigned long[ordinaleStimato];
    for(int c=0; c<ordinaleStimato; c++)
    {
        involvedPrimes[c] = (*this)[c+1];//NB. Prime[1]==2 , Prime[0]==error.
    }// end filling up the candidate prime-factor array.
    unsigned long long dividendo, divisore;
    dividendo = dividend;
    double realQuotient;
    unsigned long long intQuotient;
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

Primes::DumpElement * Primes::newDeal_recoverLastRecord( const char * dumpTail)
{
    Primes::DumpElement * lastRecord = new Primes::DumpElement();
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


// newDeal_recoverDumpTail : produce an array of couples {ordinal,prime} from a String : dumpTail_String.
Primes::DumpElement * Primes::newDeal_recoverDumpTail( const char * dumpTail_String , int *recordArrayCardinality)
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
void Primes::Start_PrimeDump_FileSys(
        unsigned long long Left
        ,unsigned long long Right
        ,std::ofstream * appendStream
        ,unsigned long long ordinal // passed as real xor extimated ordinal of "Left" i.e. Left==Prime[ordinal]
    )
{
    bool isStillPrime = true;
    double realQuotient;
    unsigned long long intQuotient;
    unsigned long long cursor = Left+1UL;// start stepping from the Int after the last found Prime.
    if( cursor<+2){cursor=+2;}// 1 (i.e. one) is the product-invariant; so, not a prime.
    // NB now a data-member ; ofstream appendStream( this->sequentialDumpPath, std::fstream::out | std::fstream::app);
    //
    for( ; cursor<=Right; cursor++)//NB. cursor==dividend.
    {
        double soglia = sqrt( cursor);// division is a two-operand operator: the bisection of dividend is Sqrt[dividend]
        // when dividend/Sqrt[dividend]==Sqrt[dividend] and when dividend/(Sqrt[dividend]+eps)<Sqrt[dividend]
        // so the stepping into divisor>Sqrt[dividend] leads to divisors<Sqrt[dividend] which have already been explored.
        unsigned long long divisor=+2;
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
            std::string * ordinalStr = Common::StrManipul::uLongLongToString(ordinal);
            std::string * primeStr = Common::StrManipul::uLongLongToString( cursor );
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
    // NO MORE file-closure : interface does it : appendStream.close();
    // ready.
}// newDeal IntegralFileFromStartFSproducer



Primes::AsinglePointInStream * Primes::acquireNextRecord( unsigned long long discriminatingElement_position)
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


Primes::DumpElement * Primes::acquireSequenceOfRecord(
    unsigned long long discriminatingElement_position
    , unsigned long long until_position
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
 Primes::Bisection( requiredOrdinal)
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

}// namespace Cantiere_Primes_2022September01_



/* cantina


**** cantina ******/
