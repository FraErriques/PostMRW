#ifndef TEST_UNIT_LOGGER_H
#define TEST_UNIT_LOGGER_H

#include "../Common/StringBuilder/StringBuilder.h"
#include "../Common/LogSinkFs/SinkFs.h"
#include "../Common/ThreadForker/ThreadForker.h"
#include "../Common/LogFs_wrap/LogFs_wrap.h"

class Test_Unit_Logger
{
    public:
        /** Default constructor */
        Test_Unit_Logger();
        /** Default destructor */
        virtual ~Test_Unit_Logger();
        //
        void managementMethod();

    protected:

    private:
        void autonomous_logger(int threadNum);
        void singleton_logger(int threadNum);
};

#endif // TEST_UNIT_LOGGER_H
