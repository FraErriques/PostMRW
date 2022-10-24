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
#include "../NumericalAnalysis/Integration/Integration.h"
#include "../NumericalAnalysis/Integration/Integrate.h"
#include "../NumericalAnalysis/PrimesFinder/Primes.h"
#include "../NumericalAnalysis/PrimesFinder/Cantiere_Primes_2022September01_.h"
#include "../NumericalAnalysis/PrimesFinder/InternalAlgos.h"
#include "../NumericalAnalysis/Complex/Complex.h"
#include "../Common/Dictionary/MapOperation.h"
//-----unit test---------
#include "Unit_Tests/Test_Unit_CantierePrimes.h"
#include "Unit_Tests/Test_Unit_PrimesFinder.h"
#include "Unit_Tests/Test_Unit_Logger.h"
#include "Unit_Tests/Test_Unit_selectInterval.h"
//
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */




//---entry point-------------------------
int main()
{
    Common::LogWrappers::SectionOpen("main", 0);
    //
    //------Unit Test-----CANTIERE------------------------------------------------
    //
    std::string prefix("this string describes the following contents: int, double : ");
    int i_log = 22;
    double d_log = 2.7182818;
    Common::LogWrappers::SectionContent_fromMultipleStrings(
        3 // how many params
        ,0 // section_vebosity
        ,Common::StrManipul::stringByValueToStringPtr(prefix)
        ,Common::StrManipul::intToString( i_log)
        ,Common::StrManipul::doubleToString(d_log)
     );

    Test_Unit_Logger test_unit_logger;
    test_unit_logger.managementMethod();

    Test_Unit_selectInterval test_Unit_selectInterval; //ctor performs the test-calls, in this case.
    test_Unit_selectInterval.managementMethod();


    //
    Test_Unit_CantierePrimes * test = new Test_Unit_CantierePrimes( 0);
    bool seq = test->sequentialDump( 99390);// required prime==soglia

    bool rand = test->randomDump(  15,  20);
//    rand = test->randomDump( 900, 920);
//    rand = test->randomDump( 1500, 1520);
//    rand = test->randomDump( 18446744073709550510, 18446744073709551610);


    bool outcome_dumpTailReaderByChar = test->dumpTailReaderByChar();
    bool outcome_lastRecordReaderByChar = test->lastRecordReaderByChar();
    bool outcome_recoverLastRecord = test->recoverLastRecord();
    bool outcome_recoverDumpTail = test->recoverDumpTail();
    bool reader = true; // used with &=
    reader = test->readSequentialDump_nextRec( 60);
    bool outcome_acquireNextRecord = test->acquireNextRecord();
    bool outcome_acquireSequenceOfRecord = test->acquireSequenceOfRecord();
    size_t ulong_size = sizeof( unsigned long long);
    reader &= test->readBy_OperatorSquares( 99);// ask Prime[n]
    for (int c=1; c<1181; c++)
    {
        reader &= test->readBy_OperatorSquares( c);// ask "n" in Prime[n]
    }
    std::cout<<"\n\n\n\t the final outcome is : "<< reader<<"\n\n";
    getchar();
    //bool reader;
    reader = test->readSequentialDump_nextRec( 60);
    reader &= test->readSequentialDump_arrayOfRec_anywhere(
        0
        ,915 // there must be room for just one record Prime<100.
     );
    for( int c=0; c<61; c++)
    {// next Rec
        reader &= test->readSequentialDump_nextRec(c);
    }
    for( int c=0; c<61; c++)
    {// array of Rec
        reader &= test->readSequentialDump_arrayOfRec_anywhere(
            c
            ,c+915 // there must be room for just one record Prime<100.
         );
    }// array of Rec
    delete test;

//    ------Unit Test-----CANTIERE---------------------------------------------------
//
//############################################################################################################
//
//    ------Unit Test-----PimesFinder------------------------------------------------
//        Test_Unit_PrimesFinder * test = new Test_Unit_PrimesFinder( 80);
//        bool seq = test->sequentialDump( 9000);
//        bool rand = test->randomDump( 0, 50);
//        bool reader = test->readBy_OperatorSquares( +2);// ask Prime[n]
//        for (int c=599; c<611; c++)
//        {
//            reader &= test->readBy_OperatorSquares( c);// ask Prime[n]
//        }
//        std::cout<<"\n\n\n\t the final outcome is : "<< reader<<"\n\n";
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
    Cantiere_Primes_2022September01_::Primes cantiere(200);// semi-amplitude of each map segment
    cantiere.distributionFunction_fromExistingMesh();
    //Cantiere_Primes_2022September01_::Primes::SingleFactor * xx = cantiere.IntegerDecomposition( 97*19);
    const std::string * sequentialPath = cantiere.feed_sequentialDumpPath();
    const std::string * randomPath = cantiere.feed_customDumpPath();
    const std::string * meshRenewalPath = cantiere.feed_meshSpecificationPath();
    const std::string * localIntegralPath = cantiere.feed_localIntegralPath();
    const std::string * globalIntegralPath = cantiere.feed_globalIntegralPath();
//  DON'T  delete sequentialPath; NB DON'T  they are pointers to members deleted by the Dtor
// //  DON'T   delete randomPath;
//  //  DON'T  delete meshRenewalPath;
//  //  DON'T  delete localIntegralPath;
// //  DON'T   delete globalIntegralPath;

    // NB. cannot do an auto-instance, since the Dtor gets called after the Singleton<Log>_destruction and that's a leak.
    // do a dynamical (pointer) instance instead and delete before the last call to the log; or, otherwise, do not call
    // the log from inside Primes::Destructor.
//    Cantiere_Primes_2022September01_::Primes cantiere(50);// semi-amplitude of each map segment
////    cantiere.coveringIntegral();
//    bool dumped = cantiere.distributionFunction( "./LogIntegral_firstPhase_.txt");
//    cantiere.mapTraverseForward( cantiere.logIntegralPillars);
//    cantiere.RandomCalcInterface( 300, 350);
//    unsigned long long interpolatedOrdinal = cantiere.interpolateOrdinal( 46);// estimate the ordinal of a candidate prime or of a threshold.
//    int ifromStr = Common::StrManipul::stringToInt("test Exception : Antani");//NB. returns zero on invalid input.

      int m;
      m= FindMax (7 // how many params
                  ,999,422,631,834,892,104,772 );// the actual params
      printf ("The largest value is: %d\n",m);


     std::string * s1 = Common::StrManipul::boolToString( true);
     std::string * s2 = Common::StrManipul::doubleToString( +2.7182818E+03 );
     std::string * s3 = Common::StrManipul::longDoubleToString( +3.1415E-03);
     std::string * s4 = Common::StrManipul::uLongLongToString( +123456789);
     std::string * s5 = Common::StrManipul::intToString( -9);
    int hm_strings_toLog = logFromMultipleStrings(
                                                 5 // hm_params
                                                 , s1,s2,s3,s4,s5 ); // the actual params

//    Cantiere_Primes_2022September01_::Primes cantiere(50);// semi-amplitude of each map segment
//Cantiere_Primes_2022September01_::Primes::SingleFactor * xx = cantiere.IntegerDecomposition( 18);
//    unsigned long long candidateThreshold = 100;
//    unsigned long long interpolatedOrdinal = cantiere.interpolateOrdinal( candidateThreshold);
//    std::cout<< "interpolatedOrdinal("<<candidateThreshold<<")=="<<interpolatedOrdinal<<std::endl;
//Cantiere_Primes_2022September01_::Primes::SingleFactor * xx = cantiere.IntegerDecomposition( 18);
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
