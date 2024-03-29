#ifndef TEST_UNIT_MONTECARLO_H
#define TEST_UNIT_MONTECARLO_H


class Test_Unit_MonteCarlo
{
    public:
        /** Default constructor */
        Test_Unit_MonteCarlo();
        /** Default destructor */
        ~Test_Unit_MonteCarlo();
        static void monolite();
        static void test_discrete();
        static void test_continuous();
        static void Process_MonteCarlo_caller();

    protected:

    private:
        Test_Unit_MonteCarlo(const Test_Unit_MonteCarlo &other);// let Copy-Ctor private, so to hide it.
        Test_Unit_MonteCarlo operator=(const Test_Unit_MonteCarlo &other);// let assignement operator private, so to hide it.
};

#endif // TEST_UNIT_MONTECARLO_H
