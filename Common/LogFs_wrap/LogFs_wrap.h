#ifndef LOGSINKFSWRAPPERS_H_INCLUDED
#define LOGSINKFSWRAPPERS_H_INCLUDED

#include <vector>
#include <string>

namespace Common
{

    namespace LogWrappers
    {

        void JustConstructTEST( const char * name, int sectionVerbosity);
        //
        void SectionOpen( const char * name, int sectionVerbosity);
        void SectionContent( const char * content, int sectionVerbosity);
        void SectionClose();// no name, since it's in the stack.
        //
        void SectionOpen_multiMessage(std::vector<std::string> &messageArray, int sectionVerbosity);
        void SectionContent_multiMessage(std::vector<std::string> &messageArray, int sectionVerbosity);
        // the Close is Always atomic, since it takes no parameters.

    }// END namespace LogWrappers
}// END namespace Common

#endif // LOGSINKFSWRAPPERS_H_INCLUDED
