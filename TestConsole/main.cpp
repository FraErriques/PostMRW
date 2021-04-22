
//#include <boost/lambda/lambda.hpp>
//#include "../Common/DbConnectionService/dbCall.h"
#include <iostream>
#include "../Common/StringBuilder/StringBuilder.h"
#include "../Common/Config_wrap/Config_wrap.h"
#include "../Common/Stream/stream_io_.h"
#include "../Common/Stream/RWtxtfile.h"
#include "../Process/MonteCarlo_wrap/MonteCarlo_wrap.h"
#include "../Common/MonteCarlo/BaseConverter.h"
#include "../Common/MonteCarlo/ClassicalDiscreteGenerator.h"
#include "../Common/MonteCarlo/ClassicalContinuousGenerator.h"
#include "../Entity/Integration/Integration.h"
#include "../Entity/Integration/Integrate.h"
#include "../Entity/PrimesFinder/Primes.h"


unsigned long recoveredOrdinal;
unsigned long recoveredPrime;
void recoverLastRecord( const char * fromFile)
{
    std::vector<std::string> * tokenArray = Common::StrManipul::stringSplit("_", fromFile, true );// remove empty entries.

    int i=+1;
    for( std::vector<std::string>::iterator it=tokenArray->begin(); it!=tokenArray->end(); ++it, i++)
    {
        std::cout << "token nr."<< i <<"   "<< *it<<std::endl;
        std::vector<std::string> * splitOnNewLine = Common::StrManipul::stringSplit("\n", *it, true );// remove empty entries.
        int nestedTokens=0;
        for( std::vector<std::string>::iterator nested_it=splitOnNewLine->begin(); nested_it!=splitOnNewLine->end(); ++nested_it, nestedTokens++)
        {
            std::cout << "nested-token nr."<< nestedTokens <<"   "<< *nested_it<<std::endl;
        }
    }
}//recoverLastRecord


int main()
{
     PrimesFinder::Primes * p = new PrimesFinder::Primes();
     const char * primeDefaultFile = p->getDefaultPrimeDumpFullPath( "primeDefaultFile");
     p->createOrAppend( primeDefaultFile);
     char * straightContent  = p->lastRecordReaderByString( primeDefaultFile);
     std::cout<<"\n\n"<< straightContent;
     recoverLastRecord(straightContent);

     //const char * straightContent = p->lastRecordReader( primeDefaultFile);
     //p->old_lastRecordReader(primeDefaultFile );



//     int contentLen=0;
//     for( ; 0!=straightContent[contentLen]; contentLen++ );
//     for( int d=0; d<contentLen; d++)
//     {
//        if( 49<=straightContent[d] &&  57>=straightContent[d] )
//        {}// do nothing: i.e. leave it.
//        else if( 95==straightContent[d] )
//        {}// do nothing: i.e. leave it.
//        else
//        {//  replace invalid chars with an hash '#'
//            straightContent[d] = '#';
//        }
//     }
    std::cout<<"\n\n"<< straightContent;


//    std::vector<std::string> * tokenArray = Common::StrManipul::stringSplit("_", straightContent, true );// remove empty entries.
//    int i=+1;
//    for( std::vector<std::string>::iterator it=tokenArray->begin(); it != tokenArray->end(); ++it, i++)
//    {
//        std::cout << "token nr."<< i <<"   "<< *it<<std::endl;
//    }



//     int d=c-1,x=0;
//     for( ; d>=0; d--,x++ )
//     {
//         revertedContent[x]=straightContent[d];
//     }
//     for(  d=0; d<c; d++)
//     {
//        if( 49<=revertedContent[d] &&  57>=revertedContent[d] )
//        {}// do nothing: i.e. leave it.
//        else if( 95==revertedContent[d] )
//        {}// do nothing: i.e. leave it.
//        else
//        {//  replace invalid chars with an hash '#'
//            revertedContent[d] = '#';
//        }
//     }
//     revertedContent[c] = 0;
//     std::cout<<"\n\n"<< revertedContent;


     delete primeDefaultFile;
     delete straightContent;



    //
    std::cout<<"\n\t Strike Enter to leave\t";
    getchar();
    return 0;
}
