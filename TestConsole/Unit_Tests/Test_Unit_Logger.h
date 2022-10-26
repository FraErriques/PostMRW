#ifndef TEST_UNIT_LOGGER_H
#define TEST_UNIT_LOGGER_H

#include "../../Common/StringBuilder/StringBuilder.h"
#include "../../Common/LogSinkFs/SinkFs.h"
#include "../../Common/ThreadForker/ThreadForker.h"
#include "../../Process/LogFs_wrap/LogFs_wrap.h"
#include "../../Common/Stream/stream_io_.h"

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
        static void autonomous_logger(int threadNum);// have to be static, to be taken the address of.
        static void singleton_logger(int threadNum);
        static void interface_logFromMultipleStrings(int threadNum);
        static int logFromMultipleStrings(int howMany_params, ...);
        int FindMax (int n, ...);// not static, since I don't need to point at it, by a function pointer.

};



#endif // TEST_UNIT_LOGGER_H
