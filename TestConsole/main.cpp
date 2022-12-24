#include <iostream>
#include <vector>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */
//
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
#include "../NumericalAnalysis/RealAnalysis/Real.h"
#include "../NumericalAnalysis/RealMatrix/RealMatrix.h"
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

//    Cantiere_Primes_2022September01_::Primes cantiere_Pr(20);
//    Cantiere_Primes_2022September01_::Primes::SingleFactor * res =
//        cantiere_Pr.IntegerDecomposition( 36);
//    delete[] res;
//    //
//    Cantiere_Primes_2022September01_::Primes::Omega omega = cantiere_Pr.omegaProducer(+100);
//
//
//    //
//    Couple left;
//    left.argument = 1;
//    left.image = 6;
//    //
//    Couple right;
//    right.argument = 2;
//    right.image = 10;
//    //
//    Two_Points_Interpolation::Parametric_Linear_Manifold giacitura =
//        Two_Points_Interpolation::linear_parametric( left,right);
//    if( giacitura.isProblemWellPosed==false)
//    {
//        std::cout<<"\n\t Parametric Problem not well posed.\n"<<std::endl;
//    }
//    Two_Points_Interpolation::Linear_Variety_Coefficients giacitura_Cartesiana =
//        Two_Points_Interpolation::linear( left,right);
//    if( giacitura_Cartesiana.isProblemWellPosed==false)
//    {
//        std::cout<<"\n\t Cartesian Problem not well posed.\n"<<std::endl;
//    }
//    else
//    {// when the problem has been well posed :
//        double anteImage = Two_Points_Interpolation::linear_ante_image( giacitura_Cartesiana, 12);
//    }
    Numerics::RealMatrix MatA(10,10,true);
    MatA.show();
    MatA.inverse().show();
    (MatA*MatA.inverse()).show();// should be the identity(20,20).
//    Numerics::RealMatrix vectorR2( 2,1);
//    vectorR2.insert(1, 0,0);
//    vectorR2.insert(6, 1,0);
//    vectorR2.show();
//    Numerics::RealMatrix pullback_left = MatA.inverse() * vectorR2;
//    pullback_left.show();
//    vectorR2.insert(2,  0,0);
//    vectorR2.insert(10, 1,0);
//    vectorR2.show();
//    pullback_left = MatA.inverse() * vectorR2;
//    pullback_left.show();


//    Numerics::Complex x(100.0 , 0.0);
//    Numerics::Complex ro_1(+0.5, +14.123);
//    Numerics::Complex x_raised_ro( x^ro_1);
//    Numerics::Complex Log_x_raised_ro( x_raised_ro.LnC() );
//    Test_Unit_Complex::ExpIntegralEi_test();




//     // NO instance: Unit tests are preferrably static classes // Test_Unit_Complex:: Complex_test;
//    Test_Unit_Complex::manage_ComplexIntegr_asScalar_square();
//    Test_Unit_Complex::similErf_test();
//    Test_Unit_Complex::test_Ctor_s_();
//    Test_Unit_Complex::test_AmpliTwist();
//    //

//    //------NB.------TEST Primes Cantiere & Production ------------------------------
//    Test_Unit_CantierePrimes primes_Cantiere(0);
//    primes_Cantiere.Primes_Cantiere_managementMethod();
//    //
//    Test_Unit_Primes_Production_ primes_Production(80);
//    primes_Production.Primes_Production_managementMethod();


    //--------------------------------------------------------------
    Process::LogWrappers::SectionClose();
    Process::LogWrappers::EndOfApplication_MANDATORY_();// destroy a global Singleton:: NECESSARY!
    //---ready---------------
    std::cout<<"\n\n\n\t Strike Enter to leave\t";
    getchar();
    return 0;
}// main

/* --------------cantina----------------------------
    //
//    Cantiere_Primes_2022September01_::Primes cantiere(15);
//    cantiere.RandomCalcInterface(999999, 1001999);

//    Test_Unit_Logger::interface_logFromMultipleStrings();
*/
