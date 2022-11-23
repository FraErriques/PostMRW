#include "Test_Unit_Complex.h"

#include "../../NumericalAnalysis/Integration/Complex_Integration.h"

Test_Unit_Complex::Test_Unit_Complex()
{
    //ctor
}

Test_Unit_Complex::~Test_Unit_Complex()
{
    //dtor
}

Numerics::Complex similErf( double x, double y)// the whole image, treated as an algebraic scalar.
{
    Numerics::Complex z( x,y);
    Numerics::Complex z_squared( z*z);
    Numerics::Complex minus_z_squared( -z_squared);
    Numerics::Complex res( minus_z_squared.ExpC() );
    return res;
}// integrand_ComplexAsScalar

Numerics::Complex Square( double x, double y)// the whole image, treated as an algebraic scalar.
{
    Numerics::Complex z( x,y);
    Numerics::Complex z_squared( z*z);
    return z_squared;
}// integrand_ComplexAsScalar


// Jordan
double local_abscissa( double t){return t;}
double local_ordinate( double t)
    {return (+2)*t + (+1.0);}
double local_Dabscissa( double t){return +1;}
double local_Dordinate( double t){return (+2.0);}


void Test_Unit_Complex::manage_ComplexIntegr_asScalar_square()
{
    Complex_Integration::fPtr_ComplexAsScalar_ quadrato = Square;
    // Jordan
    Complex_Integration::fPtr_Jordan_parametriz_ abscissa = local_abscissa;
    Complex_Integration::fPtr_Jordan_parametriz_ ordinate = local_ordinate;
    Complex_Integration::fPtr_Jordan_parametriz_ dx = local_Dabscissa;
    Complex_Integration::fPtr_Jordan_parametriz_ dy = local_Dordinate;
    //
    Numerics::Complex z0(0,1); // points on the image-plane, through the CoChain
    Numerics::Complex z1(2,5); // points on the image-plane, through the CoChain
    double t0 = 0; // SAME points on the image-plane, reached via PullBack, through the Chain
    double t1 = 2;//{t0, t1} == {E, 0.5226811514040275`}
    Numerics::Complex *res =
        Complex_Integration::ContourIntegral_AsScalar_ManagementMethod(
              z0 // points on the image-plane, through the CoChain
            , z1 // points on the image-plane, through the CoChain
            , t0 // SAME points on the image-plane, reached via PullBack, through the Chain
            , t1 // SAME points on the image-plane, reached via PullBack, through the Chain
            , quadrato
            , abscissa, ordinate
            , dx, dy
            , 1000 );// #steps
    std::cout << "Integrate z^2dz {z,(0,1),(2,5) == " << res->ToString() << std::endl;
    delete res;
    // ready
}// manage_ComplexIntegr_asScalar_square

// Jordan Erf su asse reale
double local_abscissa_Erf_asseReale( double t){return t;}
double local_ordinate_Erf_asseReale( double t){return 0;}
double local_Dabscissa_Erf_asseReale( double t){return +1;}
double local_Dordinate_Erf_asseReale( double t){return 0;}
// Jordan Erf su segmento parallelo all'asse immaginario
double local_abscissa_Erf_asseImg( double t){return +1;}
double local_ordinate_Erf_asseImg( double t){return t;}
double local_Dabscissa_Erf_asseImg( double t){return 0;}
double local_Dordinate_Erf_asseImg( double t){return +1;}
// Jordan Erf su bisettrice
double local_abscissa_Erf( double t){return t;}
double local_ordinate_Erf( double t){return t;}
double local_Dabscissa_Erf( double t){return +1;}
double local_Dordinate_Erf( double t){return +1;}

void Test_Unit_Complex::similErf_test()
{
    // CoChain f(z)==Exp[-z^2] called similErf
    Complex_Integration::fPtr_ComplexAsScalar_ similErf_fPtr =
        similErf;
    // Chains
    // Jordan su asse reale
    Complex_Integration::fPtr_Jordan_parametriz_ abscissa_real = local_abscissa_Erf_asseReale;
    Complex_Integration::fPtr_Jordan_parametriz_ ordinate_real = local_ordinate_Erf_asseReale;
    Complex_Integration::fPtr_Jordan_parametriz_ dx_real = local_Dabscissa_Erf_asseReale;
    Complex_Integration::fPtr_Jordan_parametriz_ dy_real = local_Dordinate_Erf_asseReale;
    // Jordan su asse immaginario
    Complex_Integration::fPtr_Jordan_parametriz_ abscissa_img = local_abscissa_Erf_asseImg;
    Complex_Integration::fPtr_Jordan_parametriz_ ordinate_img = local_ordinate_Erf_asseImg;
    Complex_Integration::fPtr_Jordan_parametriz_ dx_img = local_Dabscissa_Erf_asseImg;
    Complex_Integration::fPtr_Jordan_parametriz_ dy_img = local_Dordinate_Erf_asseImg;
    // Jordan su bisettrice
    Complex_Integration::fPtr_Jordan_parametriz_ abscissa = local_abscissa_Erf;
    Complex_Integration::fPtr_Jordan_parametriz_ ordinate = local_ordinate_Erf;
    Complex_Integration::fPtr_Jordan_parametriz_ dx = local_Dabscissa_Erf;
    Complex_Integration::fPtr_Jordan_parametriz_ dy = local_Dordinate_Erf;
    //
    // su asse reale
    Numerics::Complex z0(0,0); // points on the image-plane, through the CoChain
    Numerics::Complex z1(1,0); // points on the image-plane, through the CoChain
    double t0 = 0; // SAME points on the image-plane, reached via PullBack, through the Chain
    double t1 = 1;//{t0, t1}=={0,1} in the PullBack.
    Numerics::Complex *res_real =
        Complex_Integration::ContourIntegral_AsScalar_ManagementMethod(
              z0 // points on the image-plane, through the CoChain
            , z1 // points on the image-plane, through the CoChain
            , t0 // SAME points on the image-plane, reached via PullBack, through the Chain
            , t1 // SAME points on the image-plane, reached via PullBack, through the Chain
            , similErf_fPtr
            , abscissa_real, ordinate_real
            , dx_real, dy_real
            , 1000 );// #steps
    std::cout << "Integrate Exp[-z^2]dz {z,(0,0),(1,0) == " << res_real->ToString() << std::endl;
    // su asse immaginario
    z0 = Numerics::Complex(1,0); // points on the image-plane, through the CoChain
    z1 = Numerics::Complex(1,1); // points on the image-plane, through the CoChain
    t0 = 0; // SAME points on the image-plane, reached via PullBack, through the Chain
    t1 = 1;//{t0, t1}=={0,1} in the PullBack.
    Numerics::Complex *res_img =
        Complex_Integration::ContourIntegral_AsScalar_ManagementMethod(
              z0 // points on the image-plane, through the CoChain
            , z1 // points on the image-plane, through the CoChain
            , t0 // SAME points on the image-plane, reached via PullBack, through the Chain
            , t1 // SAME points on the image-plane, reached via PullBack, through the Chain
            , similErf_fPtr
            , abscissa_img, ordinate_img
            , dx_img, dy_img
            , 1000 );// #steps
    std::cout << "Integrate Exp[-z^2]dz {z,(1,0),(1,1) == " << res_img->ToString() << std::endl;
    //---somma dei due tratti
    z0 = Numerics::Complex(0,0); // points on the image-plane, through the CoChain
    z1 = Numerics::Complex(1,1); // points on the image-plane, through the CoChain
    t0 = 0; // SAME points on the image-plane, reached via PullBack, through the Chain
    t1 = 1;//{t0, t1}=={0,1} in the PullBack.
    Numerics::Complex *res_whole =
        Complex_Integration::ContourIntegral_AsScalar_ManagementMethod(
              z0 // points on the image-plane, through the CoChain
            , z1 // points on the image-plane, through the CoChain
            , t0 // SAME points on the image-plane, reached via PullBack, through the Chain
            , t1 // SAME points on the image-plane, reached via PullBack, through the Chain
            , similErf_fPtr
            , abscissa, ordinate
            , dx, dy
            , 1000 );// #steps
    std::cout << "Integrate Exp[-z^2]dz {z,(0,0),(1,1) == " << res_whole->ToString() << std::endl;
    std::cout << "Sum Intg[(0,0),(1,0)]+Intg[(1,0),(1,1)] == " <<
     (*res_real+*res_img).ToString() << std::endl;
    delete res_real;
    delete res_img;
    delete res_whole;
    // ready
}// similErf_test

void Test_Unit_Complex::test_Ctor_s_()
{
    for( int c=0; c<100; c++)
    {
        Numerics::Complex * rectangular = new Numerics::Complex();
        Numerics::Complex * polar = new Numerics::Complex("polar");
        delete rectangular;
        delete polar;
    }
    // ready
}// test_Ctor_s_

bool check_product( Numerics::Complex factorLeft
                   ,Numerics::Complex factorRight
                   ,Numerics::Complex product     )
{
    double errorThreshold = +1E-13;
    double anomalia_factorLeft = factorLeft.arg();
    double anomalia_factorRight = factorRight.arg();
    double anomalia_factor_product = product.arg();
    double anomalia_mod_2PI_ = fmod( anomalia_factor_product , 2.0*PI );// NB. remander of float division.
    bool res_anomalia = true;//init to true and then &=
    // NB. keep count of arg+k*(2PI) fabs(anomalia_factor_product-(anomalia_factorLeft+anomalia_factorRight))
    res_anomalia &= ( fabs(anomalia_mod_2PI_-(anomalia_factorLeft+anomalia_factorRight))<errorThreshold);
    if( ! res_anomalia)
    {
        Process::LogWrappers::SectionContent_variable_name_value(
            "fabs(anomalia_mod_2PI_-(anomalia_factorLeft+anomalia_factorRight))=="
            ,fabs(anomalia_mod_2PI_-(anomalia_factorLeft+anomalia_factorRight))
            ,0);
    }// else skip tracing.
    double modulus_factorLeft = factorLeft.length();
    double modulus_factorRight = factorRight.length();
    double modulus_factor_product = product.length();
    bool res_modulus = true;//init to true and then &=
    res_modulus &= ( fabs(modulus_factor_product-(modulus_factorLeft*modulus_factorRight))<errorThreshold);
    // trace on error:
    if( ! res_modulus)
    {
        Process::LogWrappers::SectionContent_variable_name_value(
            "fabs(modulus_factor_product-(modulus_factorLeft*modulus_factorRight))=="
            ,fabs(modulus_factor_product-(modulus_factorLeft*modulus_factorRight))
            ,0);
    }// else skip tracing.
    // ready
    return (res_anomalia & res_modulus);// here & and && are the same: no short-circuit possible.
}// check_product

// Ampli-Twist
void Test_Unit_Complex::test_AmpliTwist()
{
    Process::LogWrappers::SectionOpen("Ampli-Twist",0);
    // rectangular
    for( double realPart=+1.0; realPart<+9.0; realPart+=+1.0)
    {
        for( double immPart=0.0; immPart<+3.0; immPart+=+0.5)
        {
            Numerics::Complex left(realPart, immPart);
            Numerics::Complex right(realPart+5, immPart+5);
            Numerics::Complex prd = left*right;
            if( ! check_product(left,right,prd))
            {
//                std::cout << "\n\t left*right== "<<left.ToString()<<
//                    " * "<<right.ToString()<<" == "<<prd.ToString()<<std::endl;
//                std::cout << "\n\t Pol_left*Pol_right== "<<left.ToString_Polar()<<
//                    " * "<<right.ToString_Polar()<<" == "<<prd.ToString_Polar()<<std::endl;
//                Process::LogWrappers::SectionContent_fromMultipleStrings(0,6,
//                    new std::string(" left*right== ")
//                    ,new std::string( left.ToString() )
//                    ,new std::string( " * " )
//                    ,new std::string( right.ToString() )
//                    ,new std::string( " == " )
//                    ,new std::string( prd.ToString() )
//                );
//                Process::LogWrappers::SectionContent_fromMultipleStrings(0,6,
//                    new std::string(" Pol_left*right== ")
//                    ,new std::string( left.ToString_Polar() )
//                    ,new std::string( " * " )
//                    ,new std::string( right.ToString_Polar() )
//                    ,new std::string( " == " )
//                    ,new std::string( prd.ToString_Polar() )
//                );
            }// end if NOT product_check (i.e. test failed).
            // else continue without logging.
        }// for internal
    }// END rectangular : for esternal
    // START polar
    for( double modulusPart=+1.0; modulusPart<+9.0; modulusPart+=+1.0)
    {
        for( double anomaliaPart=-PI/2.0; anomaliaPart<+PI/2.0; anomaliaPart+=+0.5)
        {
            Numerics::Complex left(std::string("polar"),modulusPart, anomaliaPart);
            Numerics::Complex right(std::string("polar"),modulusPart+5, anomaliaPart+5);
            Numerics::Complex prd = left*right;
            if( ! check_product(left,right,prd))
            {
//                std::cout << "\n\t left*right== "<<left.ToString()<<
//                    " * "<<right.ToString()<<" == "<<prd.ToString()<<std::endl;
//                std::cout << "\n\t Pol_left*Pol_right== "<<left.ToString_Polar()<<
//                    " * "<<right.ToString_Polar()<<" == "<<prd.ToString_Polar()<<std::endl;
//                Process::LogWrappers::SectionContent_fromMultipleStrings(0,6,
//                    new std::string(" left*right== ")
//                    ,new std::string( left.ToString() )
//                    ,new std::string( " * " )
//                    ,new std::string( right.ToString() )
//                    ,new std::string( " == " )
//                    ,new std::string( prd.ToString() )
//                );
//                Process::LogWrappers::SectionContent_fromMultipleStrings(0,6,
//                    new std::string(" Pol_left*right== ")
//                    ,new std::string( left.ToString_Polar() )
//                    ,new std::string( " * " )
//                    ,new std::string( right.ToString_Polar() )
//                    ,new std::string( " == " )
//                    ,new std::string( prd.ToString_Polar() )
//                );
            }// end if NOT product_check (i.e. test failed).
            // else continue without logging.
        }// for internal
    }// END polar for external
    Process::LogWrappers::SectionClose();
}// AmpliTwist
