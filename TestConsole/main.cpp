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
#include "../Common/Stream/stream_io_.h"
#include "../Entity/Integration/Integration.h"
#include "../Entity/Integration/Integrate.h"
#include "../Entity/PrimesFinder/Primes.h"
#include "../Entity/PrimesFinder/Cantiere_Primes_2022September01_.h"
#include "../Entity/Complex/Complex.h"
#include "../Common/Dictionary/MapOperation.h"



//---entry point-------------------------
int main()
{
    // system("pwd");
    // system("dir"); Process_cur_dir: Directory di C:\root\projects\GitHubSandBox\PostMRW\TestConsole
    Common::LogWrappers::SectionOpen("main", 0);
    const std::string customFileConfigSectionName( "primeCustomFile");
    Cantiere_Primes_2022September01_::Primes *p = new Cantiere_Primes_2022September01_::Primes();
    bool res = p->RandomCalcInterface(
       20000000
       ,20000100 );
    //
    //const char * customDumpFullPath = p->feed_CustomDumpPath();// sets a member : TODO
    //p->Start_PrimeDump_FileSys();// NB. callable from both "fromOrigin" & "custom" TODO
    long LastOrdinal = p->getLastOrdinal(); //  (*p)[p->lastOrdinal]; TODO give it a public reader
    long LastPrime = p->getLastPrime();
    //
    long desPrime = (*p)[25];
    std::string logMsg = "desPrime = (*p)[25]==";
    std::string * desinenza = Common::StrManipul::uLongToString( desPrime);
    logMsg += *desinenza;
    Common::LogWrappers::SectionContent( logMsg.c_str(), 0 );
    delete desinenza;
    //
    desPrime = (*p)[3];
    logMsg = "desPrime = (*p)[3]==";
    desinenza = Common::StrManipul::uLongToString( desPrime);
    logMsg += *desinenza;
    Common::LogWrappers::SectionContent( logMsg.c_str(), 0 );
    delete desinenza;
    //-----
    delete p;
    Common::LogWrappers::SectionClose();


    //---ready---------------
    std::cout<<"\n\n\n\t Strike Enter to leave\t";
    getchar();
    return 0;
}// main





/* --------------cantina----------------------------

    int bisectionConvergenceSteps = p->Bisection( 73, 100, false);
    for(int c=1; c<=100;c++)// NB. you cannot require an ordinal<+1.
    {
        std::cout << "\n\t converging in " << p->Bisection( c, 100, false) <<" steps."<<std::endl;
    }
    //int NsectionConvergenceSteps = p->NpartSection( 73, 100, false);
    for(int c=73; c<=100;c++)
    {
        std::cout<<" steps for Bisection to converge to LandingPoint_"<<c<<" are:"<< p->Bisection( c, 100, false)<<std::endl;
        std::cout<<" steps for NpartSection to converge to LandingPoint_"<<c<<" are:"<< p->NpartSection( c, 100, false)<<std::endl;
    }

//    as is
//    "testCowsCanBeMilked"
//    "testSheepAreNotTheOnlyFruit"
//    // to be
//    "Cows can be milked"
//    "Sheep are not the only fruit"
//

    std::vector<std::string> * camelToken = nullptr;// many usages: allocated & deleted each time.
    //
    std::string * original_one = new std::string("testCowsCanBeMilked");
    testCallerQuestTwo( original_one);

    std::string * original_two = new std::string("testSheepAreNotTheOnlyFruit");
    testCallerQuestTwo( original_two);

    std::string * original_three = new std::string("testSheep WrittenNot123#So23@Good really");
    testCallerQuestTwo( original_two);


char toLower(const char par)
{
    if( (int)par>=65 && (int)par<=90)
    {
        return ( (char)((int)(par)+32));
    }
    else
    {
        return par;// if it's not an upper case letter, don't convert it.
    }
}

bool isUpper(const char par)
{
    bool res = false;
    if( (int)par>=65 && (int)par<=90)
    {
        res = true;
    }// else stay false.
    //    if( (int)par>=97 && (int)par<=122)
    //    {
    //        res = false;
    //    } this is should be for lower case
    // done
    return res;
}



// as is
//"testCowsCanBeMilked"
//"testSheepAreNotTheOnlyFruit"
//// to be
//"Cows can be milked"
//"Sheep are not the only fruit"
//
std::vector<std::string> * camelTokenizer( const std::string * original_one )
{
    std::vector<std::string> * camelToken = new std::vector<std::string>();
    Common::StringBuilder * curToken = new Common::StringBuilder( original_one->length());// round in excess
    // if(curChar is NOT upper -> pushBack)
    // else if(curChar is upper -> terminateCurStrBuilder -> Start a new one with toLower(curChar) )
    //
    int c=0;//needed after endOfLoop
    for( ; c<original_one->length(); c++)
    {
        if( ! isUpper( original_one->at(c) ) )
        {
            curToken->append( original_one->at(c) );
        }
        else // not upper
        {
            camelToken->push_back( curToken->str() );
//std::cout<< curToken->str() <<std::endl;
            delete curToken;
            curToken = nullptr;
            curToken = new Common::StringBuilder( original_one->length());// get ready for another one
            curToken->append( toLower( original_one->at(c) ) );
        }// end else  not upper
    }// end for
    //---recover last token, which has no capitalLetter to signal its registration
    camelToken->push_back( curToken->str() );
//std::cout<< curToken->str() <<std::endl;
    delete curToken;
    curToken = nullptr;
    // no successive token, to get ready for.
    return camelToken;
}// end camelTokenizer(


void testCallerQuestTwo( const std::string * original_one )
{
    std::vector<std::string> * camelToken = camelTokenizer( original_one);
    // rewind
    for( std::vector<std::string>::iterator theTokens=camelToken->begin();
        theTokens != camelToken->end();
        theTokens++
    )
    {
        std::cout << *theTokens << " ";
    }
    std::cout << std::endl;
    delete original_one;
    delete camelToken;
}// end testCallerQuestTwo(



*/
