#ifndef TEST_UNIT_COMPLEX_H
#define TEST_UNIT_COMPLEX_H

#include "../../NumericalAnalysis/Complex/Complex.h"

class Test_Unit_Complex
{
    public:
        /** Default constructor */
        Test_Unit_Complex();
        /** Default destructor */
        virtual ~Test_Unit_Complex();
        //
//NB. CoChains and in general function-pointees have to be declared as local function in the .cpp file only of  the test class, and not as members.
        void manage_ComplexIntegr_asScalar_square();
        void similErf_test();
        void test_Ctor_s_();
        void test_AmpliTwist();

    protected:

    private:
};

#endif // TEST_UNIT_COMPLEX_H
