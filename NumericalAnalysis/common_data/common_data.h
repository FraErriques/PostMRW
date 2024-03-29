
# ifndef _COMMON_DATA_
# define _COMMON_DATA_

#include <cstddef>
#include <cmath>

typedef short Domain; // type for throwing exceptions

#define PI M_PI // once I called it that, and now I don't want to alter the whole source.
//const double PI = 3.1415926535897932384626433832795; // double PI = 6.0 * arctan ( pow (3.0, -0.5) );
//const double E  = 2.7182818284590452353602874713527;

#include <iostream>
struct Crash // a type for Exception-Handling
{
   Crash (const char * message) {std::cout << "\n\t" << message << "\n\t";}
   Crash (void)                 {char  message[] = "\n\tMemory Fault\n\n"; std::cout<<message;}
};
// another type for Exceptions is coded in Common::Exception; the boolean field is intended to be used as a semaphore for continuing.

struct Couple // the generic vector of R2, used by the interpolation methods
{
   double argument; // abscissa
   double image;    // ordinate
};
/*
# ifdef _USE_VECTOR_
   # include "vector.h"
   typedef Vector < Couple > Table; // Class for a vector of R2 points
# endif
*/

# endif

