#include "Primes.h"
#include "InternalAlgos.h"
#include "../../Common/Config_wrap/Config_wrap.h"
#include "../../Common/StringBuilder/StringBuilder.h"
#include "../../Common/LogFs_wrap/LogFs_wrap.h"
#include "../../Entity/Integration/Integration.h"
#include <iostream>
#include <string>
#include <math.h>
#include <ctime>

/*  TODO
*   Bisection---------------------------------------------------------------------------(V)
*   transformed the char* into sdt::string *   -----------------------------------------(V)
*   StreamReader automatic and seekg internal to the reading-methods -------------------(V)
*   let the StreamWriter an automatic variable -----------------------------------------(V)
*   comment-records can be present in the sequntialDump---------------------------------(V)
*   enrich StringBuilder and Log for better tracing, overloading variable types-------------------------TODO
*/


namespace PrimesFinder {



// Ctor : reads the strings from Config, for both the sequentialFile and randomFile fullpath.
Primes::Primes( unsigned semiAmplitudeOfEachMapSegment )
{
    Common::LogWrappers::SectionOpen("Ctor Primes::Primes", 0);
    // set the semi-amplitude of each Prime-Segment that will be stored in the Map; it can be modified at runtime.
    this->sogliaDistanza = semiAmplitudeOfEachMapSegment;
    // build the Map of couples {Ordinal,Prime}
    this->memoryMappedDump = new std::map<unsigned long long, unsigned long long>();
    if( nullptr==this->memoryMappedDump)
    {
        this->isHealthlyConstructed = false;
        this->canOperate = true;// can operate even without memory mapped data.
    }
    // build the Map of couples {Threshold,LogIntegral[Threshold]}
    this->logIntegralPillars = new std::vector<LogIntegralPillarPoint>();
    if( nullptr==this->logIntegralPillars)
    {
        this->isHealthlyConstructed = false;
        this->canOperate = true;// can operate even without memory mapped data.
    }
    else
    {
        this->distributionFunction_fromExistingMesh();// locates via ini-file.
    }
    //
    bool dumpPathAcquisitionFromConfig = false;// init to invalid
    //---start sequential_file treatment
    this->feed_sequentialDumpPath();// SEQUENTIAL : default section, in default file.
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
    this->feed_customDumpPath();// CUSTOM section, in default file.
    if( nullptr != this->randomDumpPath )
    {
        this->createOrAppend( this->randomDumpPath);
        dumpPathAcquisitionFromConfig &= true;// NB &= only if both files are found, the flag gets true.
    }// else :  not-healthly built:  dumpPathAcquisitionFromConfig already false from init.
    else
    {
        dumpPathAcquisitionFromConfig = false;
    }
    this->feed_meshSpecificationPath();
    this->feed_localIntegralPath();
    this->feed_globalIntegralPath();
    //
    //---check operations' result and document the error for the user.
    if( false==dumpPathAcquisitionFromConfig)// something wrong reading from config files.
    {// not-healthly built.
        this->isHealthlyConstructed = false;
        this->canOperate = false;
//        this->append_Sequential_Stream = nullptr;
//        this->append_Random_Stream = nullptr;
    }// not-healthly built.
    // anyway( healtly or not) :avoid dangling pointers in Destructor's body.
//    this->append_Sequential_Stream = nullptr;
//    this->append_Random_Stream = nullptr;
    Common::LogWrappers::SectionClose();
}// empty Ctor(semiAmplitudeOfEachMapSegment)



bool Primes::SequentialCalcInterface( unsigned long long Threshold )
{
    Common::LogWrappers::SectionOpen("Primes::SequentialCalcInterface", 0);
    bool hasSequentialDumpBeenReset = false;// it's true on filesize<1k and of course on non existing file
    // ---call with params
    const std::string * stringDumpTail = this->dumpTailReaderByChar( this->sequentialDumpPath );// last few records in a string.
    if(nullptr==stringDumpTail)
    {// unable to read the tail of the sequential dump in a string -> reset the file.
        hasSequentialDumpBeenReset = true;
    }// else it has been initialized to false.
    Primes::DumpElement * lastRec = nullptr;// the very last record, deciphered from a func:recoverLastRecord
    long long LastOrdinal = 0UL;
    long long LastPrime = 0UL;
    std::ofstream localStreamWriter;
    if(nullptr!=stringDumpTail)
    {// i.e. if we have the string of dump-tail :
        lastRec = this->recoverLastRecord( stringDumpTail);// to be deleted.
        if(nullptr != lastRec)// if NOT null the lastRec
        {// the dump tail string content was valid.
            LastOrdinal = lastRec->ordinal;
            LastPrime = lastRec->prime;
            hasSequentialDumpBeenReset = false;
        }// else : stringDumpTail is not valid, i.e. sequentialDump needs a reset.
        else
        {// the sequential dump-tail is readable but not valid  -> reset the file.
         // start a new dump from scratch :
            LastOrdinal = 0;
            LastPrime = 0;
            localStreamWriter.open(*(this->sequentialDumpPath), std::fstream::out);// overWrite a previous invalid sequentialDump.
            //this->append_Sequential_Stream = new std::ofstream( *(this->sequentialDumpPath), std::fstream::out);// overWrite.
            hasSequentialDumpBeenReset = true;
        }
        // append to a valid previous sequentialDump:
        localStreamWriter.open(*(this->sequentialDumpPath), std::fstream::out | std::fstream::app);
    }// else sequentialFile not present.
    else
    {//  hasSequentialDumpBeenReset
        hasSequentialDumpBeenReset = true;
        // start a new dump from scratch :
        LastOrdinal = 0;
        LastPrime = 0;
        localStreamWriter.open( *(this->sequentialDumpPath), std::fstream::out);// reset.
    }
    if( localStreamWriter.is_open())
    {
        //---call with appropriate params---------
        this->Start_PrimeDump_FileSys(
                                      LastPrime
                                      , Threshold
                                      , &localStreamWriter
                                      , LastOrdinal );
        localStreamWriter.flush();
        localStreamWriter.close();
    }
    // clean
    delete stringDumpTail;
    delete lastRec;
    Common::LogWrappers::SectionClose();
    // ready.
    return hasSequentialDumpBeenReset;
}// SequentialCalcInterface


bool Primes::ReadSequentialDumpInterface_nextRec( long long acquireRecordNextToOffset)
{
    Common::LogWrappers::SectionOpen("Primes::ReadSequentialDumpInterface_nextRec", 0);
    std::ifstream localReader;
    if(nullptr==this->sequentialDumpPath)
    {
        this->feed_sequentialDumpPath();
    }// else ready;
    localReader.open( *this->sequentialDumpPath, std::fstream::in);
    if( ! localReader.is_open() )
    {
        return false;
    }// else continue
    bool res = false;
    long long sequentialDump_size = this->sequentialStreamSize();
    if( acquireRecordNextToOffset >= sequentialDump_size
        // ||  TODO evaluate additional error conditions
       )
    {// cannot operate.
        res = false;
        return res;
    }
    //---acquireNextRecord----START
    // pass parameter: file-offset.
    Primes::AsinglePointInStream * nextRecord = this->acquireNextRecord( acquireRecordNextToOffset);
    if( nullptr != nextRecord)
    {
        std::cout<<"\n\t acquireNextRecord("<<acquireRecordNextToOffset<<") reads:";
        std::cout<<"\n\t Prime["<<nextRecord->Ordinal<<"]=="<<nextRecord->Prime;
        std::cout<<"\n\t startPositionOfRecord=="<<nextRecord->startPositionOfRecord;
        std::cout<<"\n\t endPositionOfRecord=="<<nextRecord->endPositionOfRecord;
        std::cout<<"\n\n\n";
        delete nextRecord;
        res = true;
    }
    else
    {// cannot operate.
        res = false;
        return res;
    }//---acquireNextRecord----END
    localReader.close();
    Common::LogWrappers::SectionClose();
    // ready
    return res;
}// ReadSequentialDumpInterface_lastRec


bool Primes::ReadSequentialDumpInterface_arrayOfRec_anywhere( long long recArray_START, long long recArray_END)
{
    Common::LogWrappers::SectionOpen("Primes::ReadSequentialDumpInterface_arrayOfRec_anywhere", 0);
    bool res = false;
    long long sequentialDump_size = this->sequentialStreamSize();
    if( recArray_START >= sequentialDump_size
        || recArray_START < 0
        // ||  TODO evaluate additional error conditions
       )
    {// cannot operate.
        res = false;
        return res;
    }// else continue.
    //---------acquireSequenceOfRecord----START
    int cardinalityOfRecordSequence = 0;
    Primes::DumpElement * recSequence = this->acquireSequenceOfRecord(
        recArray_START
        , recArray_END
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
    else
    {// cannot operate.
        res = false;
        return res;
    }
    delete[] recSequence;
    Common::LogWrappers::SectionClose();
    //----------------acquireSequenceOfRecord------END
    // ready
    return res;
}//ReadSequentialDumpInterface_arrayOfRec_anywhere



bool Primes::RandomCalcInterface( unsigned long long infLeft, unsigned long long maxRight )
{
    Common::LogWrappers::SectionOpen("Primes::RandomCalcInterface", 0);
    bool res = false;
    //
    // NB. no {dumpTailReader, recoverLastRecord,...} -> work in [infLeft, maxRight].
    Entity::Integration::FunctionalForm LogIntegral = internalAlgos::LogIntegral_coChain;// function pointer.
    Primes::LogIntegralPillarPoint * nearestIntegral = this->getNearestIntegral( infLeft);
    unsigned long long threshold_lastIntegral = nearestIntegral->threshold;
    unsigned long long measure_lastIntegral = nearestIntegral->logIntegral;
    delete nearestIntegral;// clean
    long double LogIntegral_lastMile =
        Entity::Integration::trapezi(
                 (long double)threshold_lastIntegral// start from last integral saved.
                 , (long double)infLeft   // get to infleft
                 , (long double)1000  // test 1000 steps
                 , LogIntegral ); // func-pointer
unsigned long long extimatedOrdinal= (unsigned long long)( measure_lastIntegral + LogIntegral_lastMile);//TODO test
//unsigned long long extimatedOrdinal= 0;//TODO test
    // the first integer analyzed will be infLeft+1; the last will be "maxRight" parameter.
    // write a stamp, about what we're doing and when.
    time_t ttime = time(0);
    char* dt = ctime(&ttime);
    //   NB. for UTC Greenwich tm *gmt_time = gmtime(&ttime);
    //   NB. for UTC Greenwich dt = asctime(gmt_time);
    // use the member randomDumpPath which represents the customDump.
    std::ofstream localStreamWriter( *(this->randomDumpPath) , std::fstream::out | std::fstream::app);
    if( localStreamWriter.is_open() )
    {
        localStreamWriter << "\n\n Custom Interval ("<<infLeft<<", "<<maxRight<<"] ,worked on: "<<dt; //test<<"\n";
        localStreamWriter << " Ordinals are extimated by LogIntegral; so the ordinal appears usually bigger than the correct one.\n";
        // ---call with params
        Start_PrimeDump_FileSys( infLeft, maxRight, &localStreamWriter, extimatedOrdinal );
        localStreamWriter.flush();
        localStreamWriter.close();
    }
    else
    {
        res = false;
        std::cout<<"\n\t Unable to open Random Dump. \n";
    }
    Common::LogWrappers::SectionClose();
    // ready.
    return res;
}// RandomCalcInterface




const std::string * Primes::feed_sequentialDumpPath() // non const : feeds a member.
{// retrieve sequential-file fullpath.
    if( nullptr==this->sequentialDumpPath )
    {
        std::string * tmp = new std::string("primeDefaultFile");// sequential file section-name.
        this->sequentialDumpPath = this->getConfigSectionContent( tmp);// Default Section Name.
        delete tmp;
    }//else ready.
    return this->sequentialDumpPath;
}// retrieve sequential-file fullpath.

const std::string * Primes::feed_customDumpPath() // non const : feeds a member.
{// retrieve custom-file fullpath (i.e. the random-interval file).
    if( nullptr==this->randomDumpPath )
    {
        std::string * tmp = new std::string("primeCustomFile");// random-custom file section-name.
        this->randomDumpPath = this->getConfigSectionContent( tmp);// CUSTOM Section Name, for non complete dumping.
        delete tmp;
    }//else ready.
    return this->randomDumpPath;
}// retrieve custom-file fullpath (i.e. the random-interval file).

const std::string * Primes::feed_meshSpecificationPath() // non const : feeds a member.
{// retrieve the mesh-specification-fullpath (i.e. the file for mesh-renewal).
    if( nullptr==this->meshRenewalPath )
    {
        std::string tmp("meshRenewalFile");// mesh-renewal file section-name.
        this->meshRenewalPath = this->getConfigSectionContent( &tmp );// mesh-renewal file section-name.
        // in this auto-variable case -> no del delete tmp;
    }//else ready.
    return this->meshRenewalPath;
}// retrieve  mesh-renewal file.

const std::string * Primes::feed_localIntegralPath() // non const : feeds a member.
{// retrieve the local-integral-fullpath (i.e. the LogIntegral between real pillars).
    if( nullptr==this->localIntegralPath )
    {
        std::string * tmp = new std::string("localIntegralFile");//LogIntegral btw real pillars section-name.
        this->localIntegralPath = this->getConfigSectionContent( tmp);//LogIntegral btw real pillars section-name.
        delete tmp;
    }//else ready.
    return this->localIntegralPath;
}// retrieve the local-integral-fullpath

const std::string * Primes::feed_globalIntegralPath() // non const : feeds a member.
{// retrieve the global-integral-fullpath (i.e. the LogIntegral from +2 to real pillars).
    if( nullptr==this->globalIntegralPath )
    {
        std::string * tmp = new std::string("globalIntegralFile");//the LogIntegral from +2 to real pillars
        this->globalIntegralPath = this->getConfigSectionContent( tmp);//the LogIntegral from +2 to real pillars
        delete tmp;
    }//else ready.
    return this->globalIntegralPath;
}// retrieve the global-integral-fullpath


const std::string * Primes::getConfigSectionContent( const std::string * sectionNameInFile) const
{// the param is the desired section-name; the retval is the section-content.
    Common::ConfigurationService * primeNamedConfig =// config name is hard-coded.
        new Common::ConfigurationService( "./PrimeConfig.txt");// Prime-configuration-file. All in this file.
    const std::string * desiredSectionContent = // feed the return value.
        primeNamedConfig->getValue( *sectionNameInFile);// configSectionNames can be added.
    delete primeNamedConfig;// the Config instance is no more useful.
    return desiredSectionContent;// caller has to delete.
}// getPrimeDumpFullPath


/// Dtor()
Primes::~Primes()
{/// Dtor()
    // don't log from Dtor: for auto-instances it leaks  Common::LogWrappers::SectionOpen("Dtor Primes::~Primes", 0);
    if( nullptr != this->memoryMappedDump)
    {// map of couples {Ordinal,Prime}
        this->memoryMappedDump->clear();
        delete this->memoryMappedDump;
        this->memoryMappedDump = nullptr;// not dangling.
    }// else already nulled.
    if( nullptr != this->logIntegralPillars)
    {// map of couples {Threshold, LogIntegral[Threshold]}
        this->logIntegralPillars->clear();
        delete this->logIntegralPillars;
        this->logIntegralPillars = nullptr;// not dangling.
    }// else already nulled.
    if( nullptr != this->sequentialDumpPath )
    {
        delete this->sequentialDumpPath;
        this->sequentialDumpPath = nullptr;
    }
    if( nullptr != this->randomDumpPath )
    {
        delete this->randomDumpPath;
        this->randomDumpPath = nullptr;
    }
    if( nullptr != this->meshRenewalPath )
    {
        delete this->meshRenewalPath;
        this->meshRenewalPath = nullptr;
    }
    if( nullptr != this->localIntegralPath )
    {
        delete this->localIntegralPath;
        this->localIntegralPath = nullptr;
    }
    if( nullptr != this->globalIntegralPath )
    {
        delete this->globalIntegralPath;
        this->globalIntegralPath = nullptr;
    }
    // NB don't log from Dtor: for auto-instances it leaks Common::LogWrappers::SectionClose();
}// Dtor




void Primes::createOrAppend( const std::string * fullPath)
{
    std::ofstream createOrApp( *fullPath, std::fstream::out | std::fstream::app);
    createOrApp.close();
}// dumpFile createOrAppend


// SEEKER
const std::string * Primes::lastRecordReaderByChar( const std::string * fullPath)
{
    Common::LogWrappers::SectionOpen("Primes::lastRecordReaderByChar", 0);
    std::string * directTailDump = new std::string();// retval
    std::ifstream lastrecReader( *fullPath, std::fstream::in );// one day we will parse the tail of CustomDump too.
    long long streamSize =  this->sequentialStreamSize();
    //--step back in the Stream, of Max(100, fileSize).
    int stepBack = 100;// init
    if( streamSize < 0)
    {
        return nullptr;
    }
    if( streamSize <stepBack)
    {
        stepBack = streamSize;
    }// else stay as init==100.
    Common::StringBuilder * sb = new Common::StringBuilder( stepBack);// forecasted size.
    lastrecReader.seekg( -1*stepBack, lastrecReader.end);// get in place to read last char[stepBack].
    int currentPosition;
    int step = 0;
    // DBG int howManyLineEndings = 0;
    for( char c=0; ! lastrecReader.eof(); )
    {
        lastrecReader.get( c);
        sb->append(c);
        step++;
        currentPosition = lastrecReader.tellg();
        if( currentPosition > streamSize)
        {
            Common::LogWrappers::SectionContent("Reached EOF in lastRecordReaderByChar", 0);
        }// else continue.
    }// for
    directTailDump->assign( sb->str() );
    delete sb;// clean up the temporary StringBuilder.
    Common::LogWrappers::SectionClose();
    // ready
    return directTailDump;// caller has to delete
}// lastRecordReaderByChar

// SEEKER
long long Primes::sequentialStreamSize()
{
    if( nullptr == this->sequentialDumpPath)
    {
        this->feed_sequentialDumpPath();
    }// else ready.
    std::ifstream lastrecReader( *(this->sequentialDumpPath), std::fstream::in );// auto
    lastrecReader.seekg( -1, std::ios::end );
    long long streamSize = lastrecReader.tellg();
    lastrecReader.close();
    return streamSize;
}// sequentialStreamSize


// SEEKER
const std::string * Primes::dumpTailReaderByChar( const std::string * fullPath)
{// reads multiple records from EOF back to Max(filesize,100).
    Common::LogWrappers::SectionOpen("Primes::dumpTailReaderByChar", 0);
    std::ifstream lastrecReader( *fullPath, std::fstream::in );
    long long streamSize = this->sequentialStreamSize();
    // reads multiple records from EOF back to Max(filesize,100).
    int stepBack = 100;
    if( streamSize < 0)
    {
        return nullptr;
    }
    if( streamSize <stepBack)
    {
        stepBack = streamSize;
    }// else stay as init.
    lastrecReader.seekg( -1*stepBack, lastrecReader.end);// get in place to read last char[stepBack].
    Common::StringBuilder * sb = new Common::StringBuilder( stepBack);// forecasted size.
    int currentPosition;
    int step = 0;
    // DBG int howManyLineEndings = 0;
    for( char c=0; ; ) //  read_file from -stepBack to EOF.
    {
        lastrecReader.get( c);
        sb->append(c);
        step++;
        currentPosition = lastrecReader.tellg();
        if( currentPosition > streamSize
           || lastrecReader.eof()
           )
        {
            Common::LogWrappers::SectionContent("reached EOF in dumpTailReaderByChar", 0);
            break;
        }// else continue.
    }// for
    std::string * sequentialFile_tail = new std::string( sb->str() ); // caller has to delete!
    delete sb;// clean up the temporary StringBuilder.
    lastrecReader.close();// auto Stream close.
    Common::LogWrappers::SectionClose();// Log
    // ready.
    return sequentialFile_tail;// caller has to delete
}// dumpTailReaderByChar()



//  IntegerDecomposition : the Fundamental Thm of Arithmetic.
Primes::SingleFactor * Primes::IntegerDecomposition( const unsigned long long dividend)
{
    Entity::Integration::FunctionalForm LogIntegral = internalAlgos::LogIntegral_coChain;// function pointer.
    double LogIntegral_ofInfPar = Entity::Integration::trapezi( +2.0, (double)dividend, ((double)dividend-2.0)*4, LogIntegral );
    unsigned long long ordinaleStimato = (unsigned long)LogIntegral_ofInfPar;// approx eccesso: LogIntegral[Soglia]==LastOrdinal_under_Soglia==Cardinalita[sottoSoglia].
    SingleFactor * factorization = new SingleFactor[ordinaleStimato];// stimare #fattoriMaximal.
    // Oss. greatest involved-prime==dividend/2 in a composite, since greatestFactor is the cofactor of the PotentialSmallest(i.e. 2).
    for( unsigned long long c=0; c<ordinaleStimato; c++)
    {// init to zeroContentMemory.
        factorization[c].factorBase = 0;
        factorization[c].factorMultiplicity = 0;
    }
    // TODO readRange( 1, ordinaleStimato);
    unsigned long long * involvedPrimes = new unsigned long long[ordinaleStimato];
    for(unsigned long long c=0; c<ordinaleStimato; c++)
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


Primes::DumpElement * Primes::recoverLastRecord( const std::string * dumpTail)
{
    Common::LogWrappers::SectionOpen("Primes::recoverLastRecord",0);
    Primes::DumpElement * lastRecord = new Primes::DumpElement();// retval

     std::string parFromFile( *dumpTail);
    int inputParamLength = parFromFile.length();
    char lastCharOfTail = parFromFile[inputParamLength-1];
    if(
       lastCharOfTail != (char)10
       && lastCharOfTail != (char)13
       )
    {
        parFromFile.append("\n");// add in RAM in the string
        std::ofstream addNewLineAtEOF( *(this->sequentialDumpPath), std::fstream::app );
//addNewLineAtEOF.open( *(this->sequentialDumpPath), std::fstream::app );
//( *(this->sequentialDumpPath), std::fstream::out | std::fstream::app);
//this->append_Sequential_Stream = new std::ofstream
        addNewLineAtEOF.put('\n');// add on disk
        addNewLineAtEOF.flush();
        addNewLineAtEOF.close();
    }// else the sequentialDump already ends with a "newline" (either Unix or Win)
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
        }
        else
        {// else the token is not interesting; it was included in the read portion, but is not part of the last row.So skip when i>2
            break;
        }
    }// the two interesting semi-tokens are the last two; so the reading is in reverse order.
    delete tokenArray;// TODO test
    Common::LogWrappers::SectionClose();
    // ready.
    return lastRecord;// caller has to delete.
}// recoverLastRecord


// recoverDumpTail : produce an array of couples {ordinal,prime} from a String : dumpTail_String.
Primes::DumpElement * Primes::recoverDumpTail( const std::string * dumpTail_String , int *recordArrayCardinality)
{
    DumpElement * dumpTail_Records = nullptr;// ret val
    const std::string parFromFile( *dumpTail_String );
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
    delete tokenArray;// clean.
    //ready.
    return dumpTail_Records;// caller has to delete.
}// recoverDumpTail


// state of the art.
void Primes::Start_PrimeDump_FileSys(
        unsigned long long Left
        ,unsigned long long Right
        ,std::ofstream * appendStream
        ,unsigned long long ordinal // passed as real xor extimated ordinal of "Left" i.e. Left==Prime[ordinal]
    )
{
    bool isStillPrime = true;
    long double realQuotient;
    unsigned long long intQuotient;
    unsigned long long cursor = Left+1UL;// start stepping from the Int after the last found Prime.
    if( cursor<+2){cursor=+2;}// 1 (i.e. one) is the product-invariant; so, not a prime.
    // NB now a data-member ; ofstream appendStream( this->sequentialDumpPath, std::fstream::out | std::fstream::app);
    //
    for( ; cursor<=Right; cursor++)//NB. cursor==dividend.
    {
        long double soglia = sqrt( cursor);// division is a two-operand operator: the bisection of dividend is Sqrt[dividend]
        // when dividend/Sqrt[dividend]==Sqrt[dividend] and when dividend/(Sqrt[dividend]+eps)<Sqrt[dividend]
        // so the stepping into divisor>Sqrt[dividend] leads to divisors<Sqrt[dividend] which have already been explored.
        unsigned long long divisor=+2;
        for( ; divisor<=soglia; divisor++)
        {
            realQuotient = (long double)cursor/(long double)divisor;
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
}//  IntegralFileFromStartFSproducer


// SEEKER
Primes::AsinglePointInStream * Primes::acquireNextRecord( unsigned long long discriminatingElement_position)
{
    std::ifstream localReader;
    if(nullptr==this->sequentialDumpPath)
    {
        this->feed_sequentialDumpPath();
    }// else ready;
    localReader.open( *this->sequentialDumpPath, std::fstream::in);
    if( ! localReader.is_open() )
    {
        return nullptr;
    }// else continue
    localReader.seekg( discriminatingElement_position , std::ios::beg );// TODO test
    AsinglePointInStream * nextRecord = nullptr;// retval; caller has to delete.
    int c=0;
    int stepDone = 0;
    for( ; ; stepDone++ )
    {// for step until next-record starting point.
        if(0==discriminatingElement_position){break;}// cannot go backwards and it's surely the beginning of a record( the first one).
        // cannot check if one byte before there's end-of-record;
        if( ! localReader.good()) {break;}
        c = localReader.get();
        if( 13==c || 10==c )
        {
            break;
        }// next record begin-position is set
        if( localReader.eof() ) {break;}
    }// for step until next-record starting point.
    // then write down next complete record :
    Common::StringBuilder sb(50);// estimate a single record's length (in excess).
    int terminator=0;
    int midRecord_separator=0;
    for( stepDone = 0;  ; stepDone++ )
    {
        if( ! localReader.good()) {break;}
        c = localReader.get();
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
            const std::string * nextRecord_txt = new std::string( sb.str() );
            // DBG int nextRecord_len = nextRecord_txt->length();
            int recordCardinality=0;
            Primes::DumpElement * nextRecPtr =
                recoverDumpTail( nextRecord_txt , &recordCardinality);
            delete nextRecord_txt;
            // only now allocate; if func does not pass here -> ret null.
            nextRecord = new Primes::AsinglePointInStream();
            nextRecord->startPositionOfRecord = discriminatingElement_position;
            nextRecord->endPositionOfRecord = discriminatingElement_position + stepDone;
            nextRecord->Ordinal = nextRecPtr->ordinal;
            nextRecord->Prime = nextRecPtr->prime;
            delete[] nextRecPtr;// clean.
            localReader.clear();// reset in case EOF or badRead has been set.
            break;
        }
    }// for step into next complete record
    localReader.close();
    //ready
    return nextRecord;// caller has to delete.
}// acquireNextRecord


// SEEKER
Primes::DumpElement * Primes::acquireSequenceOfRecord(
    unsigned long long discriminatingElement_position
    , unsigned long long until_position
    , int * howMany_RecordInSequence
                                        )
{
    Common::LogWrappers::SectionOpen(
        "acquireSequenceOfRecord", 0);
    std::ifstream localReader;
    if(nullptr==this->sequentialDumpPath)
    {
        this->feed_sequentialDumpPath();
    }// else ready;
    localReader.open( *this->sequentialDumpPath, std::fstream::in);
    if( ! localReader.is_open() )
    {
        return nullptr;
    }// else continue
    Common::LogWrappers::SectionContent_variable_name_value(
        "discriminatingElement_position ==", discriminatingElement_position, 0);
    localReader.seekg( discriminatingElement_position , std::ios::beg );// TODO test
    DumpElement * sequenceRecord = nullptr;
    int c=0;
    int stepDone = 0;
    for( ; ; stepDone++ )
    {// for step until next-record starting point.
        if(0==discriminatingElement_position){break;}// cannot go backwards and it's surely the beginning of a record( the first one).
        // cannot check if one byte before there's end-of-record;
        if( ! localReader.good()) {break;}
        c = localReader.get();
        if( 13==c || 10==c )
        {
            break;
        }// next record begin-position is set
        if( localReader.eof() ) {break;}
    }// for step until next-record starting point. Steps done until here cannot be counted, since they are before next record.
    // then write down next complete record :
    Common::StringBuilder sb(until_position-discriminatingElement_position);// estimate
    int terminator=0;
    int midRecord_separator=0;
    for( stepDone = 0;  ; stepDone++ )// start counting steps from here:
    {
        if( ! localReader.good()) {break;}
        c = localReader.get();
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
        if( ( discriminatingElement_position + stepDone >= until_position // TODO test : reached endpoint
              && (13==c || 10==c) )// as additional condition, step until the end of current record
             || localReader.eof()// this alone is a sufficient condition.
           )
        {
            const std::string * sequenceOfRecord_txt = new std::string( sb.str() );
            // DBG int sequenceOfRecord_len = sequenceOfRecord_txt.length();
            // howMany_RecordInSequence is an out parameter, coming from the caller of this func
            sequenceRecord =
                recoverDumpTail( sequenceOfRecord_txt , howMany_RecordInSequence );
            localReader.clear();// reset in case EOF or badRead has been set.
            delete sequenceOfRecord_txt;// clean the temporary.
            break;
        }
    }// for step into next complete record
    localReader.close();
    Common::LogWrappers::SectionContent_variable_name_value(
        "howMany_RecordInSequence ==", (signed long long)(*howMany_RecordInSequence),0);
    Common::LogWrappers::SectionClose();
    //ready
    return sequenceRecord;// caller has to delete.
}// acquireSequenceOfRecord


template<typename It>
void printInsertionStatus(It it, bool success)
{
    std::cout << "Insertion of " << it->first << (success ? " succeeded\n" : " failed\n");
}

bool Primes::MoveToMap(
    long long      discriminatingElement_position
    , long long    until_position
    , int *                 howMany_RecordInSequence
               )
{
    Common::LogWrappers::SectionOpen("MoveToMap",0);
    bool res = true;
    Common::LogWrappers::SectionContent_variable_name_value(
        "discriminatingElement_position ==", discriminatingElement_position, 0);
    if( discriminatingElement_position < 0)
    {// cannot go back from origin.
        discriminatingElement_position = 0;
    }// else ok.
    Primes::DumpElement * tmpStorage = acquireSequenceOfRecord( // to be delete[]
         discriminatingElement_position
         ,until_position
         ,howMany_RecordInSequence
        );
    if( nullptr==tmpStorage)
    {
        res = false;
        return res;
    }//else continue.
    for( int c=0; c< (*howMany_RecordInSequence); c++)
    {// std::move() from array[] to map<>
        std::pair<unsigned long long, unsigned long long> OrdinalPrime(
            std::move( tmpStorage[c].ordinal)
            ,std::move( tmpStorage[c].prime)  // TODO test!
            );// TODO
    if(0==this->memoryMappedDump->count(OrdinalPrime.first))// which means the key is NOT present
    {
        //const auto [it_hinata, success] = this->memoryMappedDump->insert( OrdinalPrime);// >assign( OrdinalPrime->second);
        const std::pair<std::_Rb_tree_iterator<std::pair<const long long unsigned int, long long unsigned int> >,bool> tmpNodeToBeInserted =
            this->memoryMappedDump->insert( OrdinalPrime);// >assign( OrdinalPrime->second);
        printInsertionStatus(tmpNodeToBeInserted.first, tmpNodeToBeInserted.second );
    }// else skip existing Ordinal.
    }//for
    // clean the pointer, to not let it dangling, but do not delete, since the pointee
    // hase ben moved::.
    if( nullptr != tmpStorage)
    {
        delete[] tmpStorage;
    }// TEST
    tmpStorage = nullptr;
    //
    res = true;
    Common::LogWrappers::SectionClose();//pop
    return res;
}// MoveToMap


bool Primes::Bisection( unsigned long long requiredOrdinal )
 {
    Common::LogWrappers::SectionOpen("Cantiere::Bisection", 0);
    Common::LogWrappers::SectionContent_variable_name_value(
        "---------------------------------------------------PARAMETER requiredOrdinal ==", requiredOrdinal, 0);
    Common::LogWrappers::SectionContent_variable_name_value(
        "----------------------------------------------PARAMETER sogliaDistanza ==", (unsigned long long)this->sogliaDistanza, 0);
    bool res = false;
    std::ifstream localReader;
    if(nullptr==this->sequentialDumpPath)
    {
        this->feed_sequentialDumpPath();
    }// else ready;
    localReader.open( *this->sequentialDumpPath, std::fstream::in);
    if( localReader.is_open() )
    {
        res = true;
    }// else stay false.
    else
    {// cannot operate.
        res = false;
        Common::LogWrappers::SectionContent("the Stream Reader is invalid.", 0);
        return res;
    }
    unsigned long long left = 0;
    unsigned long long previous_left = left;
    long long dumpSize = this->sequentialStreamSize();
    long long previous_dumpSize = dumpSize;
    unsigned long long right = dumpSize;
    unsigned long long previous_right = right;
    std::string * dumpSize_str = Common::StrManipul::uLongLongToString( dumpSize);
    Common::LogWrappers::SectionContent( ("___________________________dumpSize == " + *dumpSize_str).c_str() , 0);
    delete dumpSize_str;
    long long discriminatingElement_position;// let it signed, to avoid overflows.
    long signedDelta = this->sogliaDistanza*3;//init to any value, but not within threshold.
    unsigned long UNsignedDelta = this->sogliaDistanza*3;//init to any value, but not within threshold.
    Primes::AsinglePointInStream * nextRecord = nullptr;
    //
    unsigned Bisection_step = 1;
    for (;; Bisection_step++)// breaks on Bisection_failure_
    {
        Common::LogWrappers::SectionContent_variable_name_value(
            "----------------------------------Bisection_step # ", (unsigned long long)Bisection_step, 0);
        // acquire the first record, successive to the offset "discriminatingElement_position"
        discriminatingElement_position = (right-left)/2 + left;// ==(right+left)/2 == M1(left,right).
        Common::LogWrappers::SectionContent_variable_name_value(
            "discriminatingElement_position ==", discriminatingElement_position, 0);
        if( discriminatingElement_position<0)
        {// can't go back from the origin.
            discriminatingElement_position = 0;
        }// else ok.
        if( discriminatingElement_position > (long long)right )
        {// can't go past the end.
            discriminatingElement_position = right;// stay in the interval.
        }// else ok.
        Common::LogWrappers::SectionContent_variable_name_value(
            "discriminatingElement_position AFTER CORRECTIONS ==", discriminatingElement_position, 0);
        nextRecord = acquireNextRecord( discriminatingElement_position);// memory allocated by callee
        if( nullptr == nextRecord)
        {// cannot operate.
            res = false;
            Common::LogWrappers::SectionContent("cannot acquire Next Record", 0);
            return res;
        }
        Common::LogWrappers::SectionContent_variable_name_value(
            "nextRecord->Ordinal ==", nextRecord->Ordinal, 0);
        Common::LogWrappers::SectionContent_variable_name_value(
            "nextRecord->Prime ==", nextRecord->Prime, 0);
        // compare
        signedDelta = nextRecord->Ordinal - requiredOrdinal;
        UNsignedDelta = abs( signedDelta);
        if( UNsignedDelta <= this->sogliaDistanza)
        {// linear acquisition & move&& to map<>
            Common::LogWrappers::SectionContent("---Under Threshold---: linear acquisition & move&& to map<>",0);
            //log-size-for category.
            int currentRecordLength = nextRecord->endPositionOfRecord - nextRecord->startPositionOfRecord;
            // signed:(+)means landed right of obj
            // signed:(-)means landed left of obj
            long long  distance_from_Target_bytes = signedDelta * (currentRecordLength+2);// {Win=+2,Unix=+1}
            Common::LogWrappers::SectionContent_variable_name_value(
                "distance_from_Target_bytes ==", (signed long long)distance_from_Target_bytes, 0);
            // the minus sign in next statement is understandable by means of the previous two comments.
            long long extimated_target_position_bytes =
                discriminatingElement_position - distance_from_Target_bytes;
            Common::LogWrappers::SectionContent_variable_name_value(
                "extimated_target_position_bytes ==", extimated_target_position_bytes, 0);
            // grab an interval centered in extimated_target and wide twise threshold
            long long beg_RecArray = extimated_target_position_bytes -currentRecordLength*this->sogliaDistanza;
            if( beg_RecArray < 0)
            {// cannot go back from origin.
                beg_RecArray = 0;
            }// else ok.
            long long end_RecArray = extimated_target_position_bytes +currentRecordLength*this->sogliaDistanza;
            Common::LogWrappers::SectionContent_variable_name_value(
                "beg_RecArray ==", beg_RecArray, 0);
            Common::LogWrappers::SectionContent_variable_name_value(
                "end_RecArray ==", end_RecArray, 0);
            int howManyRecordInSequence;
            bool moveResult =
                MoveToMap(
                  beg_RecArray
                  , end_RecArray
                  , &howManyRecordInSequence
                 );// NB. the temporary array gets created and moved within the callee MoveToMap(). Nothing left here.
            Common::LogWrappers::SectionContent_variable_name_value(
                "moveResult ==", (long long)moveResult, 0);
            Common::LogWrappers::SectionContent_variable_name_value(
                "howManyRecordInSequence ==", (long long)howManyRecordInSequence, 0);
            break;//found within threshold -> exit (i.e. break)
        }//if( UNsignedDelta <= this->sogliaDistanza) i.e. if within threshold
        // else continue Bisection:
        // decide wether to retain left or right half
        if( signedDelta < 0) // nextRecord->Ordinal < requiredOrdinal)
        {// retain right
            Common::LogWrappers::SectionContent("retain right",0);
            left = nextRecord->endPositionOfRecord;  //acquireNextRecord_end;
            Common::LogWrappers::SectionContent_variable_name_value(
                "left ==", left, 0);
            right = previous_right;
            Common::LogWrappers::SectionContent_variable_name_value(
                "right ==", right, 0);
        }
        else if( signedDelta > 0) // nextRecord->Ordinal > requiredOrdinal)
        {// retain left
            Common::LogWrappers::SectionContent("retain left",0);
            left = previous_left;
            Common::LogWrappers::SectionContent_variable_name_value(
                "left ==", left, 0);
            right = nextRecord->startPositionOfRecord; // acquireNextRecord_start;
            Common::LogWrappers::SectionContent_variable_name_value(
                "right ==", right, 0);
        }
        else if( nextRecord->Ordinal == requiredOrdinal)
        {
            Common::LogWrappers::SectionContent("punctual convergence",0);
            std::pair<unsigned long long, unsigned long long> p(nextRecord->Ordinal,nextRecord->Prime);
            this->memoryMappedDump->insert( p);
            res = true;//done.
            break;//found -> exit (i.e. break)
        }// no other else possible.
        //---update loop variables-----
        previous_left = left;
        previous_right = right;
        dumpSize = right - left;
        Common::LogWrappers::SectionContent_variable_name_value(
            "dumpSize ==", dumpSize, 0);
        if( dumpSize >= previous_dumpSize )// check before updating.
        {
            Common::LogWrappers::SectionContent(
                "\n\n\t ############# Bisection Failure : the filesize is not decresing !! DBG needed !!!#######\n\n", 0);
            break;
        }// else continue.
        previous_dumpSize = dumpSize;// then update.
        Common::LogWrappers::SectionContent_variable_name_value(
            "---------END of Bisection_step #", (unsigned long long)Bisection_step, 0);
        //---clean up memory before next loop---------------
        delete nextRecord;
        nextRecord = nullptr;// cleanup stepWise-record.
    }// for
    if( nullptr != nextRecord)
    {// when a break is hit, the cleanup has to be guaranteed, if not yet happened.
        delete nextRecord;
    }// else the delete already took place.
    //---close Reader
    localReader.close();
    Common::LogWrappers::SectionClose();
    // ready
    return res;
 }// Bisection

unsigned long long Primes::queryMap( unsigned long long desiredOrdinal)
{// the Map contains Pairs{Ordinal,Prime}. So the zero is available as error code.
    if(+1==this->memoryMappedDump->count( desiredOrdinal))// which means te key is present
    {//if(nullptr!=(*dictionary).operator[]( requiredkey)) DON'T :this inserts a new pair.
        return this->memoryMappedDump->at( desiredOrdinal);//NB. right way to search the value of a key.
        //(*dictionary).operator[]( requiredkey)->internalPrint(); do NOT use operator[] ,which is a writer.
    }// else return zero, as error-code, since the required key is absent in the map.
    else
    {// else means count==0. In the map count has only the states{0==absent, +1==present}.
        std::cout<<"----\n\t Key "<<desiredOrdinal<<" not found."; // DBG
        return 0;
    }
}// queryMap

unsigned long long Primes::operator[]( unsigned long long desiredOrdinal )
{
    unsigned long long  desiredPrime = this->queryMap( desiredOrdinal);// check if there's already the record in Map.
    if( 0 != desiredPrime)
    {
        std::cout<<"-------\n\t Key FOUND in map. Prime["<<desiredOrdinal<<"]=="<< desiredPrime<<" \n----------";
        return desiredPrime;
    }
    else // zero returned by queryMap means key-absent.
    {// try to feed the Map.
        this->Bisection( desiredOrdinal
                        // NB. soglia distanza is an instance-member.
                );
    }
    // try again to ask the Map, after feeding it.
    desiredPrime = this->queryMap( desiredOrdinal);
    if( 0 == desiredPrime)
    {
        std::cout<<"\n\t Key not found AGAIN, after feeding it. DEBUG needed: exceptional case. \n----------"; // DBG
    }// DBG !
    else
    {
        std::cout<<"\t Key FOUND, after feeding it. Prime["<<desiredOrdinal<<"]=="<< desiredPrime<<" \n----------";
    }// DBG !
    //
    return desiredPrime;
}// operator[]

void Primes::coveringIntegral()
{
    unsigned long long sogliaCustom = -1;// reach it by underflow. 1.8447*10^19-1
    struct LogIntegralStep
    {
        long double inf;
        long double sup;
        long double card_partiz;
    };
    LogIntegralStep * LogIntegralStep_Array = new LogIntegralStep[7];
    //----init
    LogIntegralStep_Array[0].inf = +2.0;
    LogIntegralStep_Array[0].sup = +100.0;
    LogIntegralStep_Array[0].card_partiz = +400.0;
    //-
    LogIntegralStep_Array[1].inf =  +100.0;
    LogIntegralStep_Array[1].sup = +1000.0;
    LogIntegralStep_Array[1].card_partiz = +2000.0;
    //-
    LogIntegralStep_Array[2].inf = +1000.0;
    LogIntegralStep_Array[2].sup = pow(10,6);
    LogIntegralStep_Array[2].card_partiz = +2000.0;
    //-
    LogIntegralStep_Array[3].inf = pow(10,6);
    LogIntegralStep_Array[3].sup = pow(10,9);
    LogIntegralStep_Array[3].card_partiz = +1000.0;
    //-
    LogIntegralStep_Array[4].inf = pow(10,9);
    LogIntegralStep_Array[4].sup = pow(10,12);
    LogIntegralStep_Array[4].card_partiz = +9000.0;
    //-
    LogIntegralStep_Array[5].inf = pow(10,12);
    LogIntegralStep_Array[5].sup = pow(10,15);
    LogIntegralStep_Array[5].card_partiz = +9000.0;
    //-
    LogIntegralStep_Array[6].inf = pow(10,15);
    LogIntegralStep_Array[6].sup = sogliaCustom; // 1.8447*10^19-1
    LogIntegralStep_Array[6].card_partiz = +9000.0;
    //-
    std::ofstream logIntegral("./LogIntegral_firstPhase_.txt", std::fstream::out);// reset.
    std::string colonneStr("inf \t sup \t LogIntegral(inf,sup) \n");
    logIntegral.write( colonneStr.c_str(), colonneStr.length() );
    Entity::Integration::FunctionalForm LogIntegral = internalAlgos::LogIntegral_coChain;// function pointer.
    for( int c=0; c<=6; c++ )
    {// integrate and dump the covering intervals.
        long double quantileLogIntegral =
            Entity::Integration::trapezi(
                                         LogIntegralStep_Array[c].inf
                                         ,LogIntegralStep_Array[c].sup
                                         ,LogIntegralStep_Array[c].card_partiz // how many steps
                                         , LogIntegral );// function-pointer
        std::string * infStr = Common::StrManipul::uLongLongToString( LogIntegralStep_Array[c].inf);
        std::string * supStr = Common::StrManipul::uLongLongToString( LogIntegralStep_Array[c].sup);
        std::string * LogIntegralStr = Common::StrManipul::uLongLongToString( (unsigned long long)quantileLogIntegral);
        int forecastedTokenSize = 100;
        Common::StringBuilder * strBuild = new Common::StringBuilder( forecastedTokenSize);
        strBuild->append(infStr->c_str());
        strBuild->append("_");
        strBuild->append(supStr->c_str());
        strBuild->append("_");
        strBuild->append(LogIntegralStr->c_str());
        strBuild->append("\n");// choose '\n'
        delete infStr;
        delete supStr;
        delete LogIntegralStr;
        // instead of returning it, dump it on the file.
        logIntegral.write( strBuild->str().c_str(), strBuild->str().length() );
        delete strBuild;// clean up the token-buffer.
        strBuild = nullptr;
    }// for
    logIntegral.flush();
    logIntegral.close();
    delete[] LogIntegralStep_Array;
}// coveringIntegral

//
//bool Primes::distributionFunction(const char * fullPath)
//{
//    std::fstream testFile;
//    bool result = false;// init to invalid.
//    std::vector <std::string> data;
//    std::vector<std::string>::iterator iter;
//    std::string curr_data;
//    // Open for read : Input
//	testFile.open( fullPath, std::ios::in);
//	int step = 1;
//    if (testFile.is_open())
//    {
//        while (!testFile.eof())
//        {
//            if( testFile.eof() ) {break;}
//            std::getline ( testFile, curr_data);// legge con separatore EOL : TODO test if '\n' or '\r\n'
//            if( curr_data.length() > 0)
//            {
//                data.push_back(curr_data);// push the read line in a list.
//            }// else skip empty entry.
//        }
//        testFile.close();
//        result = true;
//    }// else result remains false; end File-read loop.
//    //
//    unsigned long long cumulate = 0;
//    step = 1;
//    std::ofstream secondPhase("./20221013_LogIntegral_secondPhase_.txt");
//    if( secondPhase.is_open())
//    {
//        result = true;
//        secondPhase<<"Offset LogIntegral == Integrate[1/Log[t], {t,+2,x}] \n";
//        secondPhase<<"x		\t	LogIntegral \n";
//        secondPhase<<"----------------------------------------------------------------------\n";
//    }
//    else
//    {// file not open.
//        result = false;
//    }
//    std::vector<std::string> * tokenizedLine = nullptr;
//    for (iter = data.begin(); iter != data.end(); iter++)
//    {
//       if( data.end() == iter){break;}
//       if( step >+1)
//       {
//           std::string tmp( *iter);
//           tokenizedLine = Common::StrManipul::stringSplit(
//            "_"
//            , tmp  // NB. original passed by value, to be preserved.
//            , true );
//           if( (*tokenizedLine).size() >= 2)
//           {
//               const std::string LogIntegral_inf_sup_( (*tokenizedLine)[2] );
//               cumulate += Common::StrManipul::stringToUnsignedLongLong( LogIntegral_inf_sup_);// check if exists
//               secondPhase<<(*tokenizedLine)[1]<<"\t"<< cumulate<<"\n";// dump Distribution[+2,x] on file.
//               // dump in RAM::Map : this->logIntegralPillars
//               std::pair<unsigned long long, unsigned long long> LogIntegralSinglePillar(
//                    Common::StrManipul::stringToUnsignedLongLong( (*tokenizedLine)[1] )
//                    ,cumulate
//               );
////                const std::pair<std::_Rb_tree_iterator<std::pair<const long long unsigned int, long long unsigned int> >,bool> tmpNodeToBeInserted =  unnecessary here
//               this->logIntegralPillars->insert( LogIntegralSinglePillar);
//           }// else skip
//           delete tokenizedLine;
//       }
//       step++;
//    }
//    secondPhase.flush();
//    secondPhase.close();
//    // ready.
//    return result;
//}// distributionFunction

bool Primes::distributionFunction_fromExistingMesh()
{
    bool result = false;
    this->feed_globalIntegralPath();
    std::fstream globalIntegralFile( *this->globalIntegralPath, std::ios::in);
    std::string curr_line;
    int lineOrdinal = 0;

    if (globalIntegralFile.is_open())
    {
        while (!globalIntegralFile.eof())
        {
            if( globalIntegralFile.eof() ) {break;}
            std::getline ( globalIntegralFile, curr_line);// legge con separatore EOL : TODO test if '\n' or '\r\n'
            lineOrdinal++;
            if( lineOrdinal >+3)// skip the headers (they ar 3 and we count lines here from 0)
            {
                if( curr_line.length() > 0)
                {
                    std::vector<std::string>  * aLine = Common::StrManipul::stringSplit("\t",curr_line,true);
                    int tokenInCurLine = aLine->size();
                    if( tokenInCurLine >= +2)// two token required
                    {
                        LogIntegralPillarPoint currentPillar;
                        currentPillar.threshold = Common::StrManipul::stringToUnsignedLongLong((*aLine)[0]);
                        currentPillar.logIntegral = Common::StrManipul::stringToUnsignedLongLong((*aLine)[1]);
                        delete aLine;
                        this->logIntegralPillars->push_back( currentPillar);
                    }// else : invalid line: not enough tokens.
                }// else skip empty entry.
            }// skip the headers
        }// while !EOF
        globalIntegralFile.close();
        result = true;
    }// else not opened.
    else
    {
        result = false;
    }
    // ready.
    return result;
}// distributionFunction_fromExistingMesh




unsigned long long Primes::interpolateOrdinal( unsigned long long candidatePrimeThreshold)
{
    //
    //2                       0
    //100                     29
    //1000                    176
    //1000000                 78626
    //1000000000              50849654
    //1000000000000           37607953088
    //1000000000000000        29844572821462
    //18446744073709551615    425656551648260822
    //
    struct PillarPoint
    {
        unsigned long long abscissa;
        unsigned long long ordinate;
    };
    PillarPoint * thePillarPoints = new PillarPoint[8];
    //
    thePillarPoints[0].abscissa = +2;
    thePillarPoints[0].ordinate =  0;
    //
    thePillarPoints[1].abscissa = +100;
    thePillarPoints[1].ordinate =  +29;
    //
    thePillarPoints[2].abscissa = +1000;
    thePillarPoints[2].ordinate =  +176;
    //
    thePillarPoints[3].abscissa = +1000000;// 10^6
    thePillarPoints[3].ordinate =   +78626;
    //
    thePillarPoints[4].abscissa = +1000000000;// 10^9
    thePillarPoints[4].ordinate =   +50849654;
    //
    thePillarPoints[5].abscissa = +1000000000000;// 10^12
    thePillarPoints[5].ordinate =   +37607953088;
    //
    thePillarPoints[6].abscissa = +1000000000000000;// 10^15
    thePillarPoints[6].ordinate =   +29844572821462;
    //
    thePillarPoints[7].abscissa = +18446744073709551615;// 10^19
    thePillarPoints[7].ordinate =   +425656551648260822;
    //
    int selectedInterval = 0;
    unsigned long long x0,x1,y0,y1;
    unsigned long long x = candidatePrimeThreshold;
    unsigned long long y;// to be interpolated
    for(int c=0; c<7; c++)// TODO test on last interval
    {
        if( thePillarPoints[c].abscissa < candidatePrimeThreshold
            && thePillarPoints[c+1].abscissa >= candidatePrimeThreshold
           )
        {
            selectedInterval = c;
            x0 = thePillarPoints[c].abscissa;
            x1 = thePillarPoints[c+1].abscissa;
            y0 = thePillarPoints[c].ordinate;
            y1 = thePillarPoints[c+1].ordinate;
            break;
        }
    }// for
    unsigned long long DeltaX = x1 - x0;
    unsigned long long DeltaY = y1 - y0;
    long double DyOnDx = (long double)DeltaY/(long double)DeltaX;
    y = DyOnDx*(x-x0) + y0;

    // TODO : which interval does candidatePrimeThreshold belong to ?
    //      : which are the two boundary points of the selected interval ?
    //      : which are the parameters of the line, that interpolates the interval boundary ?
    // : given the line y=y(x) return the interpolatedOrdinal(candidatePrimeThreshold)

    delete[] thePillarPoints;//clean
    // ready.
    return y;
}// interpolateOrdinal

void Primes::mapTraverseForward( std::map<unsigned long long, unsigned long long> * mapOfNaturals )
{
    if( nullptr!=mapOfNaturals)
    {
        std::cout<<"\n\n\t The map size is \t"<<mapOfNaturals->size()<<"\n\n";
        for( std::map<unsigned long long, unsigned long long>::iterator fwd=mapOfNaturals->begin();
             fwd != mapOfNaturals->end();
             fwd++
        )
        {// Traverse loop
            std::cout<<"\t"<< fwd->first << "\t";
            std::cout<<"\t "<<fwd->second << std::endl;
        }// Traverse loop
        std::cout<<"\n\n\t The map size is \t"<<mapOfNaturals->size()<<"\n";
    }// if( nullptr!=dictionary)
    else
    {// map empty
        std::cout<<"\n\n\t The map is empty \n\n";
    }// map empty
}//mapTraverseForward


Primes::LogIntegralPillarPoint *  Primes::getNearestIntegral( unsigned long long candidatePrimeThreshold)
{
    size_t map_size = logIntegralPillars->size();
//    LogIntegralPillarPoint * thePillarPoints = new LogIntegralPillarPoint[map_size];
//    int array_index = 0;
//    for( std::map<unsigned long long, unsigned long long>::iterator fwd=this->logIntegralPillars->begin();
//         fwd != this->logIntegralPillars->end();
//         fwd++
//    )
//    {// Traverse loop
//        thePillarPoints[array_index].threshold = fwd->first;
//        thePillarPoints[array_index].logIntegral = fwd->second;
//        array_index++;
//        if( map_size-1 == array_index )
//        {
//            break;
//        }
//    }// Traverse loop
////    if( map_size != array_index )
////    {// NB. last valid array_index is map_size-1 but at loop::exit it gets increased.
////        std::cout<<" Alarm Primes::getNearestIntegral ! \n";
////    }
//    //
    int selectedInterval = 0;
    LogIntegralPillarPoint * nearestIntegral = new LogIntegralPillarPoint();// caller has to delete
    for(int c=0; c<map_size-1; c++)// c+1<n -> c<n-1
    {

        if( this->logIntegralPillars->operator[](c).threshold < candidatePrimeThreshold
            && this->logIntegralPillars->operator[](c+1).threshold >= candidatePrimeThreshold
           )
        {
            selectedInterval = c;
            nearestIntegral->threshold = this->logIntegralPillars->operator[](c).threshold;
            nearestIntegral->logIntegral = this->logIntegralPillars->operator[](c).logIntegral;
            break;
        }// else continue.
    }// for
    // sequence : which interval does candidatePrimeThreshold belong to ?
    //          : which are the two coordinates of the left boundary point of the selected interval ?
    //          : return such coordinates.
//delete[] thePillarPoints;//clean
    // ready.
    return nearestIntegral;
}// getNearestIntegral

} //namespacesp PrimesFinder

