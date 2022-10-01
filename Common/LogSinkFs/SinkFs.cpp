#include "SinkFs.h"
#include "../StringBuilder/StringBuilder.h"
#include "../ConfigurationService/ConfigurationService.h"
#include <cstdlib>
#include <iostream>
#include <exception>      // std::exception

std::mutex SinkFs_mtx;   // NB. mutex for critical section has to be global to compilation unit.

namespace Common
{

    /// <summary>
    /// Sink FileSys Stream; quick_access: Open-Write-Close. No handle hung.
    /// Log rotation: one file per day.
    /// NB. thread safety is granted only tracing (i.e. calling method SectionTrace )
    /// If a program is known to actively spawn new threads, it should be instrumented
    /// only with: SectionTrace
    /// {SectionOpen, SectionClose} are valid tools to visually monitor the stack state(i.e. its depth)
    /// but there's no way to guarantee that an incoming call to the SectionClose method, really comes
    /// from the same thread which pushed the last opening-tag on the tag-stack.
    /// So, in a multithreaded context avoid the couple {SectionOpen, SectionClose}.
    /// Such couple also has the vulnerability of unbalanced instrumentation: if a caller method
    /// has a SectionOpen without a SectionClose or vice-versa, the stack-depth vision will
    /// always be incorrect.
    /// The most robust kind of usage is : just SectionTrace.
    /// </summary>



string & SinkFs::GetCurrentLogName() //public string GetCurrentLogName()
{
    return this->fName;
}//


inline bool SinkFs::FileExists_test( const std::string& name)
{
    struct stat buffer;// local variable, whose address is taken to get an output parameter.
    int result = stat( name.c_str(), &buffer);// NB. buffer contains a lot of interesting fields after the call.
    bool FileExists = (result==0);
    return FileExists;
}


// private: no usage of copy Ctor: it's a Singleton.
SinkFs & SinkFs::operator=(const SinkFs &other)
{
    SinkFs * invalidPtr = nullptr;
    return *invalidPtr;
}


/// <summary>
/// Constructor // Ctor //public SinkFs()
/// </summary>
SinkFs::SinkFs()
{
    SinkFs_mtx.lock();// critical section : the lock avoids multiple file-creations.
    {// the lock avoids multiple file-creations
        Common::ConfigurationService * configFileManager = nullptr;
        std::string * logFullPathPrefix_config = nullptr;
        std::string * semaphore_config = nullptr;
        std::string * verbosity_config = nullptr;
        try// this should happen due to interrupts during the execution of the current lock-block content
        {
            this->tagStack = nullptr;// init
            this->indentmentLevel = 0;
            this->hasPermissionsToWrite = false;
            this->semaphore = 0;// red.
            this->verbosity = 0;// full.
            this->constructorException = "not yet initialized";
            this->operationException = "not yet initialized";
            //####
            bool isConstructorStillAlive=true;
            std::string reasonForNotBeingAlive;
            //  first check : check Configuration existence.
            if(isConstructorStillAlive)
            {// check Configuration existence
                const std::string ConfigurationFullPath("./LogSinkFsConfig.txt");
                bool ConfigurationFilePresent = FileExists_test( ConfigurationFullPath);
                if( ! ConfigurationFilePresent)  // i.e. if Config file not present -> create an HOWTO file for the user in the AppDomain curDir.
                {
                    std::fstream LogConfig_HOWTO;
                    LogConfig_HOWTO.open("./LogConfig_HOWTO.txt", std::ios::out);
                    LogConfig_HOWTO << "Please create a file called LogSinkFsConfig.txt just in the directory where you find the current file. Then fill it as follows:\n";
                    LogConfig_HOWTO << "key_logFullPathPrefix PARAGRAPH NAME (the first three rows are an index of the contents, they work as a memo)\n";
                    LogConfig_HOWTO << "key_semaphore PARAGRAPH NAME (paragraph names and their order is compulsive) \n";
                    LogConfig_HOWTO << "key_verbosity PARAGRAPH NAME NB the delimiter compulsory char is one between {blank, TAB, '\r', '\n' .";
                    LogConfig_HOWTO << "logFullPathPrefix ACTUAL VALUE (the second three rows contain the actual contents of the paragraphs announced in the index)\n";
                    LogConfig_HOWTO << "semaphore ACTUAL VALUE (the order of the contents must respect the order in the index)\n";
                    LogConfig_HOWTO << "verbosity ACTUAL VALUE n in [0..99] means levels from current verbosity up will be written: if I set verbosity==3, messages of level=2 will not be written\n";
                    LogConfig_HOWTO << std::endl;// this flushes
                    LogConfig_HOWTO.close();
                    std::cout<<"missing configuration file in  the AppDomain curDir; look for LogConfig_HOWTO.txt";
                    isConstructorStillAlive = false;
                    reasonForNotBeingAlive = "configuration file called LogSinkFsConfig.txt is missing in the home directory.";
                }// else can continue.
                //##
                if(isConstructorStillAlive)
                {// read configuration
                    configFileManager = new Common::ConfigurationService( ConfigurationFullPath);// NB. library ConfigurationService
                    std::vector<std::string> * allKeys = configFileManager->getAllKeys();
                    logFullPathPrefix_config = configFileManager->getValue( "key_logFullPathPrefix");
                    semaphore_config = configFileManager->getValue( "key_semaphore");
                    verbosity_config = configFileManager->getValue( "key_verbosity");
                    // check Configuration consistency
                    int rowCardinality = allKeys->size()*2;// ==n_key+n_value skipping the separator '#'. allKeys is n_key.
                    if( 6 != rowCardinality
                        || false == configFileManager->readIsHealtlyConstructed()
                      )
                    {// 3 keys, the separator '#', three values.
                        std::cout<<"the Configuration File is inadequate. Each block must be of three rows. The blocks are two:{index, paragraphs}.\n";
                        isConstructorStillAlive=false;
                        reasonForNotBeingAlive = "Configuration consistency";
                    }// else can continue.
                    //##
                    if(isConstructorStillAlive)
                    {// check Configuration::semaphore
                        std::string configurationChecker("on");
                        if( configurationChecker.compare(*semaphore_config)==0)
                        {
                            this->semaphore = 1;// green semaphore
                        }
                        else // "off" or any other word different from "on"
                        {
                            this->semaphore = 0;// red semaphore
                            std::cout<<"the semaphore is red->exit without opening the log. Any value different from ""on"" sets red semaphore.\n";
                            isConstructorStillAlive=false;
                            reasonForNotBeingAlive = "Configuration::semaphore";
                        }// end semaphore.
                        //##
                        if(isConstructorStillAlive)
                        {// check Configuration::verbosity
                            // if it gets here, we have green semaphore : go on:
                            // NB. use of stringstream to convert from string to integer.
                            std::stringstream(*verbosity_config) >> this->verbosity;// C# int.Parse(verbosity);
                            // end configuration acquisition. Implementation starts:
                            if( 0 > this->verbosity // verbosity has to be in [0, 99]
                            || 99 < this->verbosity )
                            {
                                std::cout<<"Configuration::verbosity set to "+ *verbosity_config +"  Values outside [0, 99] are illegal.\n";
                                isConstructorStillAlive=false;
                                reasonForNotBeingAlive = "Configuration::verbosity";
                            }// else can continue.
                            //##
                            if(isConstructorStillAlive)
                            {// check tryOpen
                                this->tagStack = new std::stack<CurrentTag>();// prepare tag-stack
                                // prepare log-stream
                                // build desinenza dinamica per log rotation.
                                time_t now = time(0);// current date/time based on OperSys.
                                char * dynamicEnding = ctime(&now);// convert Operating Sys date to string form.
                                std::string LogFsName(dynamicEnding);
                                std::string dateStamp(LogFsName.substr(0,11) + LogFsName.substr(20,4) + "_" );// size()==11+4+1==16// this string is the one to replace in.
                                std::string timeStamp(LogFsName.substr(11,8) );
                                // some necessary replacements, to let the filename legal, within the Operating Sys syntax rules.
                                std::string * progressiveReplacement = nullptr;
                                progressiveReplacement = StrManipul::replaceAllOccurencesOf( " ", dateStamp, "_" );
                                std::string * tmp = progressiveReplacement;
                                progressiveReplacement = StrManipul::replaceAllOccurencesOf( ":", *progressiveReplacement, "#" );
                                delete tmp;
                                tmp = progressiveReplacement;
                                progressiveReplacement = StrManipul::replaceAllOccurencesOf( ";", *progressiveReplacement, "#" );
                                delete tmp;
                                tmp = progressiveReplacement;
                                progressiveReplacement = StrManipul::replaceAllOccurencesOf( "\n", *progressiveReplacement, "_" );
                                delete tmp;
                                tmp = progressiveReplacement;
                                progressiveReplacement = StrManipul::replaceAllOccurencesOf( "\r", *progressiveReplacement, "_" );
                                delete tmp;
                                // Now the datetime string is appropriate, but we have to cut off the time portion, since the logFile is daily
                                // and so it needs only the date. The time will be used after, inside the Log.
                                StringBuilder sb( 500);// 500 bytes of extimated lenght for the LogFile path.
                                sb.append( *logFullPathPrefix_config);
                                sb.append( std::string("_LogStream_") );
                                sb.append( *progressiveReplacement);
                                sb.append( std::string(".log") );
                                this->fName.assign( sb.str()); // TEST = sb.str();// get the whole package.
                                delete progressiveReplacement;
                                // NB. tryOpen here: on fail the exception will be caught a few rows below here.
                                bool has_tryOpen_succeded = tryOpen();// on success sets this->hasPermissionToWrite=true. the caller of tryOpen has to catch and to close the stream.
                                this->where << "\r\n S=start, C=content, K=close -=separator \t"<< timeStamp <<"\t\r\n" << std::endl;// NB. ::endl flushes the buffer.
                                this->where.close();// already flushed by ::endl. NB. always close after tryOpen , from all the class methods.
                                if( ! has_tryOpen_succeded)
                                {
                                    std::cout<<"Trying to open the LogFile gave a failure: write permissions missing; check to be trying to Log ONLY in your home dir.\n";
                                    isConstructorStillAlive=false;
                                    reasonForNotBeingAlive = "tryOpen failed: write permissions missing; check to be trying to Log ONLY in your home dir.";
                                }
                                delete configFileManager;
                            }// END check tryOpen
                        }// END check check Configuration::verbosity
                    }// END check Configuration::semaphore
                }// END check Configuration consistency
            }// END check Configuration existence
            delete logFullPathPrefix_config;
            delete semaphore_config;
            delete verbosity_config;
        }// end try
        catch (...)
        {// if catch -> hasPermissionsToWrite=false
            this->constructorException = "failed to construct LogSinkFs";
            this->hasPermissionsToWrite = false;
            delete configFileManager;
            delete logFullPathPrefix_config;
            delete semaphore_config;
            delete verbosity_config;
        }// end catch.
    }// end lock
    SinkFs_mtx.unlock();// END critical section : the lock avoids multiple file-creations.
}//end constructor





/// <summary>
// tries to open and does not catch on error : the caller must catch and close the handle after usage.
// this is true in all the class. tryOpen is a private method to let the istance-wide stream-handle open.
// the caller must always catch and close the handle after usage.
// please do not nest multiple try and neither multiple locks.
/// </summary>
bool SinkFs::tryOpen() // private
{
    bool has_tryOpen_succeded = false;
    bool todayLogAlreadyPresent = FileExists_test( this->fName);
    if( todayLogAlreadyPresent)//if ( File.Exists(fName)) // file exists -> Append
    { // Append to already existing log
        this->where.open( this->fName.c_str() , std::ios::app ); // Append to already existing log
    }
    else // Create today's log, which is not yet present
    {
        this->where.open( this->fName.c_str() , std::ios::out ); // Create today's log, which is not yet present
    }// end selection of opening mode, between {Append, Create}.
    if( this->where.is_open() )// check about opening success:
    {
        this->hasPermissionsToWrite = true;
        has_tryOpen_succeded = true;
    }
    else
    {
        this->hasPermissionsToWrite = false;
        has_tryOpen_succeded = false;
    }// END check about opening success:
    return has_tryOpen_succeded;
}// end tryOpen





/// <summary>
/// technical private method.
/// </summary>
void SinkFs::indent()
{// NB. do not lock here! it creates a deadlock, since it's called from inside the three{open, trace, close} which lock.
    if( ! hasPermissionsToWrite)
    {// without write-permission, silently skip.
        return;// this->semaphore is red, so no writing.
    }//else can continue.
    // can write:
    for (int c = 0; c < indentmentLevel; c++)
    {
        // C# where.Write("   ");// three blanks==1 indentation.
        where << "   "; // three blanks==1 indentation.
    }
}// end indent



/// <summary>
/// open the tag
/// </summary>
/// <param name="curTag"></param>
/// <param name="sectionVerbosity"></param>
void SinkFs::SectionOpen(const string &curTag, int sectionVerbosity)
{
    if( ! hasPermissionsToWrite)
    {// without write-permission, silently skip.
        return;// this->semaphore is red, so no writing.
    }//else can continue.
    // can write:
    // START critical section.
    SinkFs_mtx.lock();// critical section.
    {// the lock avoids multiple file-creations
        try// this should happen due to interrupts during the execution of the current lock-block content
        {
            CurrentTag currentTag( curTag, sectionVerbosity);// Open is compulsory named (i.e. not unspecified).
            if( nullptr != this->tagStack)
            {
                // push the Tag anyway on the stack. It is necessary even if it's below this.verbosity.
                // when closing the section, the verbosity will be checked to decide whether to write.
                // C# currentTag = new CurrentTag(curTag, sectionVerbosity);
                this->tagStack->push(currentTag);// NB. cruciale ! #############
            }// else skip pushing, when no stack has been prepared.
            if(sectionVerbosity < this->verbosity)
            {// maximum verbosity leves is zero. Higher verbosity-levels prune the lower-level messages.
                SinkFs_mtx.unlock();// END critical section.
                return;// the verbosity of CurrentSection is not adequate for writing.
            }//else can continue.
            tryOpen();// quick_access connection -> close at once and reopen when required.
            ++indentmentLevel;
            where << "-\nS";
            indent();
            // C# where.WriteLine(((CurrentTag)this.tagStack.Peek()).tagName + "_______start");
            where << ((CurrentTag)this->tagStack->top()).tagName + "_______start" << "\n";// Openig decoration.
            // NB. this->tagStack->pop();//NB. DO NOT remove onOpen: it's needed until Close.
            // NB. top() only reads the top of the stack; pop() actually picks up such element(i.e. the stack top).
            where.flush();
            ++indentmentLevel;
            where.close();// quick_access connection -> close at once and reopen when required.
        }// end try
        catch(...)
        {
            this->operationException = "TODO : write trouble text from SectionOpen";
        }
    }// end lock
    SinkFs_mtx.unlock();// END critical section.
}//end SectionOpen



/// <summary>
/// trace content, from inside a tag
/// </summary>
/// <param name="what"></param>
/// <param name="sectionVerbosity"></param>
void SinkFs::SectionTrace( const string &what, int sectionVerbosity)
{
    if(sectionVerbosity < this->verbosity)
    {
        return;// the verbosity of CurrentSection is not adequate for writing.
    }//else can continue.
    // maximum verbosity level is zero. Higher verbosity-levels prune the lower-level messages.
    if( ! hasPermissionsToWrite)
    {// without write-permission, silently skip.
        return;// this->semaphore is red, so no writing.
    }//else can continue.
    // START critical section.
    SinkFs_mtx.lock();// critical section.
    {// the lock avoids multiple file-creations
        try// this should happen due to interrupts during the execution of the current lock-block content
        {
            CurrentTag currentTag;// just book it : init in the following rows, by deciding
            // if currentSection belongs to a named one in the stack, or if it's an unspecified:
            if( nullptr != this->tagStack
                && 0 < this->tagStack->size() )
            {// peek current section, to give a signature to the tracing.
                currentTag = ((CurrentTag)this->tagStack->top());// read the stack-top.
                // NB. DON'T ! this->tagStack->pop();// DON'T remove the stack-top: we need it for SectionClose().
            }// else e' una chiamata alla sola SectionTrace, non avvolta fra SectionOpen e SectionClose.
            // quindi persa obbligatorieta' della decorazione. Default Ctor was called on declaration of
            // currentTag.tagName which will be "unspecified".
            tryOpen();// quick_access connection -> close at once and reopen when required.
            where << "C  ";
            indent();
            time_t now = time(0);// current date/time based on Oper-Sys.
            char * dynamicEnding = ctime(&now);// convert Operating Sys date to string form.
            std::string LogFsName(dynamicEnding);
            std::string dateStamp(LogFsName.substr(0,11) + LogFsName.substr(20,4) + " " );
            std::string timeStamp(LogFsName.substr(11,8) );
            std::string traceStamp = dateStamp + timeStamp;
            // write on Log:
            where << "inside " + currentTag.tagName + ": " + traceStamp + " " + what + '\n';
            where.flush();
            where.close();// quick_access connection -> close at once and re-open when required.
        }// end try
        catch(...)
        {
            this->operationException = "TODO : write trouble text from SectionTrace";
        }
    }// end lock
    SinkFs_mtx.unlock();// END critical section.
}// end SectionTrace method


/// <summary>
/// close the tag that is on the stack top.
/// </summary>
void SinkFs::SectionClose()
{
    if( ! hasPermissionsToWrite)
    {// without write-permission, silently skip.
        return;// this->semaphore is red, so no writing.
    }//else can continue.
    SinkFs_mtx.lock();// critical section.
    {// the lock avoids multiple file-creations
        try// this should happen due to interrupts during the execution of the current lock-block content
        {
            CurrentTag currentTag;
            if (nullptr != this->tagStack )
            {
                if( 0 < this->tagStack->size() )
                {
                    currentTag = ((CurrentTag)this->tagStack->top() );// get the section. It will be written down only if the section verbosity is above this.verbosity.
                    this->tagStack->pop();// remove the closing section from the stack.
                }// do not operate on an empty stack.
            }// do not operate on an unprepared stack.
            // if(sectionVerbosity < this->verbosity)
            // NB. there's no section verbosity in a close. It necessarily has the same verbosity of it omologous open.
            // maximum verbosity leves is zero. Higher verbosity-levels prune the lower-level messages.
            // if got here -> can write:
            --indentmentLevel;
            tryOpen();// quick_access connection -> close at once and reopen when required.
            //where.Write("^");
            where <<"K"; //K=close
            indent();
            // C# where.WriteLine("closing____" + currentTag.tagName + "_______end");
            where<< "closing____" + currentTag.tagName + "_______end" << '\n';
            where.flush();
            where.close();// quick_access connection -> close at once and reopen when required.
            --indentmentLevel;
        }// end try
        catch(...)
        {
            this->operationException = "TODO : except inside SectionClose";
        }
    }// end lock
    SinkFs_mtx.unlock();// END critical section.
}// end SectionClose method


// ex void SinkFs::Dispose()
SinkFs::~SinkFs()
{
    SinkFs_mtx.lock();// critical section.
    {// the lock avoids thread collisions on Destruction.
        try
        {
            if( where.is_open())
            {
                where.flush();// NB. crucial !
                where.close();
            }// endif if(where.open()) : already closed.
            if( nullptr != this->tagStack)
            {
                delete this->tagStack;
            }
        }// end try block
        catch(...)
        {
            this->operationException = "TODO : except inside Dispose";
        }
    }// end lock
    SinkFs_mtx.unlock();// END critical section.
}// end Dispose


}// end namespace LogLibrary

/*    cantina ##############################################

*/
