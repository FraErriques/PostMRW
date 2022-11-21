
#ifndef _MY_COMPLEX_
#define _MY_COMPLEX_

#include "../common_data/common_data.h"
#include "../../Common/StringBuilder/StringBuilder.h"

namespace Numerics {

class Complex
{
public:
   /// Constructors and Destructors
   Complex              (double real=0.0, double immaginary=0.0);// default params for rectangular coordinates
   // TODO  Complex  a Ctor for Polar coordinates : cannot overload, since the params are exactly the same TODO
   Complex              (double ro, double theta, bool isPolar);
   // Copy Constructor
   Complex              (const Complex & original);
   // operator=
   Complex & operator=  (const Complex & original);
   // Destructor
   ~Complex             (void){} // let it virtual if You derive

   double       Re          (void)           const             {return _Re;}
   double       Im          (void)           const             {return _Im;}
   void         set_Re      (double par)     /* non-const */   {_Re = par; }
   void         set_Im      (double par)     /* non-const */   {_Im = par; }
   double       length      (void)           const;
   double       arg         (void)           const;
   Complex      conjugated  (void)           const;
   Complex      real_factor (double factor)  const;
   std::string  ToString    (void)           const;


   /// binary operators /////////////////////////////////////////////////////
   Complex  operator+  (const Complex & second)    const;
   Complex  operator+  (const double    second)    const;
   Complex  operator-  (const Complex & second)    const;
   Complex  operator*  (const Complex & second)    const;
   Complex  operator*  (const int second)          const;
   Complex  operator*  (const double second)       const;
   Complex  operator/  (const Complex & second)    const;
   bool  operator==    (const Complex & second)    const;
   /// unary operators ////////////////////////////////////////////////////////
   Complex & operator+= (const Complex & second);  // non-const
   Complex & operator-= (const Complex & second);  // non-const
   Complex   operator-  (void)                     const; // unary: -z==(-1.0,0.0)*z. const: does not modify this
   Complex & operator*= (const Complex & second);  // non-const
   Complex & operator*= (const double    second);  // non-const
   Complex & operator/= (const Complex & second);  // non-const
   Complex & operator^= (const Complex & second);  // non-const: *this = *this^second
   /// Complex Analisys section /////////////////////////////////////////////////////////////////////////////////////////
   Complex LnC          (void)                     const;
   Complex ExpC         (void)                     const;
   Complex ExpC_selfish (void)                     const;  // exponential e^z mediante serie di McLaurin
   Complex Nat_powC     (size_t n)                 const;// integer exponent power
   Complex operator^    (const Complex & exponent) const;
   Complex operator^    (const int exponent)       const;
   // trigonometric sub-section /////////////////////////
   Complex SinC         (void)                     const;
   Complex CosC         (void)                     const;
   Complex TanC         (void)                     const;
   Complex ArcSinC      (void)                     const;
   Complex ArcCosC      (void)                     const;
   Complex ArcTanC      (void)                     const;
   // hyperbolic sub-section ////////////////////////////
   Complex SinhC        (void)                     const;
   Complex CoshC        (void)                     const;
   Complex TanhC        (void)                     const;
   Complex SetSinhC     (void)                     const;
   Complex SetCoshC     (void)                     const;
   Complex SetTanhC     (void)                     const;

private:
   /// data /////////////////////////////////////////////////////////////////////////////////////////
   double _Re; // rectangular Cartesian coordinates
   double _Im;
   double _ro; // polar coordinates
   double _theta;
};// end Class Complex

const Complex i(0.0, 1.0);

} // closing namespace Numerics

# endif
