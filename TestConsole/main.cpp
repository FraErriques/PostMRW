#include <iostream>
#include <vector>
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
#include "../Entity/PrimesFinder/InternalAlgos.h"
#include "../Entity/Complex/Complex.h"
#include "../Common/Dictionary/MapOperation.h"
//-----unit test---------
#include "Test_Unit_CantierePrimes.h"
#include "Test_Unit_PrimesFinder.h"


void mapTraverseForward(
 std::map<unsigned long long, unsigned long long> * logIntegralPillars
                                                          )
{
    if( nullptr!=logIntegralPillars)
    {
        std::cout<<"\n\n\t The map size is \t"<<logIntegralPillars->size()<<"\n\n";
        for( std::map<unsigned long long, unsigned long long>::iterator fwd=logIntegralPillars->begin();
             fwd != logIntegralPillars->end();
             fwd++
        )
        {// Traverse loop
            std::cout<<"\t"<< fwd->first << "\t";
            std::cout<<"\t "<<fwd->second << std::endl;
        }// Traverse loop
        std::cout<<"\n\n\t The map size is \t"<<logIntegralPillars->size()<<"\n";
    }// if( nullptr!=dictionary)
    else
    {// map empty
        std::cout<<"\n\n\t The map is empty \n\n";
    }// map empty
}//mapTraverseForward


//---entry point-------------------------
int main()
{
    Common::LogWrappers::SectionOpen("main", 0);
    //
    //------Unit Test-----CANTIERE------------------------------------------------
    //
    // NB. cannot do an auto-instance, since the Dtor gets called after the Singleton<Log>_destruction and that's a leak.
    // do a dynamical (pointer) instance instead and delete before the last call to the log; or, otherwise, do not call
    // the log from inside Primes::Destructor.
    Cantiere_Primes_2022September01_::Primes cantiere(50);// semi-amplitude of each map segment
//    cantiere.coveringIntegral();
    bool dumped = cantiere.distributionFunction( "./LogIntegral_firstPhase_.txt");
    mapTraverseForward( cantiere.logIntegralPillars);
    cantiere.RandomCalcInterface( 300, 350);
//    unsigned long long interpolatedOrdinal = cantiere.interpolateOrdinal( 46);// estimate the ordinal of a candidate prime or of a threshold.
//    int ifromStr = Common::StrManipul::stringToInt("test Exception : Antani");//NB. returns zero on invalid input.



//    Cantiere_Primes_2022September01_::Primes cantiere(50);// semi-amplitude of each map segment
//    unsigned long long candidateThreshold = 100;
//    unsigned long long interpolatedOrdinal = cantiere.interpolateOrdinal( candidateThreshold);
//    std::cout<< "interpolatedOrdinal("<<candidateThreshold<<")=="<<interpolatedOrdinal<<std::endl;
//Cantiere_Primes_2022September01_::Primes::SingleFactor * xx = cantiere.IntegerDecomposition( 18);

//    Test_Unit_CantierePrimes * test = new Test_Unit_CantierePrimes( 0);
//    bool seq = test->sequentialDump( 390);// required prime==soglia
//    bool rand = test->randomDump( 900, 920);
//    bool outcome_dumpTailReaderByChar = test->dumpTailReaderByChar();
//    bool outcome_lastRecordReaderByChar = test->lastRecordReaderByChar();
//    bool outcome_recoverLastRecord = test->recoverLastRecord();
//    bool outcome_recoverDumpTail = test->recoverDumpTail();
//    bool reader = true; // used with &=
//    reader = test->readSequentialDump_nextRec( 60);
//    bool outcome_acquireNextRecord = test->acquireNextRecord();
//    bool outcome_acquireSequenceOfRecord = test->acquireSequenceOfRecord();
//    size_t ulong_size = sizeof( unsigned long long);
//    reader &= test->readBy_OperatorSquares( 99);// ask Prime[n]
//    for (int c=1; c<1181; c++)
//    {
//        reader &= test->readBy_OperatorSquares( c);// ask "n" in Prime[n]
//    }
//    std::cout<<"\n\n\n\t the final outcome is : "<< reader<<"\n\n";
//    getchar();
//    //bool reader;
//    reader = test->readSequentialDump_nextRec( 60);
//    reader &= test->readSequentialDump_arrayOfRec_anywhere(
//        0
//        ,915 // there must be room for just one record Prime<100.
//     );
//    for( int c=0; c<61; c++)
//    {// next Rec
//        reader &= test->readSequentialDump_nextRec(c);
//    }
//    for( int c=0; c<61; c++)
//    {// array of Rec
//        reader &= test->readSequentialDump_arrayOfRec_anywhere(
//            c
//            ,c+915 // there must be room for just one record Prime<100.
//         );
//    }// array of Rec
//    delete test;
//    ------Unit Test-----CANTIERE---------------------------------------------------
//
//    ------Unit Test-----PimesFinder------------------------------------------------
//        Test_Unit_PrimesFinder * test = new Test_Unit_PrimesFinder( 80);
//        bool seq = test->sequentialDump( 9000);
//        bool rand = test->randomDump( 0, 50);
//        bool reader = test->readBy_OperatorSquares( 525);// ask Prime[n]
//        for (int c=599; c<611; c++)
//        {
//            reader &= test->readBy_OperatorSquares( c);// ask Prime[n]
//        }
//        std::cout<<"\n\n\n\t the final outcome is : "<< reader<<"\n\n";
//        getchar();
//        std::cout<<"\n\n\n\t Strike Enter to continue\t";
//        getchar();
//        reader &= test->readSequentialDump_nextRec( 60);
//        reader &= test->readSequentialDump_arrayOfRec_anywhere(
//        23
//        ,915 // there must be room for just one record Prime<100.
//        );
//        for( int c=0; c<61; c++)
//        {// next Rec
//            reader &= test->readSequentialDump_nextRec(c);
//        }
//        for( int c=0; c<61; c++)
//        {// array of Rec
//            reader &= test->readSequentialDump_arrayOfRec_anywhere(
//                c
//                ,c+915 // there must be room for just one record Prime<100.
//             );
//        }// array of Rec
//        delete test;
//    //------Unit Test-----------------------------------------------------



    //--------------------------------------------------------------
    Common::LogWrappers::SectionClose();
    Common::LogWrappers::EndOfApplication_MANDATORY_();// destroy a global Singleton:: NECESSARY!
    //---ready---------------
    std::cout<<"\n\n\n\t Strike Enter to leave\t";
    getchar();
    return 0;
}// main





/* --------------cantina----------------------------
    //----test accessori--------------------------
//    unsigned long long ull_r = 6000000400;
//    unsigned long long ull_l = 6000000100;
//    int Delta = ull_r - ull_l; // neanche un warning !
//    Common::StringBuilder *sb = new Common::StringBuilder( 100);
//    sb->append( ull_r);
//    const char *ull_r_str = sb->str().c_str();
//    delete sb;
//    std::cout<<"\n\n\t "<< ull_r_str<<"\n\n";
//    std::string * stlStringPtr = new std::string("some_data");
//    const char *c_buffer = stlStringPtr->c_str();
//    delete stlStringPtr;
//    std::cout<<"\n\n\t "<< c_buffer<<"\n\n";
    //----test accessori--------------------------
*/
