#include "Test_Unit_Logger.h"

Test_Unit_Logger::Test_Unit_Logger()
{
    //ctor
}

Test_Unit_Logger::~Test_Unit_Logger()
{
    //dtor
}


void Test_Unit_Logger::autonomous_logger(int threadNum)
{// a negative threadNum param means the call has been spawn from the main thread.
    Common::SinkFs log;
    //
    log.SectionOpen("autonomous_logger", 0);
    for(int c=0; c<10; c++)
    {
        std::string threadLabel("from inside autonomous_logger:: thread number ");
        std::string * converter = Common::StrManipul::intToString( threadNum);
        threadLabel += *converter;
        delete converter;
        log.SectionTrace( threadLabel, 0);
    }
    log.SectionClose();
}// autonomous_logger


void Test_Unit_Logger::singleton_logger(int threadNum)
{// a negative threadNum param means the call has been spawn from the main thread.
    // no log instance -> Singleton
    Common::LogWrappers::SectionOpen("singleton_logger", 0);
    for(int c=0; c<10; c++)
    {
        std::string threadLabel("from inside singleton_logger:: thread number ");
        std::string * converter = Common::StrManipul::intToString( threadNum);
        threadLabel += *converter;
        delete converter;
        Common::LogWrappers::SectionContent( threadLabel.c_str() , 0);
    }
    Common::LogWrappers::SectionClose();
}// singleton_logger



void Test_Unit_Logger::managementMethod()
{
    // the following test logs from ...
    autonomous_logger( -1);// from main thread, without spawning new threads
    singleton_logger( -1);// from main thread, without spawning new threads
    //
    // the following test logs from an automatic(i.e. Stack) instance of Common::SinkFs, forking new threads
    Common::FuncPtr autonomousLogger_funcPtr;
    autonomousLogger_funcPtr = (void (*)(int))(&this->autonomous_logger);// take the address of a member function
    Common::ThreadForker threadForker( autonomousLogger_funcPtr, 8);
    threadForker.theForkingPoint();// forking new threads
    // the following test logs from a Singleton<Common::SinkFs>, forking new threads
    Common::FuncPtr  singletonLogger_funcPtr;
    singletonLogger_funcPtr = (void (*)(int))(&this->singleton_logger);// take the address of a member function
    Common::ThreadForker singleton_threadForker( singletonLogger_funcPtr, 8);
    singleton_threadForker.theForkingPoint();// forking new threads
}// managementMethod
