#ifndef LOGSINKFSWRAPPERS_H_INCLUDED
#define LOGSINKFSWRAPPERS_H_INCLUDED

#include <vector>
#include <string>

namespace Common
{

    namespace LogWrappers
    {

        void JustConstructTEST( const char * name, int sectionVerbosity);
        void EndOfApplication_MANDATORY_();// make_destruction of a static class , i.e. the Singleton<LogDinkFs>
        //
        void SectionOpen( const char * name, int sectionVerbosity);
        void SectionContent( const char * content, int sectionVerbosity);
        void SectionClose();// no name, since it's in the stack.
        //
        void SectionOpen_multiMessage(std::vector<std::string> &messageArray, int sectionVerbosity);
        void SectionContent_multiMessage(std::vector<std::string> &messageArray, int sectionVerbosity);
        // the Close is Always atomic, since it takes no parameters.
        //
        void SectionContent_variable_name_value(
                std::string &variable_name
                ,unsigned long long variable_value
                ,int sectionVerbosity         );
        void SectionContent_variable_name_value(
                const char *variable_name
                ,unsigned long long variable_value
                ,int sectionVerbosity         );
        // the signed versions:
        void SectionContent_variable_name_value(
                std::string &variable_name
                , long long variable_value
                ,int sectionVerbosity         );
        void SectionContent_variable_name_value(
                const char *variable_name
                , long long variable_value
                ,int sectionVerbosity         );

    }// END namespace LogWrappers
}// END namespace Common

#endif // LOGSINKFSWRAPPERS_H_INCLUDED
