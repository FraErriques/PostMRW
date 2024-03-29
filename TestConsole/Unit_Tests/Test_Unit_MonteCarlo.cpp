#include "Test_Unit_MonteCarlo.h"
#include "../../Common/MonteCarlo/BaseConverter.h"
#include "../../Common/MonteCarlo/ClassicalDiscreteGenerator.h"
#include "../../Common/MonteCarlo/ClassicalContinuousGenerator.h"
#include "../../Process/MonteCarlo_wrap/MonteCarlo_wrap.h"

Test_Unit_MonteCarlo::Test_Unit_MonteCarlo()
{
    //ctor
}

Test_Unit_MonteCarlo::~Test_Unit_MonteCarlo()
{
    //dtor
}

void Test_Unit_MonteCarlo::monolite()
{
 // MonteCarlo
    std::cout<<"\n\n\t -------- START discrete model;"<<std::endl;
    int left_d = 70;
    int right_d = 90;
    int populationCardinality = 9000;
    //
    Common::MonteCarlo::ClassicalDiscreteGenerator * monteCGen_d =
        new Common::MonteCarlo::ClassicalDiscreteGenerator( 70, 80);// Ctor
    for( int c=0; c<populationCardinality;c++)
    {
        monteCGen_d->nextIntegerInInterval();
    }
    monteCGen_d->showDiscretePopulation();
    monteCGen_d->buildOmega( left_d, right_d );
    monteCGen_d->buildDiscreteFrequencyDistribution();
    monteCGen_d->showFrequencyDistribution();
    monteCGen_d->showCumulatedFrequency();
    delete monteCGen_d;
    //
    std::cout<<"\n\t -------- END discrete model; START continuous model \n"<<std::endl;
    double left  = -92.809;
    double right = +12.209;
    Common::MonteCarlo::ClassicalContinuousGenerator * monteCGen_c =
        new Common::MonteCarlo::ClassicalContinuousGenerator( left, right);
    //monteCGen_c->resetExtractionInterval( left, right);// NB. compulsory
    for( int c=0; c<populationCardinality;c++)
    {
        monteCGen_c->nextDoubleInInterval();
    }
    //monteCGen_c->showContinuousPopulation();
    monteCGen_c->buildOmega( left, right );
    monteCGen_c->buildContinuousFrequencyDistribution();
    monteCGen_c->showFrequencyDistribution();
    monteCGen_c->showCumulatedFrequency();
    delete monteCGen_c;
 // MonteCarlo
 }// monolite


void Test_Unit_MonteCarlo::test_continuous()
{
    double LEFT_ = -50;
    double RIGHT_ = +30;
    int populationCardinality = 4000;
    for( double left=LEFT_, right=left+10; right<=RIGHT_; left+=10, right+=10)
    {
        Common::MonteCarlo::ClassicalContinuousGenerator * monteCGen_c =
            new Common::MonteCarlo::ClassicalContinuousGenerator( left, right);
        //monteCGen_c->resetExtractionInterval( left, right);// NB. compulsory
        for( int c=0; c<populationCardinality;c++)
        {
            monteCGen_c->nextDoubleInInterval();
        }
        //monteCGen_c->showContinuousPopulation();
        monteCGen_c->buildOmega( left, right );
        monteCGen_c->buildContinuousFrequencyDistribution();
    //    monteCGen_c->showFrequencyDistribution();
        monteCGen_c->showCumulatedFrequency();
        delete monteCGen_c;
    }
}// test_continuous


void Test_Unit_MonteCarlo::test_discrete()
{
    int LEFT_ = -50;
    int RIGHT_ = +30;
    int populationCardinality = 4000;// TODO : pass to Ctor to shape the vector capacity.---------------
    for( int left=LEFT_, right=left+10; right<=RIGHT_; left+=10, right+=10)
    {
        //instance( left, right)
        Common::MonteCarlo::ClassicalDiscreteGenerator * monteCGen_d =
            new Common::MonteCarlo::ClassicalDiscreteGenerator( left, right);// Ctor
        //job....
        for( int c=0; c<populationCardinality;c++)
        {
            monteCGen_d->nextIntegerInInterval();
        }
//        monteCGen_d->showDiscretePopulation();
        monteCGen_d->buildOmega( left, right );
        monteCGen_d->buildDiscreteFrequencyDistribution();
//        monteCGen_d->showFrequencyDistribution();
        monteCGen_d->showCumulatedFrequency();
        // resize on existing instance:
//        for( int resize_nr=0; resize_nr<3; resize_nr++)
//        {
//            // resize( left++, right--)// narrowing
//            monteCGen_d->resetExtractionInterval( left++, right--);// narrowing
//            // job...
//            for( int c=0; c<populationCardinality;c++)
//            {
//                monteCGen_d->nextIntegerInInterval();
//            }
////            monteCGen_d->showDiscretePopulation();
//            monteCGen_d->buildOmega( left, right );
//            monteCGen_d->buildDiscreteFrequencyDistribution();
////            monteCGen_d->showFrequencyDistribution();
//            monteCGen_d->showCumulatedFrequency();
//        }// for ..resized
        delete monteCGen_d;// finally delete
        monteCGen_d = nullptr;// and create a new instance
    }// for
}// test_discrete



void Test_Unit_MonteCarlo::Process_MonteCarlo_caller()
{
    std::vector<int> * dist_d = Process::MonteCarlo::getDiscreteRandomArray( 10, -50, -40);
    for( std::vector<int>::iterator it_d=dist_d->begin();
          it_d!=dist_d->end();
          it_d++
    )
    {
        std::cout<<*it_d<<std::endl;
    }
    delete dist_d;
    //
    std::cout<<"\n\t separation between Discrete and Continuous : "<<std::endl;
    //
    std::vector<double> * dist_c = Process::MonteCarlo::getContinuousRandomArray( 10, -50, -40);
    for( std::vector<double>::iterator it_c=dist_c->begin();
          it_c!=dist_c->end();
          it_c++
    )
    {
        std::cout<<*it_c<<std::endl;
    }
    delete dist_c;
} // Process_MonteCarlo_caller


//    Common::MonteCarlo::BaseConverter b(false);
//    const unsigned long threshold = +1.5E6;
//
//    for(unsigned long c=0; c<threshold; c++)
//    {
//        for( int base=2; base<=64; base++)
//        {
//            std::string * converted = b.baseOtherConverterTo( c, base);
//            unsigned long reConverted = b.base10ConverterTo( *converted, base);
//            if( reConverted != c)
//            {
//                std::cout<<"\n\t Allarme ! c:"<<c<<" base:"<<base<<std::endl;
//            }
//            else
//            {
////                std::cout<<"\n\t OK ! converted: "<<*converted
////                <<" reConverted:"<<reConverted
////                <<" original:"<<c
////                <<" base:"<<base<<std::endl;
//            }
//            delete converted;
//        }
//    }


