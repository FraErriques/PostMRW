#ifndef CONFIG_PROCESS_H_INCLUDED
#define CONFIG_PROCESS_H_INCLUDED

#include "../../Common/ConfigurationService/ConfigurationService.h"

namespace Process{
namespace Configuration{

Common::ConfigurationService * getConfigurationOfLogSinkFs();  // get "LogSinkFsConfig.txt"
Common::ConfigurationService * getDefaultConfiguration();  // get "./AppConfig.txt"
Common::ConfigurationService * getNamedConfiguration( const std::string &NamedConfigFullPath ); // get // a custom config file.

}// nmsp Configuration
}// nmsp Process

#endif // CONFIG_PROCESS_H_INCLUDED
