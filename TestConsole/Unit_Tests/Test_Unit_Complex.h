#ifndef TEST_UNIT_COMPLEX_H
#define TEST_UNIT_COMPLEX_H

#include "../../NumericalAnalysis/Complex/Complex.h"

class Test_Unit_Complex
{
    public:

        //
        //NB. CoChains and in general function-pointees have to be declared as local function
        // in the .cpp file only, of  the test class, and not as members.
        static void manage_ComplexIntegr_asScalar_square();
        static void similErf_test();
        static void test_Ctor_s_();
        static void test_AmpliTwist();
        static void ExpIntegralEi_test();
        static void ContourIntegral_AsScalar_JordanLinearAutoDetect_test(
            Numerics::Complex z0
            ,Numerics::Complex z1
            ,unsigned long long n_trapezia );// #trapezia in the partition
        static void ContourIntegral_AsScalar_JordanLinearAutoDetect_ExpIntegralEiRiemann_test(
            // no more -- Numerics::Complex z0  --fixed on minus_Infinity and Integral cached until log(sqrt(2)).
            Numerics::Complex z1
            ,unsigned long long n_trapezia );// #trapezia in the partition
        static Numerics::Complex *ContourIntegral_AsScalar_JordanLinearAutoDetect_onExpIntegralEiCoChain_test(
            Numerics::Complex z0
            ,Numerics::Complex z1
            ,unsigned long long n_trapezia );// #trapezia in the partition

    protected:
    private:
        Test_Unit_Complex();// Ctor
        ~Test_Unit_Complex();// Dtor
};

#endif // TEST_UNIT_COMPLEX_H
