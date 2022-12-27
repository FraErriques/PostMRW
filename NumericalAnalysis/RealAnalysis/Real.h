
#ifndef _MY_REAL
#define _MY_REAL

#include "../common_data/common_data.h"


/// utilities
double Fabs (double x); // floating absolute value
double Fact (size_t x); // factorial

/// functions related to powers, logarithms, exponentials, ...
double Ln      (double x);                   // natural logarithm
double Nat_pow (double x, size_t n);         // integer exponent power
double Pow     (double base, double esp);    // power
double Exp     (double x);                   // exponential

/// real trigonometry
double Sin     (double x);
double Cos     (double x);
double Tan     (double x);
double Arctan  (double x);  // the arc(in radiants) whose tangent measures "x"

/// hyperbolic trigonometry
double Sinh (double x);
double Cosh (double x);
double Tanh (double x);



/// varieties between two points
namespace Two_Points_Interpolation {

struct Linear_Variety_Coefficients // line between two points
{
   // y = a*x+b = (y1-y0)/(x1-x0)*x + (y0-x0*(y1-y0)/(x1-x0))
   // Def. (y1-y0)/(x1-x0)=:Dy/Dx
   // Dy/Dx*x + y0 - x0*Dy/Dx -> Dy/Dx*(x-x0)+y0 == y(x)
   // a = (y1-y0)/(x1-x0) =: Dy/Dx
   // b = (y0-x0*(y1-y0)/(x1-x0)) = y0-x0*a == y0 - x0*Dy/Dx
   double alpha;
   double beta;
   //
   bool isProblemWellPosed;
};// a Cartesian linear equation, like y[x]==alpha*x+beta

struct Parametric_Linear_Manifold
{// a line in R2 parametrized as useful for contour integration: {x[t]=a*x+b, y[t]=c*x+d}
   // f[t]==a*t+b==(y1-y0)/(x1-x0)*(t-x0)+y0
   // Def. (y1-y0)/(x1-x0)=:Dy/Dx
   // -> f[t]==Dy/Dx*(t-x0)+y0 when fabs(x1-x0)>epsilon else {x[t]=x0,y[t]=t}
   // two coefficients for each equations are provided. But the parametrizations above are enough for all the lines in R2.
   double alpha_x;
   double beta_x;
   double alpha_y;
   double beta_y;
   //
   bool isProblemWellPosed;
};// a line in R2 parametrized as useful for contour integration: {x[t]=a*x+b, y[t]=c*x+d}

struct Exponential_Variety_Coefficients // exponential between two points
{
   /// double gamma == y0
   /// double chi   == y1/y0
   /// double psi   == x0
   /// double fi    == x1-x0
   /// y = y0*(y1/y0)^((x-x0)/(x1-x0)) = gamma*chi^((x-psi)/fi) = IMMAGINE
   /// x = fi * log_base_chi(y/gamma) + psi                     = ANTE_IMMAGINE
   double gamma; ///   y0
   double chi;   ///   y1/y0
   double psi;   ///   x0
   double fi;    ///   x1-x0
};

//obtain the coefficients of a non-vertical line between points {left,right}
Linear_Variety_Coefficients  linear( Couple left, Couple right);
//obtain the coefficients of any line between points {left,right}(i.e. even vertical)
Parametric_Linear_Manifold   linear_parametric( Couple left, Couple right);
//obtain the coefficients of an exponential curve between two points
Exponential_Variety_Coefficients  log_linear (Couple left, Couple right);
// via the coefficients of a Cartesian line, obtain y==y(x)
double linear_image        (Linear_Variety_Coefficients, double);
// via the coefficients of a Cartesian line, obtain x==x(y)
double linear_ante_image   (Linear_Variety_Coefficients, double);
// via the coefficients of an exponential function, obtain y==y(x)
double log_linear_image        (Exponential_Variety_Coefficients, double);
// via the coefficients of an exponential function, obtain x==x(y)
double log_linear_ante_image   (Exponential_Variety_Coefficients, double);
//---NB. gain {x[t],y[t]}, giving {m,t,q} to compose m*t+q
double *f( Parametric_Linear_Manifold parametric_layout, double independent_parameter);
// via the points {{x0,y0},{x1,y1}} and the Parametric_Linear_Manifold coefficients, obtain two extrema in the pullback {t0,t1}.
double *parametricLinear_ante_image( Parametric_Linear_Manifold parametric_layout
                                    ,double x0,double y0
                                    ,double x1,double y1  );

} // end of namespace Two_Points_Interpolation






/// will be used for least-squares varieties
namespace Many_Points_Interpolation {
struct Linear_Variety_Coefficients // will be used for least-squares line
{
   double alpha;
   double beta;
};
} // end of namespace Many_Points_Interpolation



namespace Newton_Raphson {
typedef double (*Fp) (double);
double f1 (double x , Fp f, const double dx=2.0e-9);
double NR (double xn, Fp f, const double dx=2.0e-9);
double Newton_Raphson ( double x0, Fp f, const double dx,
                        const unsigned short Max_Steps, const double epsilon);
}

#endif
