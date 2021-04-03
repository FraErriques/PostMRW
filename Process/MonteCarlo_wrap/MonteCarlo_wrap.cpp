
#include <iostream>
#include "MonteCarlo_wrap.h"

 void Process::testMonteCarlo()
 {
//    Common::MonteCarlo * monteCarlo = new Common::MonteCarlo();
//    //
//    for(int c=0; c<10; c++)
//    {
//        std::cout<<"\n\t random integer at step "<<c<<" : "<< monteCarlo->nextInteger();
//        std::cout<<"\n\t random probability at step "<<c<<" : "<< monteCarlo->nextProbabilityMeasure();
//        std::cout<<"\n\t SUP in random integers is : "<< monteCarlo->showSup();
//        std::cout<<"\n\t current-SEED in random integers is : "<< monteCarlo->showCurrentSeed() <<std::endl;
//    }
//    //
//    delete monteCarlo;
    // ready.
 }// end test


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
