#ifndef TEST_UNIT_SELECTINTERVAL_H
#define TEST_UNIT_SELECTINTERVAL_H

// This is a test class about the selection of the belonging interval of a candidate prime.
// the subject matter is to determine the interval of integration, of the LogIntegral, which the candidate
// point belongs to. The intervals are defined by the pillars of the current mesh. The belonging interval
//gives information about the nearest available integration threshold and the remaining portion, which has to
//be integrated at the moment.
class Test_Unit_selectInterval
{
    public:
        Test_Unit_selectInterval();// Ctor
        ~Test_Unit_selectInterval();// Dtor
        //
        static void managementMethod();

    protected:

    private:
        static int selectInterval(int candidate);
};

#endif // TEST_UNIT_SELECTINTERVAL_H
