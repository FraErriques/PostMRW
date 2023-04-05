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
#include "../Common/Exception/Exception_model.h"//-------NB. Exception suggestions.
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


#pragma region alpha
/* used to create a documentation area or to group some functions
    multiline comment
    data...
    Oss. on 2023.February.19 first build on Fedora.
    Oss. on 2023.April.05    aligned the branch WinPlatf to latest master::label. Done on host::Kronecker.
*/

void exampleOfFunctionInRegion(double par)
{
    return;
}
#pragma endregion alpha

//---entry point-------------------------
int main()
{
    Process::LogWrappers::SectionOpen("main", 0);
    //
    //------NB.------ Primes Cantiere
    Cantiere_Primes_2022September01_::Primes primes_Cantiere(0);
    bool seq_res = primes_Cantiere.SequentialCalcInterface( 1000);
//    for( int c=0; c<9; c++)
//    {
//        std::cout<<"\n\t current index = "<<c<<std::endl;
//        std::cout<<"\n\t LiouvilleLambda = "<< primes_Cantiere.LiouvilleLambda( c) <<std::endl;
//        std::cout<<"\n\t MoebiusMu("<<c<<") = "<< primes_Cantiere.MoebiusMu( c) <<std::endl;
//        Cantiere_Primes_2022September01_::Primes::SingleFactor * factorization = primes_Cantiere.IntegerDecomposition( c);
//        if( nullptr!= factorization)
//        {
//            primes_Cantiere.show_IntegerDecomposition_helper( factorization);
//            delete[] factorization;
//            factorization = nullptr;
//        }// else skip a null factorization.
//        std::cout<<"\n\t #################### END record #######################\n"<<std::endl;
//    }
/*
    std::string *s = new std::string("   to be trimmed, but not inside    ");
    std::string rambaldo = Common::StrManipul::trimBoth( *s);
    delete s;
    std::string tmp1("  something to be trimmed left and something right, but not inside    ");
    std::string p = Common::StrManipul::trimLeft( tmp1);
    std::string q = Common::StrManipul::trimRight(p);
    std::string tmp2("  something to be trimmed left and something right, but not inside    ");
    std::string zstr = Common::StrManipul::trimBoth( tmp2);
    //
    std::string *indiceRadice_str = Common::StrManipul::intToString(99);
    std::string *Xsoglia_i_root_str = Common::StrManipul::doubleToString( +23.987,false);
    std::string desinenzaFilename(*indiceRadice_str+std::string("_")+ Common::StrManipul::trimBoth(*Xsoglia_i_root_str));
    Common::StringBuilder sb(200);//stima riga
    sb.append( *indiceRadice_str);
    sb.append( std::string("_") );
    sb.append( Common::StrManipul::trimBoth(*Xsoglia_i_root_str));
    std::string desinenzaFilename_viaStringBuilder = sb.str();
    delete indiceRadice_str;
    delete Xsoglia_i_root_str;

    Numerics::Complex z(+0.5, +14.123);
    Numerics::Complex pow_z = Numerics::Complex(+20.0,0.0)^z;
    double z_arg = z.arg();
    Numerics::Complex z_log = z.LnC();
    Numerics::Complex *z_Ei = Complex_Integration::ContourIntegral_AsScalar_JordanLinearAutoDetect_ExpIntegralEiRiemann( z_log, 999);
    //
    Numerics::Complex zw(+0.5, +914.123);
    Numerics::Complex pow_zw = Numerics::Complex(+20.0,0.0)^zw;
    double zw_arg = zw.arg();
    Numerics::Complex zw_log = zw.LnC();
    Numerics::Complex *zw_Ei = Complex_Integration::ContourIntegral_AsScalar_JordanLinearAutoDetect_ExpIntegralEiRiemann( zw_log, 999);
    delete z_Ei;
    delete zw_Ei;
    //
    */


    primes_Cantiere.Pi_of_J( (unsigned long long)1.1E+09);




//    //------NB.------TEST Primes Cantiere & Production ------------------------------
    Test_Unit_CantierePrimes primes_unitTestCantiere(0);
    primes_unitTestCantiere.Primes_Cantiere_managementMethod();
    //
    Test_Unit_Primes_Production_ primes_unitTestProduction(80);
    primes_unitTestProduction.Primes_Production_managementMethod();


    //--------------------------------------------------------------
    Process::LogWrappers::SectionClose();
    Process::LogWrappers::EndOfApplication_MANDATORY_();// destroy a global Singleton:: NECESSARY!
    //---ready---------------
    std::cout<<"\n\n\n\t Strike Enter to leave\t";
    getchar();
    return 0;
}// main



/* --------------cantina----------------------------
//    Test_Unit_Complex::ContourIntegral_AsScalar_JordanLinearAutoDetect_test(
//       Numerics::Complex(+2.0, +3.0)
//       ,Numerics::Complex(+5.0, +7.0)
//       ,100
//    );
//    Test_Unit_Complex::ContourIntegral_AsScalar_JordanLinearAutoDetect_test(
//       Numerics::Complex(-112.0, -30.0)
//       ,Numerics::Complex(+5.0, +7.0)
//       ,100000   // #trapezia in the partition
//    );
    //
    Test_Unit_Complex::ContourIntegral_AsScalar_JordanLinearAutoDetect_ExpIntegralEiRiemann_test(
      Numerics::Complex(+3.0,+1.0),
      1000 );
    //
    Numerics::Complex *suiCateti =
        Test_Unit_Complex::ContourIntegral_AsScalar_JordanLinearAutoDetect_onExpIntegralEiCoChain_test(
            Numerics::Complex( log(sqrt(+2.0)) , 0.0),
            Numerics::Complex(+3.0, 0.0),
            1000 );

    Numerics::Complex *secondoCateto =
        Test_Unit_Complex::ContourIntegral_AsScalar_JordanLinearAutoDetect_onExpIntegralEiCoChain_test(
            Numerics::Complex(+3.0, 0.0),
            Numerics::Complex(+3.0,+1.0),
            1000 );
    *suiCateti += *secondoCateto;
    Process::LogWrappers::SectionContent_fromMultipleStrings(
        0 // verbosity
        ,2 // how many std::string*
        ,new std::string("sui cateti {a,b,c}={log(sqrt(+2.0),0.0},{+3.0,0.0},{+3.0,+1.0}== ")
        ,new std::string( (*suiCateti).ToString() )
        );
    delete suiCateti;
    delete secondoCateto;

//    someFunc();
//
//    Cantiere_Primes_2022September01_::Primes cantiere(15);
//    cantiere.RandomCalcInterface(999999, 1001999);
//
//    Test_Unit_Logger::interface_logFromMultipleStrings();
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
    Cantiere_Primes_2022September01_::Primes cantiere_Pr(20);
    for( int c=1; c<6; c++)
    {
        std::cout<< "\n\t MoebiusMu["<<c<<"]== "<<cantiere_Pr.MoebiusMu( c)<<std::endl;
    }

//    Cantiere_Primes_2022September01_::Primes cantiere_Pr(20);
//    Cantiere_Primes_2022September01_::Primes::SingleFactor * res =
//        cantiere_Pr.IntegerDecomposition( 36);
//    delete[] res;
//    //
//    Cantiere_Primes_2022September01_::Primes::Omega omega = cantiere_Pr.omegaProducer(+100);
//
//
    //
    Couple left;
    left.argument = 56;
    left.image = 226;
    //
    Couple right;
    right.argument = 56;
    right.image = 370;
    //
    Two_Points_Interpolation::Parametric_Linear_Manifold giacitura =
        Two_Points_Interpolation::linear_parametric( left,right);
    if( giacitura.isProblemWellPosed==false)
    {
        std::cout<<"\n\t Parametric Problem not well posed.\n"<<std::endl;
    }

    Numerics::RealMatrix MatA(2,2,true);
    MatA.insert( giacitura.alpha_x, 0,0);MatA.insert( giacitura.beta_x , 0,1);// row (I)
    MatA.insert( giacitura.alpha_y, 1,0);MatA.insert( giacitura.beta_y , 1,1);// row (II)
    try
    {
//        MatA.show();
//        MatA.inverse().show();
//        (MatA*MatA.inverse()).show();// should be the identity(20,20).
    //-------- pullback identification --------------
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
    //
//        Numerics::RealMatrix vectorR2( 2,1);
//        vectorR2.insert(56, 0,0);
//        vectorR2.insert(226, 1,0);
//        vectorR2.show();
//        Numerics::RealMatrix pullback_left = MatA.inverse() * vectorR2;
//        pullback_left.show();
//        vectorR2.insert(56, 0,0);
//        vectorR2.insert(370, 1,0);
//        vectorR2.show();
//        pullback_left = MatA.inverse() * vectorR2;
//        pullback_left.show();
    }
    catch( Crash curExcp)
    {
        int c = -1;
        c++;
        std::cout<<"\n\t from inside: catch( Crash curExcp) "<<std::endl;
    }
    catch(...)
    {
        int c = -1;
        c++;
    }
    //

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


*/
