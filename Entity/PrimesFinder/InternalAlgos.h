#ifndef INTERNALALGOS_H_INCLUDED
#define INTERNALALGOS_H_INCLUDED

namespace internalAlgos
{

// an internal helper, which is the coChain of LogIntegral. Used for ordinal estimates.
double LogIntegral_coChain( double x);

unsigned long factorial( unsigned int par);

// TODO:  notes on a convergent Ramanujan' series
void ExpIntegralEi_Ramanujan( double x); //( Complex x)

}// end nmsp internalAlgos

#endif // INTERNALALGOS_H_INCLUDED
