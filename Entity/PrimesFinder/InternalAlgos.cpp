#include <cmath>

namespace internalAlgos
{



// an internal helper, which is the coChain of LogIntegral. Used for ordinal estimates.
double LogIntegral_coChain( double x)
{// an internal helper, which is the coChain of LogIntegral. Used for ordinal estimates.
    return +1.0/log(x);
}// LogIntegral_coChain


unsigned long factorial( unsigned int par)
{
    unsigned long res = +1UL;
    for( int c=par; c>+1; c--)
    {
        res *= c;
    }
    return res;
}// factorial


// TODO:
    void ExpIntegralEi_Ramanujan( double x)//( Complex x)
    {// notes on a convergent series
//
//        N[(+EulerGamma + Log[x]) +
//          Exp[x/2]*Sum[((-1)^(n - 1) (+x)^n)/((n!)*(2^(n - 1)))*
//             Sum[+1/(2*k + 1), {k, 0, Floor[(n - 1)/2]}], {n, +1, +99}]]
//
//
//        N[(+EulerGamma + Log[x]) +
//          Exp[x/2]*Sum[((-1)^(n - 1) (+x)^n)/((n!)*(2^(n - 1)))*
//             Sum[+1/(2*k + 1), {k, 0, Floor[(n - 1)/2]}], {n, +1, +99}]]
//

//        double internalFactor = 0.0;
//        double externalFactor = 0.0;
//        for( int n=+1; n<=+99; n++)
//        {
//            for(int k=0; k<floor(((double)n - 1.0)/2.0); k++)
//            {
//                internalFactor += +1.0/(2.0*(double)k + 1.0);
//            }
//            externalFactor += ((-1.0)^((double)n - 1.0) (+x)^(double)n)/((n!)*(2.0^(n - 1))) * internal_factor;
//            internalFactor = 0.0;// reset.
//        }
//        externalFactor += EulerGamma + Log[x];

        const double EulerGamma = +0.577216;
        double internalFactor = 0.0;
        double externalFactor = 0.0;
        for( int n=+1; n<=+99; n++)
        {
            for(int k=0; k< floor(((double)n - 1.0)/2.0); k++ )
            {
                internalFactor += +1.0/(2.0*(double)k + 1.0);
            }

            externalFactor += pow((-1.0),((double)n - 1.0)) * pow(+x,(double)n) /(factorial(n)* pow(2.0,((double)n - 1)) ) * internalFactor;
            internalFactor = 0.0;// reset.
        }
        externalFactor += EulerGamma + log(x);

  }// Ramanujan series


}// end nmsp internalAlgos
