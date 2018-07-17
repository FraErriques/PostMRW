//#ifndef MONTECARLO_H_INCLUDED
//#define MONTECARLO_H_INCLUDED
//
//#include <string>
//
///*
//    If seed is set to 1, the generator is reinitialized to its initial value and produces the same values as
//    before any call to rand or srand.
//    Two different initializations with the same seed will generate the same succession of results
//    in subsequent calls to rand.
//*/
//
//namespace Common
//{
//
//
//    class MonteCarlo
//    {
//    private:
//        unsigned int currentSeed;
//        const int SUP; // = RAND_MAX;// #include <stdlib.h>
//
//    public:
//        MonteCarlo( unsigned int seed);
//        MonteCarlo();
//        //
//        int nextInteger() const;
//        int nextIntegerInInterval(int Min, int Threshold) const;
//        double nextProbabilityMeasure() const;
//        std::string * getGUID() const;
//        unsigned int showCurrentSeed() const;
//        int showSup() const;
//
//    };// END class MonteCarlo
//
//
//}// END namespace Common
//
//#endif // MONTECARLO_H_INCLUDED
