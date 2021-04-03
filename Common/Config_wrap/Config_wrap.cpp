
#include "Config_wrap.h"


// get "LogSinkFsConfig.txt"
Common::ConfigurationService * Process::getConfigurationOfLogSinkFs()
{
    Common::ConfigurationService * cs = new Common::ConfigurationService( "./LogSinkFsConfig.txt");
    if( cs->readIsHealtlyConstructed() == false)
    {
        delete cs;// the Construction was invalid.
        cs = NULL;
    }// else cs is valid.
    return cs;
}// END // get "LogSinkFsConfig.txt"


// get "./AppConfig.txt"
Common::ConfigurationService * Process::getDefaultConfiguration()// i.e. "./AppConfig.txt"
{
    Common::ConfigurationService * cs = new Common::ConfigurationService( "./AppConfig.txt");
    if( cs->readIsHealtlyConstructed() == false)
    {
        delete cs;// the Construction was invalid.
        cs = NULL;
    }// else cs is valid.
    return cs;
}// END // get "./AppConfig.txt"

// get // a custom config file.
Common::ConfigurationService * Process::getNamedConfiguration( const std::string &NamedConfigFullPath )// a custom config file.
{
    Common::ConfigurationService * cs = new Common::ConfigurationService( NamedConfigFullPath);
    if( cs->readIsHealtlyConstructed() == false)
    {
        delete cs;// the Construction was invalid.
        cs = NULL;
    }// else cs is valid.
    return cs;
}// // get // a custom config file.
