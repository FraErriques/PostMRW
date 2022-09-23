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
    Test_Unit_CantierePrimes * test = new Test_Unit_CantierePrimes();
    //size_t ulong_size = sizeof( unsigned long long);
    bool seq = test->sequentialDump( 9000);
    //bool rand = test->randomDump( 0, 50);
    bool reader = test->readBy_OperatorSquares( 5);// ask Prime[n]
    //bool reader;
//    reader = test->readSequentialDump_nextRec( 60);
//    reader &= test->readSequentialDump_arrayOfRec_anywhere(
//        23
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
    delete test;
    //------Unit Test-----CANTIERE---------------------------------------------------
    //
    //------Unit Test-----PimesFinder------------------------------------------------
    //    Test_Unit_PrimesFinder * test = new Test_Unit_PrimesFinder();
    //    bool seq = test->sequentialDump( 9000);
    //    bool rand = test->randomDump( 0, 50);
    //    bool reader = false;
    //    reader &= test->readSequentialDump_nextRec( 60);
    //    reader &= test->readSequentialDump_arrayOfRec_anywhere(
    //    23
    //    ,915 // there must be room for just one record Prime<100.
    //    );
    //    //    for( int c=0; c<61; c++)
    //    //    {// next Rec
    //    //        reader &= test->readSequentialDump_nextRec(c);
    //    //    }
    //    //    for( int c=0; c<61; c++)
    //    //    {// array of Rec
    //    //        reader &= test->readSequentialDump_arrayOfRec_anywhere(
    //    //            c
    //    //            ,c+915 // there must be room for just one record Prime<100.
    //    //         );
    //    //    }// array of Rec
    //    delete test;
    //------Unit Test-----------------------------------------------------



    //--------------------------------------------------------------
    Common::LogWrappers::SectionClose();
    //---ready---------------
    std::cout<<"\n\n\n\t Strike Enter to leave\t";
    getchar();
    return 0;
}// main





/* --------------cantina----------------------------

*/
