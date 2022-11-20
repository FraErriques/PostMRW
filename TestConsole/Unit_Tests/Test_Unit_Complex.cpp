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


//y[x]==-9.12517 + 3.35696 x
//{t0, t1} == {E, 0.5226811514040275`}
//z0 == {0, 1}
//z1 == {0.5 , +1.5}
//f (z0) == {E, 0}
//f (z1) == {0.5226811514040275 , -7.370546414419436}


double local_abscissa( double t){return t;}
double local_ordinate( double t)
    {return (3.35696)*t + (-9.12517);}
double local_Dabscissa( double t){return +1;}
double local_Dordinate( double t){return (3.35696);}


void Test_Unit_Complex::manage_ComplexIntegr_asScalar()
{
    Complex_Integration::fPtr_ComplexAsScalar_ errFunc =
        (Complex_Integration::fPtr_ComplexAsScalar_)(&Test_Unit_Complex::ErrorFunction);
    // Jordan
    Complex_Integration::fPtr_Jordan_parametriz_ abscissa = local_abscissa;
    Complex_Integration::fPtr_Jordan_parametriz_ ordinate = local_ordinate;
    Complex_Integration::fPtr_Jordan_parametriz_ dx = local_Dabscissa;
    Complex_Integration::fPtr_Jordan_parametriz_ dy = local_Dordinate;
    //
    //y[x]==-9.12517 + 3.35696 x
    //{t0, t1} == {E, 0.5226811514040275`}
    //z0 == {0, 1}
    //z1 == {0.5 , +1.5}
    //f (z0) == {E, 0}
    //f (z1) == {0.5226811514040275 , -7.370546414419436}
    Numerics::Complex z0 = ErrorFunction(0,1); // points on the image-plane, through the CoChain
    Numerics::Complex z1 = ErrorFunction(0.5 , +1.5); // points on the image-plane, through the CoChain
    double t0 = exp(+1); // SAME points on the image-plane, reached via PullBack, through the Chain
    double t1 = (+0.5226811514040275);//{t0, t1} == {E, 0.5226811514040275`}
    Numerics::Complex *res =
        Complex_Integration::ContourIntegral_AsScalar_ManagementMethod(
              z0 // points on the image-plane, through the CoChain
            , z1 // points on the image-plane, through the CoChain
            , t0 // SAME points on the image-plane, reached via PullBack, through the Chain
            , t1 // SAME points on the image-plane, reached via PullBack, through the Chain
            , errFunc
            , abscissa, ordinate
            , dx, dy
            , 100 );// #steps
    delete res;
    // ready
}// manage_ComplexIntegr_asScalar
