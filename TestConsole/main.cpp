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
#include "../Entity/PrimesFinder/InternalAlgos.h"
#include "../Entity/Complex/Complex.h"
#include "../Common/Dictionary/MapOperation.h"
//-----unit test---------
#include "Test_Unit_CantierePrimes.h"
#include "Test_Unit_PrimesFinder.h"


//---entry point-------------------------
int main()
{
    Common::LogWrappers::SectionOpen("main", 0);
    //
    //------Unit Test-----CANTIERE------------------------------------------------
    unsigned long long sogliaCustom = -1;// reach it by underflow. 1.8447*10^19-1
    unsigned long long inf = 1881574000000;
    struct LogIntegralStep
    {
        long double inf;
        long double sup;
        long double card_partiz;
    };

    LogIntegralStep * LogIntegralStep_Array = new LogIntegralStep[7];
    //----
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
    unsigned long long deltaAbscissa = pow(10,15);
    unsigned long long sup = inf + deltaAbscissa;
    std::ofstream logIntegral("./LogIntegral_highZone.txt", std::fstream::out);// reset.
    std::string colonneStr("inf \t sup \t LogIntegral(inf,sup) \n");
    logIntegral.write( colonneStr.c_str(), colonneStr.length() );
    Entity::Integration::FunctionalForm LogIntegral = internalAlgos::LogIntegral_coChain;// function pointer.
    for( int c=0; c<=6; c++ )
    {
        long double quantileLogIntegral =
            Entity::Integration::trapezi(
                                         LogIntegralStep_Array[c].inf
                                         ,LogIntegralStep_Array[c].sup
                                         ,LogIntegralStep_Array[c].card_partiz // how many steps
                                         , LogIntegral );// function-pointer
        std::string * infStr = Common::StrManipul::uLongLongToString( LogIntegralStep_Array[c].inf);
        std::string * supStr = Common::StrManipul::uLongLongToString( LogIntegralStep_Array[c].sup);
        std::string * LogIntegralStr = Common::StrManipul::uLongLongToString( (unsigned long long)quantileLogIntegral);
        int forecastedTokenSize = infStr->length()+supStr->length()+4;//3 stands for '_'+'_'+'\n'+'\r'
        Common::StringBuilder * strBuild = new Common::StringBuilder( forecastedTokenSize);
        strBuild->append(infStr->c_str());
        strBuild->append("_");
        strBuild->append(supStr->c_str());
        strBuild->append("_");
        strBuild->append(LogIntegralStr->c_str());
        strBuild->append("\r");// choose one btw '\r' or '\n'
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

//    int ifromStr = Common::StrManipul::stringToInt("test Exception : Antani");//NB. returns zero on invalid input.
//    Test_Unit_CantierePrimes * test = new Test_Unit_CantierePrimes( 0);
//    bool seq = test->sequentialDump( 19541);// required prime==soglia
//    bool rand = test->randomDump( 600,  700);
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
    //------Unit Test-----CANTIERE---------------------------------------------------
    //
    //------Unit Test-----PimesFinder------------------------------------------------
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
    //------Unit Test-----------------------------------------------------



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
