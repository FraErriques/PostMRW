#ifndef CONFIG_PROCESS_H_INCLUDED
#define CONFIG_PROCESS_H_INCLUDED

#include "../../Common/ConfigurationService/ConfigurationService.h"

namespace Process
{

Common::ConfigurationService * getConfigurationOfLogSinkFs();  // get "LogSinkFsConfig.txt"
Common::ConfigurationService * getDefaultConfiguration();  // get "./AppConfig.txt"
Common::ConfigurationService * getNamedConfiguration( const std::string &NamedConfigFullPath ); // get // a custom config file.

}

#endif // CONFIG_PROCESS_H_INCLUDED
