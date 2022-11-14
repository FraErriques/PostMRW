#ifndef MONTECARLO_WRAP_H_INCLUDED
#define MONTECARLO_WRAP_H_INCLUDED



namespace Process{
namespace MonteCarlo{

    std::vector<int> * getDiscreteRandomArray(
    size_t arrayCardinality
    , int intervalLeftBoundary, int intervalRightBoundary  );

    std::vector<double> * getContinuousRandomArray(
    size_t arrayCardinality
    , double intervalLeftBoundary, double intervalRightBoundary  );

    // modern
    void testCpp11_MonteCarlo();

}// nmsp
}// nmsp

#endif // MONTECARLO_WRAP_H_INCLUDED
