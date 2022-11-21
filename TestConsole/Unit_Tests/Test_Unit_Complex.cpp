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
            , 10000 );// #steps
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
            , 10000 );// #steps
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
            , 10000 );// #steps
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
            , 10000 );// #steps
    std::cout << "Integrate Exp[-z^2]dz {z,(0,0),(1,1) == " << res_whole->ToString() << std::endl;
    std::cout << "Sum Intg[(0,0),(1,0)]+Intg[(1,0),(1,1)] == " <<
     (*res_real+*res_img).ToString() << std::endl;
    delete res_real;
    delete res_img;
    delete res_whole;
    // ready
}// similErf_test
