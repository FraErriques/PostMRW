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

Numerics::Complex Test_Unit_Complex::ErrorFunction( double x, double y)// the whole image, treated as an algebraic scalar.
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


void Test_Unit_Complex::manage_ComplexIntegr_asScalar()
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
    delete res;
    // ready
}// manage_ComplexIntegr_asScalar
