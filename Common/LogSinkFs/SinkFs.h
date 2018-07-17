
#include <fstream>
#include <string>
#include <stack>
#include <mutex>
#include <sys/stat.h>
#include <sstream> // NB. specific include for string-stream
#include <ctime>

using namespace std;

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


    class SinkFs
    {
        private:
            fstream where;// the stream to log on : used for output.
            string fName;
            struct CurrentTag; // forward
            stack<CurrentTag> * tagStack;
            int indentmentLevel;
            bool hasPermissionsToWrite;
            int semaphore;
            int verbosity;
            string constructorException;
            string operationException;
            struct CurrentTag
            {// this struct represents the generic tracing-tag, which is held in a stack.
                public:
                    string tagName;
                    int sectionVerbosity;
                    CurrentTag(): tagName("unspecified"), sectionVerbosity(0){}// a default Ctor.
                    // Ctor with initialization:
                    CurrentTag(const string &par_tagName, int par_sectionVerbosity): tagName(par_tagName), sectionVerbosity(par_sectionVerbosity)
                    {}// end Ctor
            };// end struct
            //
            // follow some private methods.
            inline bool FileExists_test( const std::string& name);// private
            bool tryOpen(); // private
            void indent(); // private
            SinkFs(const SinkFs &other){}// private: no usage of copy Ctor: it's a Singleton.
            SinkFs & operator=(const SinkFs &other);// private: no usage of copy Ctor: it's a Singleton.

        public:
            SinkFs();// Ctor //public SinkFs()
            ~SinkFs();// Dtor
            string &GetCurrentLogName();
            void SectionOpen(const string &curTag, int sectionVerbosity);
            void SectionTrace(const string &what, int sectionVerbosity);
            void SectionClose();
    };// end class SinkFs


}// end namespace LogSinkFs_Library
