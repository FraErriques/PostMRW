#include <iostream>
#include <vector>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */
//
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
#include "../Process/Config_wrap/Config_wrap.h"
#include "../Common/Stream/stream_io_.h"
#include "../NumericalAnalysis/Integration/Integration.h"
#include "../NumericalAnalysis/Integration/Integrate.h"
#include "../NumericalAnalysis/PrimesFinder/Primes.h"
#include "../NumericalAnalysis/PrimesFinder/Cantiere_Primes_2022September01_.h"
#include "../NumericalAnalysis/PrimesFinder/InternalAlgos.h"
#include "../NumericalAnalysis/Complex/Complex.h"
#include "../NumericalAnalysis/Integration/Complex_Integration.h"
#include "../Common/Dictionary/MapOperation.h"
//-----unit test---------
#include "Unit_Tests/Test_Unit_Primes_Cantiere_.h"
#include "Unit_Tests/Test_Unit_Primes_Production_.h"
#include "Unit_Tests/Test_Unit_Logger.h"
#include "Unit_Tests/Test_Unit_selectInterval.h"
#include "Unit_Tests/Test_Unit_MonteCarlo.h"
#include "Unit_Tests/Test_Unit_RealAnalysis.h"
#include "Unit_Tests/Test_Unit_Complex.h"
//-----unit test---------




//---entry point-------------------------
int main()
{
    Process::LogWrappers::SectionOpen("main", 0);
    //
    // NO instance: Unit tests are preferrably static classes // Test_Unit_Complex:: Complex_test;
    Test_Unit_Complex::manage_ComplexIntegr_asScalar_square();
    Test_Unit_Complex::similErf_test();
    Test_Unit_Complex::test_Ctor_s_();
    Test_Unit_Complex::test_AmpliTwist();
    //
    Test_Unit_Logger::interface_logFromMultipleStrings();
    //
    Test_Unit_CantierePrimes primes_Cantiere(0);
    primes_Cantiere.Primes_Cantiere_managementMethod();
    //
    Test_Unit_Primes_Production_ primes_Production(80);
    primes_Production.Primes_Production_managementMethod();


    //--------------------------------------------------------------
    Process::LogWrappers::SectionClose();
    Process::LogWrappers::EndOfApplication_MANDATORY_();// destroy a global Singleton:: NECESSARY!
    //---ready---------------
    std::cout<<"\n\n\n\t Strike Enter to leave\t";
    getchar();
    return 0;
}// main

/* --------------cantina----------------------------

*/
