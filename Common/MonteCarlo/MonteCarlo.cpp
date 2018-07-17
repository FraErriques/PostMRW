//
//#include <stdio.h>      /* NULL */
//#include <stdlib.h>     /* RAND_MAX , srand , rand */
//#include <time.h>       /* time */
//#include "MonteCarlo.h"
//#include "../StringBuilder/StringBuilder.h"
//#include <cmath>
//
///*
//    If seed is set to 1, the generator is reinitialized to its initial value and produces the same values as
//    before any call to rand or srand.
//    Two different initializations with the same seed will generate the same succession of results
//    in subsequent calls to rand.
//*/
//
//
//Common::MonteCarlo::MonteCarlo( unsigned int seed) : currentSeed(seed), SUP(RAND_MAX)
//{
//    srand( seed);
//}// END Common::MonteCarlo( unsigned int seed)
//
//Common::MonteCarlo::MonteCarlo() : currentSeed( time(NULL)), SUP(RAND_MAX)
//{
//    srand( this->currentSeed);// i.e. time(NULL)
//}// END Common::MonteCarlo()
//
//int Common::MonteCarlo::nextInteger() const
//{
//    return rand();
//}// END nextInteger
//
//int Common::MonteCarlo:: nextIntegerInInterval(int Min, int Threshold) const
//{
//    double omothetia = (double)(Threshold-Min)/(double)this->SUP;
//    return rand()*omothetia+Min;
//}// END nextIntegerInInterval
//
//double Common::MonteCarlo::nextProbabilityMeasure() const
//{
//    return ( ((double)rand())/((double)SUP) );
//}// END nextProbabilityMeasure
//
//unsigned int Common::MonteCarlo::showCurrentSeed() const
//{
//    return this->currentSeed;
//}
//
//int Common::MonteCarlo::showSup() const
//{
//    return this->SUP;
//}
//
//
////typedef struct _GUID {
////    unsigned long  Data1;
////    unsigned short Data2;
////    unsigned short Data3;
////    unsigned char  Data4[8];
////} GUID;
//std::string * Common::MonteCarlo::getGUID() const
//{// get a 16-figures string, where each figure is randomized within a range.
//    Common::StringBuilder * sb = new Common::StringBuilder(17) ;
//    for( int c=0; c<16; c++)
//    {// 16 figures.
//        int tmp = -1;
//        while( 33>tmp || 126<tmp)
//        {// extract until we're in the desired range.
//            tmp = this->nextProbabilityMeasure() * 127;// selected code
//        }
//        sb->append( tmp);
//    }// for
//    std::string * res = new std::string( sb->str() );// caller has to delete.
//    delete sb;
//    return res;
//}// getGUID()
