#ifndef MODERNRAND_H_INCLUDED
#define MODERNRAND_H_INCLUDED

#include <random>
#include <chrono>
#include <thread>


namespace Common
{



//==============================================================
// RANDOM BACKOFF TIME
//==============================================================
class RandCpp11
{
  public:
    std::random_device                      rd;
    std::mt19937                            mt;
    std::uniform_real_distribution<double>  dist;

    RandCpp11() : rd{}, mt{rd()}, dist{ 0.0, +1.0} {}

    double rand();
};

}// end nmsp Common


#endif // MODERNRAND_H_INCLUDED
