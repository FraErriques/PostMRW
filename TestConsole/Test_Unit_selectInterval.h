#ifndef TEST_UNIT_SELECTINTERVAL_H
#define TEST_UNIT_SELECTINTERVAL_H


class Test_Unit_selectInterval
{
    public:
        /** Default constructor */
        Test_Unit_selectInterval();
        /** Default destructor */
        virtual ~Test_Unit_selectInterval();
        //
        void managementMethod();

    protected:

    private:
        int selectInterval(int candidate);
};

#endif // TEST_UNIT_SELECTINTERVAL_H
