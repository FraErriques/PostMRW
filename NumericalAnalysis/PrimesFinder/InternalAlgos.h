#ifndef INTERNALALGOS_H_INCLUDED
#define INTERNALALGOS_H_INCLUDED

namespace internalAlgos
{
    // an internal helper, which is the coChain of LogIntegral. Used for ordinal estimates.
    long double LogIntegral_coChain( long double x);
    unsigned long factorial( unsigned int par);
    // TODO:  notes on a convergent Ramanujan' series
    void ExpIntegralEi_Ramanujan( double x); //( Complex x)
    unsigned strlen_loc( char * par);
    std::string * exadecimalConverterTo( unsigned int par );
}// end nmsp internalAlgos

#endif // INTERNALALGOS_H_INCLUDED
