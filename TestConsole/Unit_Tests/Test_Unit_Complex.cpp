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
    Numerics::Complex z0(0,1); // points on the argument-plane
    Numerics::Complex z1(2,5); // points on the argument-plane
    double t0 = 0; // SAME points on the argument-plane, reached via PullBack, through the Chain
    double t1 = 2;//{t0, t1} == {E, 0.5226811514040275`}
    Numerics::Complex *res =
        Complex_Integration::ContourIntegral_AsScalar_ManagementMethod(
              z0 // points on the argument-plane
            , z1 // points on the argument-plane
            , t0 // SAME points on the argument-plane, reached via PullBack, through the Chain
            , t1 // SAME points on the argument-plane, reached via PullBack, through the Chain
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
double local_abscissa_Erf_asseImg( double where){return +1.52154;}// fix x=x0
double local_ordinate_Erf_asseImg( double t){return t;}
double local_Dabscissa_Erf_asseImg( double t){return 0;}
double local_Dordinate_Erf_asseImg( double t){return +1;}
// Jordan Erf su bisettrice
double local_abscissa_Erf_bisector( double t){return t;}
double local_ordinate_Erf_bisector( double t){return t;}
double local_Dabscissa_Erf_bisector( double t){return +1;}
double local_Dordinate_Erf_bisector( double t){return +1;}
// Jordan Erf su semicirconferenza intorno a singolarita' in (x0,y0)
double local_abscissa_Erf_bump( double t, double x0){return x0+cos(t);}
double local_ordinate_Erf_bump( double t, double y0){return y0+sin(t);}
double local_Dabscissa_Erf_bump( double t){return -sin(t);}
double local_Dordinate_Erf_bump( double t){return +cos(t);}

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
    Complex_Integration::fPtr_Jordan_parametriz_ abscissa = local_abscissa_Erf_bisector;
    Complex_Integration::fPtr_Jordan_parametriz_ ordinate = local_ordinate_Erf_bisector;
    Complex_Integration::fPtr_Jordan_parametriz_ dx = local_Dabscissa_Erf_bisector;
    Complex_Integration::fPtr_Jordan_parametriz_ dy = local_Dordinate_Erf_bisector;
    //
    // su asse reale
    Numerics::Complex z0(0,0); // points on the argument-plane
    Numerics::Complex z1(1,0); // points on the argument-plane
    double t0 = 0; // SAME points on the argument-plane, reached via PullBack, through the Chain
    double t1 = 1;//{t0, t1}=={0,1} in the PullBack.
    Numerics::Complex *res_real =
        Complex_Integration::ContourIntegral_AsScalar_ManagementMethod(
              z0 // points on the argument-plane
            , z1 // points on the argument-plane
            , t0 // SAME points on the argument-plane, reached via PullBack, through the Chain
            , t1 // SAME points on the argument-plane, reached via PullBack, through the Chain
            , similErf_fPtr
            , abscissa_real, ordinate_real
            , dx_real, dy_real
            , 1000 );// #steps
    std::cout << "Integrate Exp[-z^2]dz {z,(0,0),(1,0) == " << res_real->ToString() << std::endl;
    // su asse immaginario
    z0 = Numerics::Complex(1,0); // points on the argument-plane
    z1 = Numerics::Complex(1,1); // points on the argument-plane
    t0 = 0; // SAME points on the argument-plane, reached via PullBack, through the Chain
    t1 = 1;//{t0, t1}=={0,1} in the PullBack.
    Numerics::Complex *res_img =
        Complex_Integration::ContourIntegral_AsScalar_ManagementMethod(
              z0 // points on the argument-plane
            , z1 // points on the argument-plane
            , t0 // SAME points on the argument-plane, reached via PullBack, through the Chain
            , t1 // SAME points on the argument-plane, reached via PullBack, through the Chain
            , similErf_fPtr
            , abscissa_img, ordinate_img
            , dx_img, dy_img
            , 1000 );// #steps
    std::cout << "Integrate Exp[-z^2]dz {z,(1,0),(1,1) == " << res_img->ToString() << std::endl;
    //---somma dei due tratti
    z0 = Numerics::Complex(0,0); // points on the argument-plane
    z1 = Numerics::Complex(1,1); // points on the argument-plane
    t0 = 0; // SAME points on the argument-plane, reached via PullBack, through the Chain
    t1 = 1;//{t0, t1}=={0,1} in the PullBack.
    Numerics::Complex *res_whole =
        Complex_Integration::ContourIntegral_AsScalar_ManagementMethod(
              z0 // points on the argument-plane
            , z1 // points on the argument-plane
            , t0 // SAME points on the argument-plane, reached via PullBack, through the Chain
            , t1 // SAME points on the argument-plane, reached via PullBack, through the Chain
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


void Test_Unit_Complex::ExpIntegralEi_test()
{
    // CoChain f(z)==Exp[z]/z called ExpIntegralEi
    Complex_Integration::fPtr_ComplexAsScalar_  ExpIntegralEi_fPtr = Complex_Integration::ExpIntegralEi_CoChain;// production impl
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
    // Jordan su bump
//    Complex_Integration::fPtr_Jordan_parametriz_ abscissa = local_abscissa_Erf_bisector;
//    Complex_Integration::fPtr_Jordan_parametriz_ ordinate = local_ordinate_Erf_bisector;
//    Complex_Integration::fPtr_Jordan_parametriz_ dx = local_Dabscissa_Erf_bisector;
//    Complex_Integration::fPtr_Jordan_parametriz_ dy = local_Dordinate_Erf_bisector;
    //
    // su asse reale a sx dell'origine
    Numerics::Complex z0(-9.0E+05, 0.0); // points on the argument-plane
    Numerics::Complex z1(-0.1, 0.0); // points on the argument-plane
    double t0 = -9.0E+05; // SAME points on the argument-plane, reached via PullBack, through the Chain
    double t1 = -0.1;//in the PullBack.
    Numerics::Complex *res_leftOrigin =
        Complex_Integration::ContourIntegral_AsScalar_ManagementMethod(
              z0 // points on the argument-plane
            , z1 // points on the argument-plane
            , t0 // SAME points on the argument-plane, reached via PullBack, through the Chain
            , t1 // SAME points on the argument-plane, reached via PullBack, through the Chain
            , ExpIntegralEi_fPtr
            , abscissa_real, ordinate_real
            , dx_real, dy_real
            , (unsigned long long)+1.0E+02 );// #steps TODO register result
    std::cout << "Integrate Exp[z]/z d(Re(z))==dx {z,(-Infinity,0),(-0.1,0) == " << res_leftOrigin->ToString() << std::endl;
    Process::LogWrappers::SectionContent_variable_name_value("res_leftOrigin->Re()", res_leftOrigin->Re(), 0);
    Process::LogWrappers::SectionContent_variable_name_value("res_leftOrigin->Im()", res_leftOrigin->Im(), 0);
    // su asse reale a dx dell'origine
    z0 = Numerics::Complex(+0.1,0.0); // points on the argument-plane
    z1 = Numerics::Complex(+1.52154, 0.0); // points on the argument-plane
    t0 = +0.1; // SAME points on the argument-plane, reached via PullBack, through the Chain
    t1 = +1.52154;//{t0, t1}=={0,1} in the PullBack.
    Numerics::Complex *res_rightOrigin =
        Complex_Integration::ContourIntegral_AsScalar_ManagementMethod(
              z0 // points on the argument-plane
            , z1 // points on the argument-plane
            , t0 // SAME points on the argument-plane, reached via PullBack, through the Chain
            , t1 // SAME points on the argument-plane, reached via PullBack, through the Chain
            , ExpIntegralEi_fPtr
            , abscissa_real, ordinate_real
            , dx_real, dy_real
            , 1000 );// #steps
    std::cout << "Integrate Exp[z]/z d(Re(z))==dx {z,(+0.1,0),(+1.52154,0) == " << res_rightOrigin->ToString() << std::endl;
    // su asse parallelo a quello immaginario
    z0 = Numerics::Complex(+1.52154, 0.0); // points on the argument-plane
    z1 = Numerics::Complex(+1.52154, +6.29636); // points on the argument-plane
    t0 = 0; // SAME points on the argument-plane, reached via PullBack, through the Chain
    t1 = +6.29636;//{t0, t1}=={0,1} in the PullBack.
    Numerics::Complex *res_img =
        Complex_Integration::ContourIntegral_AsScalar_ManagementMethod(
              z0 // points on the argument-plane
            , z1 // points on the argument-plane
            , t0 // SAME points on the argument-plane, reached via PullBack, through the Chain
            , t1 // SAME points on the argument-plane, reached via PullBack, through the Chain
            , ExpIntegralEi_fPtr
            , abscissa_img, ordinate_img
            , dx_img, dy_img
            , 1000 );// #steps
    std::cout << "Integrate Exp[z]/z dz {z,(+1.52154,0),(+1.52154,+6.29636) == " << res_img->ToString() << std::endl;
    //---totale
    Numerics::Complex res_whole = Numerics::Complex( *res_leftOrigin+ *res_rightOrigin + *res_img);
    std::cout << "Integrate Exp[z]/z_dz {z,(-Infinity,0),(+1.52154,+6.29636) == " << res_whole.ToString() << std::endl;
    //
    delete res_leftOrigin;
    delete res_rightOrigin;
    delete res_img;
    // NO delete res_whole it's an automatic variable.
    // ready
}// ExpIntegralEi_test



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
    if( fabs(fabs(anomalia_factor_product-(anomalia_factorLeft+anomalia_factorRight))-2.0*PI)<+1.0E-11 )
    {// not an actual mistake; just a full round.
        res_anomalia = true;
    }// else it's an actual mistake.
    if( ! res_anomalia)
    {
        Process::LogWrappers::SectionContent_variable_name_value(
            "anomalia_mod_2PI_=="
            ,anomalia_mod_2PI_
            ,0);
        Process::LogWrappers::SectionContent_variable_name_value(
            "anomalia_factorLeft=="
            ,anomalia_factorLeft
            ,0);
        Process::LogWrappers::SectionContent_variable_name_value(
            "anomalia_factorRight=="
            ,anomalia_factorRight
            ,0);
        Process::LogWrappers::SectionContent_variable_name_value(
            "anomalia_factor_product=="
            ,anomalia_factor_product
            ,0);
        Process::LogWrappers::SectionContent_variable_name_value(
            "fabs(anomalia_mod_2PI_-(anomalia_factorLeft+anomalia_factorRight))=="
            ,fabs(anomalia_mod_2PI_-(anomalia_factorLeft+anomalia_factorRight))
            ,0);
        Process::LogWrappers::SectionContent_variable_name_value(
            "fabs(anomalia_factor_product-(anomalia_factorLeft+anomalia_factorRight))=="
            ,fabs(anomalia_factor_product-(anomalia_factorLeft+anomalia_factorRight))
            ,0);
        Process::LogWrappers::SectionContent_fromMultipleStrings(0,1,
            new std::string("-----record terminator-----------------------------------------------------------------------") );
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
    for( double realPart=+1.0; realPart<+9.0E+01; realPart+=+1.0)
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
        double eps = +1.0E-19;
        for( double anomaliaPart=-PI/2.0+eps; anomaliaPart<3*(+PI/2.0-eps); anomaliaPart+=+0.5E-02)
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

bool ret_false() {return false;}

void various_exercises()
{
    double anomalia_mod_2PI_ = fmod( +6*(2.0*PI)+0.3 , 2*PI );// NB. resto di divisione float
    double float_reminder = remainder( 2.0*2.0*PI , 2.0*PI );
    double float_fmod = fmod( 2.0*2.0*PI , 2.0*PI );
    bool res_1 = false & // this performs the second evaluation too
                 ret_false();
    bool res_2 = false &&  // NB. this short-circuits the second evaluation
                 ret_false();
 // no-warn
 anomalia_mod_2PI_++;
 float_reminder++;
 float_fmod++;
 res_1 |=1;
 res_2 |=1;
}// various_exercises

void someBasicContourIntegrals()
{
    // u+i*v
    Complex_Integration::fPtr_U_or_V_ realPart = Complex_Integration::genericIntegrand_u_part;
    Complex_Integration::fPtr_U_or_V_ immaginaryPart = Complex_Integration::genericIntegrand_v_part;
    // w=f(z)
    Complex_Integration::fPtr_ComplexAsScalar_ complexAsScalar = Complex_Integration::integrand_ComplexAsScalar;
    // Jordan
    Complex_Integration::fPtr_Jordan_parametriz_ abscissa = Complex_Integration::x;
    Complex_Integration::fPtr_Jordan_parametriz_ ordinate = Complex_Integration::y;
    Complex_Integration::fPtr_Jordan_parametriz_ dx_differential = Complex_Integration::dx;
    Complex_Integration::fPtr_Jordan_parametriz_ dy_differential = Complex_Integration::dy;
    //
    Numerics::Complex z0(0,1);
    Numerics::Complex z1(2,5);
    Numerics::Complex * theIntegral_result = Complex_Integration::ContourIntegral_ManagementMethod(
        z0,
        z1,
        0, 2, // extrema in the pull-back
        realPart,
        immaginaryPart,
        abscissa,
        ordinate,
        dx_differential,
        dy_differential,
        100 );// #trapezia in the partition
    delete theIntegral_result;
    //
    Numerics::Complex * complexAsScalarCoChain_result =
        Complex_Integration::ContourIntegral_AsScalar_ManagementMethod(
        z0,
        z1,
        0, 2, // extrema in the pull-back
        complexAsScalar,
        abscissa,
        ordinate,
        dx_differential,
        dy_differential,
        100 );// #trapezia in the partition
    delete complexAsScalarCoChain_result;
}// someBasicContourIntegrals
