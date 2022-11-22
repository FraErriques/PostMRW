
#include "Complex.h" // which gives common_data.h

// la seguente #define attiva l'utilizzo di una libreria (RealAnalysis/Real) che consiste in sviluppi
// in serie di potenze, delle funzioni elementari. Tale libreria viene conservata come esercizio e memoria
// storica dei tempi di Voghera, ma e' meglio non chiamarla in produzione. In caso non sia definito il simbolo
// USE_REAL_LIB , le macro reindirizzano sulla libreria di Sistema.
// NB. non scommentare la seguente riga:
//#define  USE_REAL_LIB
#ifdef  USE_REAL_LIB
   #include "../RealAnalysis/Real.h"
#else
   #define Nat_pow pow
   #define Pow     pow
   #define Fabs    fabs
   #define Arctan  atan
   #define Ln      log
   #define Exp     exp
   #include <cmath>
#endif

namespace Numerics {

// Ctor : rectangular coordinates
Complex::Complex (double real, double immaginary)
{
   _Re = real;
   _Im = immaginary;
   //
   this->_ro = this->length();
   if( fabs(_Re)<+1E-09)
   {// adopt a trick, to do not invoke arg() on an undefined vector(0,0).
       this->_theta = 0.0;
   }
   else
   {
       this->_theta = this->arg();
   }
}// Ctor : rectangular coordinates

// Ctor : Polar coordinates
Complex::Complex( std::string isPolar, double ro, double theta)
{// NB param  isPolarFlag content is ignored, but it serves to create a valid overload.
    this->_ro = ro;
    this->_theta = theta;
    //
    this->_Re = this->_ro * cos( theta);
    this->_Im = this->_ro * sin( theta);
}// Ctor : Polar coordinates

Complex::Complex (const Complex & original)
{
   _Re = original.Re();
   _Im = original.Im();
   //
   _ro = original._ro;
   _theta = original._theta;
}

Complex & Complex::operator= (const Complex & original)
{
   _Re = original.Re();
   _Im = original.Im();
   //
   _ro = original._ro;
   _theta = original._theta;
   return *this;
}

std::string Complex::ToString() const
{
   std::string * RealPart = Common::StrManipul::doubleToString(this->Re());
   std::string * ImmaginaryPart = Common::StrManipul::doubleToString(this->Im());
   Common::StringBuilder sb(55);// forecast
   sb.append( *RealPart);
   sb.append( std::string(" +I* "));
   sb.append( *ImmaginaryPart);
   delete RealPart;
   delete ImmaginaryPart;
   // ready
   return sb.str();
}

std::string Complex::ToString_Polar() const
{
   std::string * modulus_Part = Common::StrManipul::doubleToString(this->_ro);
   std::string * argument_Part = Common::StrManipul::doubleToString(this->_theta);
   Common::StringBuilder sb(55);// forecast
   sb.append( *modulus_Part);
   sb.append( std::string(" *Exp[+I* "));
   sb.append( *argument_Part);
   sb.append( std::string("]"));
   delete modulus_Part;
   delete argument_Part;
   // ready
   return sb.str();
}

/// binary operators
Complex  Complex::operator+ (const Complex & second) const
{
   return Complex ( _Re+second.Re(), _Im+second.Im());
}

Complex  Complex::operator+  (const double    second)    const
{// Complex::+Real:
   return Complex ( _Re+second, _Im );
}

Complex  Complex::operator- (const Complex & second) const
{
   return Complex ( _Re-second.Re(), _Im-second.Im());
}

Complex  Complex::operator*  (const Complex & second) const
{
   return Complex ( _Re*second.Re() - _Im*second.Im() ,
                    _Re*second.Im() + _Im*second.Re()   );
}

Complex  Complex::operator*  (const int second)     const
{// a Complex:: times an Integer:
   return Complex ( _Re*second ,
                    _Im*second    );
}

Complex  Complex::operator*  (const double second)     const
{// a Complex:: times a Double:
   return Complex ( _Re*second ,
                    _Im*second    );
}

Complex  Complex::operator/  (const Complex & second) const
{
   Domain domain = 1; // to be thrown if the division by the null vector(0,0) is required
   Complex numerator = *this * second.conjugated();
   double normalizer = Nat_pow(second.length(),2); // modulo quadro
   if ( normalizer > 1e-50 )
     { numerator *= Complex(1.0/normalizer, 0.0); }
   else
     throw (domain);
   return numerator;
}

/// comparison binary operator
bool  Complex::operator==  (const Complex & second) const
{
# define epsilon 1.0e-13
   if ( Fabs(_Re-second._Re) > epsilon )
      {return false;}
   if ( Fabs(_Im-second._Im) > epsilon )
      {return false;}
   return true;
# undef epsilon
}
/// binary operators



/// unary operators
Complex & Complex::operator+= (const Complex & second)
{
   *this =  *this + second;
   return *this; // non const: modifies "this"
}

Complex & Complex::operator-= (const Complex & second)
{
   *this =  *this - second;
   return *this; // non const: modifies "this"
}

Complex  Complex::operator-  (void) const // unary: -z==(-1.0,0.0)*z. const: does not modify this.
{
   Complex less_one(-1.0, 0.0);
   return less_one * *this;
}

Complex & Complex::operator*= (const Complex & second)
{
   *this =  *this * second;
   return *this; // non const: modifies "this"
}

Complex & Complex::operator*= (const double second)
{
   *this =  *this * second;
   return *this; // non const: modifies "this"
}

Complex & Complex::operator/= (const Complex & second)
{
   *this =  *this / second;
   return *this; // non const: modifies "this"
}

Complex & Complex::operator^= (const Complex & second)
{
   *this =  *this ^ second;
   return *this; // non const: modifies "this"
}
/// unary operators


double Complex::length (void) const
{ // the vector modulus
   double modulus;
   try
     { modulus = Pow ( Nat_pow(_Re,2) + Nat_pow(_Im,2), 0.5 ); }
   catch ( Domain )
     {modulus = 0.0;} // trap error; the null vector requires e^(0.5*ln(0)) but its length is 0
   return modulus;
}// the vector modulus

Complex Complex::conjugated  (void) const
{
   return Complex ( _Re, -_Im );
}

Complex Complex::real_factor (double factor) const // returns a temporary
{
   return Complex ( _Re * factor, _Im * factor );
}

double Complex::arg (void) const
{
   double res;
   if ( Fabs(_Re) < 1e-50 )
      {
         if ( Fabs(_Im) < 1e-50 )
            { Domain domain=1; throw domain; }
         if ( _Im > 0.0 )
            { res =  PI/2;}
         else if ( _Im < 0.0 )
            { res = -PI/2;}
      }
   else if ( _Re > 0.0 )
      { res =  Arctan ( _Im / _Re ); }
   else if ( _Re < 0.0 )
   {
         if ( Fabs(_Im) < 1e-50 )
            { res = -PI; }
         else if ( _Im > 0.0 )
            { res = Arctan ( _Im / _Re ) + PI;}
         else if ( _Im < 0.0 )
            { res = Arctan ( _Im / _Re ) - PI;}
   }

   return res;
}




///########################### Complex Analisys section ###########################################
Complex Complex::LnC (void) const
{
   Domain domain = 1; // to be thrown, to avoid ln(0), the famous Pole
   double modulus = length();
   double real;
   if ( modulus < 1e-60 ) { throw domain; } // error code; the null vector requires e^(0.5*ln(0)) but its length is 0
   real = Ln ( modulus );
   double immaginary = arg(); // i(arg(z)+2kPI) : ramo principale k==0
   return Complex ( real, immaginary );
}


Complex Complex::Nat_powC (size_t n) const // integer exponent power
{
   Complex res (1.0, 0.0); // product invariant
   for (size_t j=0; j<n; j++)
      { res *= *this; }
   return res;
}

// scritta a Voghera nel 2001 ! :)
Complex Complex::ExpC_selfish (void) const  // exponential e^z
{
   Complex res(0.0, 0.0);
   if ( Fabs(_Im)<1e-70 )
   {// if z is on the real axis, use the optimized real version of exp
     res._Re = Exp (this->_Re);
     return res;
   }
   size_t k;
   double the_fact = 1.0; // the factorial in ... fk(x0)/k!
   for (k=0; k<7e1; k++) // reduct order set at 7e1
      {
         if (k>1) { the_fact *= k; }
         res += (this->Nat_powC(k)).real_factor(1.0/the_fact);
      }
   return res;
}

Complex Complex::ExpC (void) const  // exponential e^z
{// Exp[x+I*y]==Exp[x]*Exp[I*y]==Exp[x]*(Cos[y]+I*Sin[y])
    double Exp_Re = exp( this->Re());
    double Cos_Im = cos( this->Im());
    double Sin_Im = sin( this->Im());
    Complex res( Cos_Im, Sin_Im);
    res *= Exp_Re;
   return res;
}



Complex  Complex::operator^  (const Complex & exponent) const
{// z^exponent = e^ (exponent ln z)
   Complex res ( exponent * this->LnC() ); // by a copy constructor
   return res.ExpC();
}

Complex Complex::operator^    (const int exponent)       const
{// z^exponent = e^ (exponent ln z) with Integer exponent
   Complex res( this->LnC() * exponent ); // by a copy constructor: NB. the operator(*) accepts the Integer only as second factor.
   return res.ExpC();
}


///########## trigonometric sub-section    ###########################################################
Complex  Complex::SinC    (void) const
{
  Complex res;
  Complex     iz = i * *this;
  Complex  minus_iz = -i * *this;
  Complex e_iz       =       iz.ExpC();  // e^(i*z)
  Complex e_minus_iz = minus_iz.ExpC();  // e^(-i*z)
  Complex factor ( 0.0, -0.5 ); // 1/(2i)==0.5*(-i)
  res =  ( e_iz - e_minus_iz ) * factor;
  return res;
}


Complex  Complex::CosC    (void) const
{
  Complex res;
  Complex     iz = i * *this;
  Complex  minus_iz = -i * *this;
  Complex e_iz       =       iz.ExpC();  // e^(i*z)
  Complex e_minus_iz = minus_iz.ExpC();  // e^(-i*z)
  Complex factor ( 0.5, 0.0 ); // 1/2==0.5+0*i
  res =  ( e_iz + e_minus_iz ) * factor;
  return res;
}


Complex  Complex::TanC    (void) const
{
  Complex res;
  Complex sin, cos;
  sin = this->SinC();
  cos = this->CosC();
  if ( cos.length() < 1e-16 )
     { Domain domain=1; throw domain; }
  res = sin / cos;
  return res;
}


/// trigonometric inverse functions
Complex  Complex::ArcSinC    (void) const
{
  Complex one (1.0, 0.0);
  Complex square_z = this->Nat_powC(2);
  Complex iz = i * *this;
  Complex factor (0.0, -1.0); // 1/i = -i
  Complex root = one - square_z;
  Complex half (0.5, 0.0);
  root ^= half;
  Complex to_log = iz + root;
  Complex post_log = to_log.LnC();
  post_log *= factor;
  return  post_log;
}


Complex  Complex::ArcCosC    (void) const
{
  Complex one (1.0, 0.0);
  Complex square_z = this->Nat_powC(2);
  Complex factor (0.0, -1.0); // 1/i = -i
  Complex root = square_z - one;
  Complex half (0.5, 0.0);
  root ^= half;
  Complex to_log = *this + root;
  Complex post_log = to_log.LnC();
  post_log *= factor;
  return  post_log;
}


Complex  Complex::ArcTanC    (void) const
{
  Complex one (1.0, 0.0);
  Complex iz = i * *this;
  Complex factor (0.0, -0.5); // 1/(2i) = (1/2)(-i) = (0.0 , -0.5)
  Complex numerator = one + iz;
  Complex denominator = one - iz;
  Complex to_log = numerator / denominator;
  Complex post_log = to_log.LnC();
  post_log *= factor;
  return  post_log;
}





///########## hyperbolic sub-section  #############################################################
Complex  Complex::SinhC    (void) const
{ //   1/2 (e^z-e^-z)
  Complex res;
  Complex  minus_z = -*this;
  Complex e_z       =   this->ExpC();  // e^( z)
  Complex e_minus_z = minus_z.ExpC();  // e^(-z)
  Complex factor ( 0.5, 0.0 );
  res =  ( e_z - e_minus_z ) * factor;
  return res;
}


Complex  Complex::CoshC    (void) const
{ //   1/2 (e^z+e^-z)
  Complex res;
  Complex  minus_z = -*this;
  Complex e_z       =   this->ExpC();  // e^( z)
  Complex e_minus_z = minus_z.ExpC();  // e^(-z)
  Complex factor ( 0.5, 0.0 );
  res =  ( e_z + e_minus_z ) * factor;
  return res;
}


Complex  Complex::TanhC    (void) const
{
  Complex res;
  Complex sinhyp, coshyp;
  sinhyp = this->SinhC();
  coshyp = this->CoshC();
  if ( coshyp.length() < 1e-16 )
     { Domain domain=1; throw domain; }
  res = sinhyp / coshyp;
  return res;
}

/// hyperbolic inverse functions
Complex  Complex::SetSinhC    (void) const
{
  Complex one (1.0, 0.0);
  Complex square_z = this->Nat_powC(2);
  Complex root =  square_z + one ;
  Complex half (0.5, 0.0);
  root ^= half;
  Complex to_log = *this + root;
  Complex post_log = to_log.LnC();
  return  post_log;
}

Complex  Complex::SetCoshC    (void) const
{
  Complex one (1.0, 0.0);
  Complex square_z = this->Nat_powC(2);
  Complex root =  square_z - one ;
  Complex half (0.5, 0.0);
  root ^= half;
  Complex to_log = *this + root;
  Complex post_log = to_log.LnC();
  return  post_log;
}

Complex  Complex::SetTanhC    (void) const
{
  Complex one (1.0, 0.0);
  Complex factor (0.5, 0.0); // 1/2
  Complex numerator   = one + *this;
  Complex denominator = one - *this;
  Complex to_log = numerator / denominator;
  Complex post_log = to_log.LnC();
  post_log *= factor;
  return  post_log;
}

} // closing namespace Numerics
