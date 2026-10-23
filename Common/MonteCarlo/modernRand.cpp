
#include "modernRand.h"

using namespace std;

namespace Common
{


    double RandCpp11::rand()
    {
      return dist(mt);
    }


}// end nmsp Common
