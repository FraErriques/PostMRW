#include <sstream>                      // NB. specific include for string-stream
#include <thread>                       // std::thread
#include <mutex>                        // std::mutex
#include <stdarg.h>                     // va_list, va_start, va_arg, va_end
#include "LogFs_wrap.h"                 // here std::<vector> gets included.
#include "../../Common/TSingleton/Singleton.h"    // here mutexes get included.
#include "../../Common/LogSinkFs/SinkFs.h"
#include "../../Common/Stream/stream_io_.h"
#include "../../Common/StringBuilder/StringBuilder.h"

namespace Process{
namespace LogWrappers{


// if no ConfigFile is present in the current directory( of the caller process) a Config_HOWTO_ is dumped
// in the current directory( of the caller process). It requires to produce a Config in the curDir
// and explains how to write it.
// the path to log to has to be relative to the curDir
// NB. on Unix there's no allowance to write outside the subtree of the process'home dir.
void JustConstructTEST( const char * name, int sectionVerbosity)
{// requiring an instance, without tracing anything, produces the logFile on disk.
    Common::TSingleton<Common::SinkFs>::instance();
}// it appends if the daily stream is already in place, while creates it if it's not there.

void EndOfApplication_MANDATORY_()
{// make_destruction of a static class , i.e. the Singleton<LogSinkFs>
    Common::TSingleton<Common::SinkFs>::unsubscribe_all_();
}// make_destruction of a static class , i.e. the Singleton<LogDinkFs>

void SectionOpen( const char * name, int sectionVerbosity)
{
    Common::TSingleton<Common::SinkFs>::instance()->SectionOpen(name, sectionVerbosity);
}


void SectionContent( const char * content, int sectionVerbosity)
{// here the param is called "content" instead of "name", since it's not a section name, but rather its content.
    Common::TSingleton<Common::SinkFs>::instance()->SectionTrace(content, sectionVerbosity);
}




 void SectionClose( )// no name, since it's in the stack.
{// pop the section name from the stack, and close such element.
    Common::TSingleton<Common::SinkFs>::instance()->SectionClose();
}

//////////////////////////


// mutex for critical section has to be compilation-unit global.
std::mutex SectionOpen_multiMessage_instance_mtx;// mutex for critical section has to be compilation-unit global.

void SectionOpen_multiMessage(std::vector<std::string> &messageArray, int sectionVerbosity)
{
    SectionOpen_multiMessage_instance_mtx.lock();// START critical section.
    {
        int cardMessages = messageArray.size();
        Common::StringBuilder openTracer( 100 * cardMessages);// an extimate.
        for( std::vector<std::string>::iterator curMsg=messageArray.begin();
             curMsg != messageArray.end();
             curMsg++
           )
           {
               openTracer.append( *curMsg);
               openTracer.append( " ");// some separation
           }
        SectionOpen( openTracer.str().c_str() , sectionVerbosity);// trace the whole.
    }
    SectionOpen_multiMessage_instance_mtx.unlock();// END critical section.
}// END SectionOpen_multiMessage

// mutex for critical section has to be compilation-unit global.
std::mutex SectionContent_multiMessage_instance_mtx;// mutex for critical section has to be compilation-unit global.

void SectionContent_multiMessage(std::vector<std::string> &messageArray, int sectionVerbosity)
{
    SectionContent_multiMessage_instance_mtx.lock();// START critical section.
    {
        int cardMessages = messageArray.size();
        Common::StringBuilder contentTracer( 100 * cardMessages);// an extimate.
        for( std::vector<std::string>::iterator curMsg=messageArray.begin();
             curMsg != messageArray.end();
             curMsg++
           )
           {
               contentTracer.append( *curMsg);
               contentTracer.append( " ");// some separation
           }
        SectionContent( contentTracer.str().c_str() , sectionVerbosity);// trace the whole.
    }
    SectionContent_multiMessage_instance_mtx.unlock();// END critical section.
}// END SectionContent_multiMessage

void SectionContent_variable_name_value(
        std::string &variable_name
        ,unsigned long long variable_value
        ,int sectionVerbosity         )
{
    SectionContent_multiMessage_instance_mtx.lock();// START critical section.
    {
        // twenty is an estimate of length for a string containing a long-long.
        Common::StringBuilder contentTracer( variable_name.length() + 20);
        contentTracer.append(variable_name);
        contentTracer.append("  ");//some separation.
        contentTracer.append( variable_value);
        SectionContent( contentTracer.str().c_str() , sectionVerbosity);// trace the whole.
    }
    SectionContent_multiMessage_instance_mtx.unlock();// END critical section.
}// END SectionContent_variable_name_value

        void SectionContent_variable_name_value(
                const char *variable_name
                ,unsigned long long variable_value
                ,int sectionVerbosity         )
{
    std::string var_name( variable_name);
    SectionContent_variable_name_value( var_name
                                       ,variable_value
                                       ,sectionVerbosity  );
}// SectionContent_variable_name_value

        // the signed versions:
        void SectionContent_variable_name_value(
                std::string &variable_name
                , long long variable_value
                ,int sectionVerbosity         )
{
    SectionContent_multiMessage_instance_mtx.lock();// START critical section.
    {
        // twenty is an estimate of length for a string containing a long-long.
        Common::StringBuilder contentTracer( variable_name.length() + 20);
        contentTracer.append(variable_name);
        contentTracer.append("  ");//some separation.
        contentTracer.append( variable_value);
        SectionContent( contentTracer.str().c_str() , sectionVerbosity);// trace the whole.
    }
    SectionContent_multiMessage_instance_mtx.unlock();// END critical section.
}// END SectionContent_variable_name_value


void SectionContent_variable_name_value(
    const char *variable_name
    , long long variable_value
    ,int sectionVerbosity         )
{
    std::string var_name( variable_name);
    SectionContent_variable_name_value( var_name
    ,variable_value
    ,sectionVerbosity  );
}


// TODO goes in Process::Log_Wrappers
// take std::string* and delete --------------------(V)
// use a StringBuilder to concatenate the input ----(V)
void SectionContent_fromMultipleStrings(int sectionVerbosity, int howMany_params, ...)
{// takes an unspecified number of std::string and concatenates them, with  blank in between.
    int loopCounter;
    std::string * val;// the choosen data-type; string* generated by conversion of vars to be logged.
    va_list v_list;// va_list is the data-type of the unspecified parameters
    va_start( v_list, howMany_params);// va_start is the macro to open the set.
    Common::StringBuilder sb( 100 * howMany_params);// estimate 100 chars per param; auto.
    // DBG sb.append( std::string("composition state : "));
    for( loopCounter=0; loopCounter<howMany_params; loopCounter++)
    {// the macro va_arg can be queried from zero to howMany_params-1.
        val = va_arg( v_list, std::string* );// get the current one of the params, specifying the common data-type.
        sb.append( *val);// use the StringBuilder to append
        sb.append( std::string("  "));// insulate params.
        delete val;// delete the original heap-parameter.
    }
    va_end( v_list);// macro to close the set.
    //
    SectionContent( sb.str().c_str() , sectionVerbosity);// trace the whole.
    // DBG std::cout<< "\n\t composition state : "<< sb.str(); // DBG
    // ready.
}// logFromMultipleStrings


}// end namespace LogWrappers
}// end nmsp Process

/*
namespace testLog
{

std::mutex open_phase;
std::mutex trace_phase_1_;
std::mutex trace_phase_2_;
std::mutex trace_phase_3_;
std::mutex close_phase;

void EntryPointThreadSpawn( int par)
{
    open_phase.lock();
    {
        std::stringstream thread_number_open;
        thread_number_open << par;
        //
        std::vector<std::string> opener;
        opener.push_back("EntryPointThreadSpawn");
        opener.push_back(" thread_number= ");
        opener.push_back(thread_number_open.str());
        Process::LogWrappers::SectionOpen_multiMessage( opener, 0);
        opener.clear();
    }
    open_phase.unlock();
    //
    trace_phase_1_.lock();
    {
        std::stringstream thread_number_t1;
        thread_number_t1 << par;
        //
        std::vector<std::string> contentTracer_1;
        contentTracer_1.push_back("Nel mezzo del cammin di nostra vita");
        contentTracer_1.push_back(" thread_number= ");
        contentTracer_1.push_back(thread_number_t1.str());
        Process::LogWrappers::SectionContent_multiMessage( contentTracer_1 , 0);
        contentTracer_1.clear();
    }
    trace_phase_1_.unlock();
    //
    trace_phase_2_.lock();
    {
        std::stringstream thread_number_t2;
        thread_number_t2 << par;
        //
        std::vector<std::string>  contentTracer_2;
        contentTracer_2.push_back("Mi ritrovai per una selva oscura");
        contentTracer_2.push_back(" thread_number= ");
        contentTracer_2.push_back(thread_number_t2.str());
        Process::LogWrappers::SectionContent_multiMessage( contentTracer_2 , 0);
        contentTracer_2.clear();
    }
    trace_phase_2_.unlock();
    //
    trace_phase_3_.lock();
    {
        std::stringstream thread_number__t3;
        thread_number__t3 << par;
        //
        std::vector<std::string>  contentTracer_3;
        contentTracer_3.push_back("che la diritta via era smarrita");
        contentTracer_3.push_back(" thread_number= ");
        contentTracer_3.push_back(thread_number__t3.str());
        Process::LogWrappers::SectionContent_multiMessage( contentTracer_3 , 0);
        contentTracer_3.clear();
    }
    trace_phase_3_.unlock();
    //
    close_phase.lock();
    {
        Process::LogWrappers::SectionClose();
    }
    close_phase.unlock();
    // ready.
}// END EntryPointThreadSpawn


void EntryPointThreadSpawn_TraceOnly_( int par)
{
    //
    trace_phase_1_.lock();
    {
        std::stringstream thread_number_t1;
        thread_number_t1 << par;
        //
        std::vector<std::string> contentTracer_1;
        contentTracer_1.push_back("Nel mezzo del cammin di nostra vita");
        contentTracer_1.push_back(" thread_number= ");
        contentTracer_1.push_back(thread_number_t1.str());
        Process::LogWrappers::SectionContent_multiMessage( contentTracer_1 , 0);
        contentTracer_1.clear();
    }
    trace_phase_1_.unlock();
    //
    trace_phase_2_.lock();
    {
        std::stringstream thread_number_t2;
        thread_number_t2 << par;
        //
        std::vector<std::string>  contentTracer_2;
        contentTracer_2.push_back("Mi ritrovai per una selva oscura");
        contentTracer_2.push_back(" thread_number= ");
        contentTracer_2.push_back(thread_number_t2.str());
        Process::LogWrappers::SectionContent_multiMessage( contentTracer_2 , 0);
        contentTracer_2.clear();
    }
    trace_phase_2_.unlock();
    //
    trace_phase_3_.lock();
    {
        std::stringstream thread_number__t3;
        thread_number__t3 << par;
        //
        std::vector<std::string>  contentTracer_3;
        contentTracer_3.push_back("che la diritta via era smarrita");
        contentTracer_3.push_back(" thread_number= ");
        contentTracer_3.push_back(thread_number__t3.str());
        Process::LogWrappers::SectionContent_multiMessage( contentTracer_3 , 0);
        contentTracer_3.clear();
    }
    trace_phase_3_.unlock();
    // ready.
}// END EntryPointThreadSpawn_TraceOnly_


bool configFileTestGenerator()
{
    std::fstream theOutStream;
    std::string configFpath("./AppConfig.txt");
    bool opened = true;// TODO
//Common::Stream::outstreamOpener( configFpath, theOutStream); TODO undefined reference
    const int threshold = 125;// has to be odd(dispari).
    const int n_couples_keyVal =(int)(threshold/2);
    for(int c=0; c<threshold; c++)
    {// putline ( const std::string &theLine,  std::fstream &theOutStream )
        if(n_couples_keyVal>c)
        {
            std::stringstream tmp;
            tmp << "key_" << c+1;
            Common::Stream::putline( tmp.str(), theOutStream);
        }
        else if(n_couples_keyVal==c)
        {
            std::stringstream tmp;
            tmp << "#";
            Common::Stream::putline( tmp.str(), theOutStream);
        }
        else // if(n_couples_keyVal<c)
        {
            std::stringstream tmp;
            tmp << "val_" << c-n_couples_keyVal;
            Common::Stream::putline( tmp.str(), theOutStream);
        }// end putline
    }// end for
    opened &= Common::Stream::outstreamCloser(theOutStream);
    return opened;
}// END configFileTestGenerator



int recursionLevel=0;

void logTest_monothread_()
{
    Process::LogWrappers::SectionOpen("void logTest_1()", 0);
    //
    while(25>recursionLevel)
    {
        ++recursionLevel;
        std::stringstream sStream;
        sStream << recursionLevel;
        std::string LogText("this is some content, logged from inside the current function. Recursion== " + sStream.str() );
        Process::LogWrappers::SectionContent( LogText.c_str(), 0);
        logTest_monothread_();
    }
    //
    Process::LogWrappers::SectionClose();
}


}// end nmsp testLog
*/
