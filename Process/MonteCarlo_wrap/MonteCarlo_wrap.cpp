#include <iostream>
#include <vector>
#include "MonteCarlo_wrap.h"
#include "../../Common/MonteCarlo/ClassicalDiscreteGenerator.h"
#include "../../Common/MonteCarlo/ClassicalContinuousGenerator.h"
#include "../../Common/MonteCarlo/modernRand.h"


std::vector<int> * Process::MonteCarlo::getDiscreteRandomArray(
    size_t arrayCardinality
    , int intervalLeftBoundary, int intervalRightBoundary  )
{
    Common::MonteCarlo::ClassicalDiscreteGenerator cdg( intervalLeftBoundary, intervalRightBoundary);
    std::vector<int> * result = new std::vector<int>();
    for( size_t c=0; c<arrayCardinality; c++)
    {
        result->push_back( cdg.nextIntegerInInterval());
    }
    // ready
    return result;// caller has to delete
}// getDiscreteRandomArray

std::vector<double> * Process::MonteCarlo::getContinuousRandomArray(
    size_t arrayCardinality
    , double intervalLeftBoundary, double intervalRightBoundary  )
{
    Common::MonteCarlo::ClassicalContinuousGenerator cdg( intervalLeftBoundary, intervalRightBoundary);
    std::vector<double> * result = new std::vector<double>();
    for( size_t c=0; c<arrayCardinality; c++)
    {
        result->push_back( cdg.nextDoubleInInterval());
    }
    // ready
    return result;// caller has to delete
}// getContinuousRandomArray



//#define LinuxPlatf
#ifdef LinuxPlatf
void Process::MonteCarlo::testCpp11_MonteCarlo()
{
    //thread_local
    Common::RandCpp11 * randCpp11 = new Common::RandCpp11();
    //
    for(int c=0; c<100; c++)
    {
        std::cout<<"\n\t random Common::RandCpp11 extraction at step "<<c<<" : "<< randCpp11->rand();
    }
    std::cout << std::endl;
    delete randCpp11;
}// end void Process::testCpp11_MonteCarlo()
#endif
