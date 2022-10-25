
#include <iostream>
#include "MonteCarlo_wrap.h"
#include "../../Common/MonteCarlo/ClassicalDiscreteGenerator.h"
#include "../../Common/MonteCarlo/ClassicalContinuousGenerator.h"
#include "../../Common/MonteCarlo/modernRand.h"


// void Process::testMonteCarlo()
// {
//     Common::MonteCarlo::ClassicalDiscreteGenerator discrete_monteCarlo;
//     Common::MonteCarlo::ClassicalContinuousGenerator continuous_monteCarlo;
////    Common::MonteCarlo::ClassicalDiscreteGenerator * discrete_monteCarlo = new Common::MonteCarlo::ClassicalDiscreteGenerator();
////    Common::MonteCarlo::ClassicalContinuousGenerator * continuous_monteCarlo = new Common::MonteCarlo::ClassicalContinuousGenerator();
//    //
//    for(int c=0; c<10; c++)
//    {
//        discrete_monteCarlo.nextIntegerInInterval();
//        discrete_monteCarlo.showDiscretePopulation();
//        //std::cout<<"\n\t random integer at step "<<c<<" : "<< discrete_monteCarlo->showDiscretePopulation();
//        continuous_monteCarlo.nextDoubleInInterval();
//        continuous_monteCarlo.showFrequencyDistribution();
//        //std::cout<<"\n\t random probability at step "<<c<<" : "<< continuous_monteCarlo->showFrequencyDistribution();
//        continuous_monteCarlo.showSup();
//        //std::cout<<"\n\t SUP in random integers is : "<<
//        continuous_monteCarlo.showCurrentSeed();
//        //std::cout<<"\n\t current-SEED in random integers is : "<< <<std::endl
//    }
//    //
////    delete discrete_monteCarlo;
////    delete continuous_monteCarlo;
//    // ready.
// }// end test


void Process::testCpp11_MonteCarlo()
{
    thread_local Common::RandCpp11 * randCpp11 = new Common::RandCpp11();
    //
    for(int c=0; c<100; c++)
    {
        std::cout<<"\n\t random Common::RandCpp11 extraction at step "<<c<<" : "<< randCpp11->rand();
    }
    std::cout << std::endl;
    delete randCpp11;
}// end void Process::testCpp11_MonteCarlo()
