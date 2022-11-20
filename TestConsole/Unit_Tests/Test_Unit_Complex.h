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
        Numerics::Complex ErrorFunction( double x, double y);
        void manage_ComplexIntegr_asScalar();

    protected:

    private:
};

#endif // TEST_UNIT_COMPLEX_H
