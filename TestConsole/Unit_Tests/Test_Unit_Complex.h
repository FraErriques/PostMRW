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

    protected:
    private:
        Test_Unit_Complex();// Ctor
        ~Test_Unit_Complex();// Dtor
};

#endif // TEST_UNIT_COMPLEX_H
