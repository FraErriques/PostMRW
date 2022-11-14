#include <stdlib.h>     /* RAND_MAX , srand , rand */
#include "Test_Unit_RealAnalysis.h"
#include "../../NumericalAnalysis/RealAnalysis/Real.h"
#include "../../Common/MonteCarlo/ClassicalDiscreteGenerator.h"
#include "../../Common/MonteCarlo/ClassicalContinuousGenerator.h"



Test_Unit_RealAnalysis::Test_Unit_RealAnalysis()
{
    //ctor
}

Test_Unit_RealAnalysis::~Test_Unit_RealAnalysis()
{
    //dtor
}

void Test_Unit_RealAnalysis::LinearInterpolatorCoefficients()
{
    Couple left;
    left.argument = 0;
    left.image = -50;
    //
    Couple right;
    right.argument = RAND_MAX;
    right.image = -40;
    //
    Two_Points_Interpolation::Linear_Variety_Coefficients linearCoeffs =
        Two_Points_Interpolation::linear( left, right);
    //
    Common::MonteCarlo::ClassicalDiscreteGenerator cdg( -50, -40);// auto
    Common::MonteCarlo::ClassicalContinuousGenerator ccg( -50, -40);// auto
    // ready.
    linearCoeffs.alpha++;// no warn.
}// LinearInterpolatorCoefficients
