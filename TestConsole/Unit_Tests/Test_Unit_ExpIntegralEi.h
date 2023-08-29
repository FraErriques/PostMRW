#ifndef TEST_EXPINTEGRALEI_H
#define TEST_EXPINTEGRALEI_H


/* Scope and purpose:
    after Hadamard factorization, we have a Zeta(s) expressed in five addends;
    such representation of Zeta(s) has to be used in the equation (1/s)*Log[Zeta(s)]=Integrate[J[x]*x^(-s-1),{x,0,*Infinity]
    The need to have an explicit representation of J[x] leads us to do the inverse Mellin Transform, on each of the five addenda.
    After some(complex) calculations, the addends remain only four; the first two of them involve the LogIntegral.
    The first one on the positive real semi-axis.
    The second one on a Complex contour, which requires the Analytic-Continuation of the real LogIntegral.
    The free bound is x^ro, x in R+, ro in C. The free bound points form a circle.
    The LogIntegral[x^ro] is suitable to be transformed into ExpIntegralEi[ro*Log[x]].
    Due to this reason, and to the convenience of this last form in NumericalAnalysis, this class is created,
    for the necessary tests.
*/
class Test_ExpIntegralEi
{
    public:
        /** Default constructor */
        Test_ExpIntegralEi();
        /** Default destructor */
        virtual ~Test_ExpIntegralEi();

    protected:

    private:
};

#endif // TEST_EXPINTEGRALEI_H
