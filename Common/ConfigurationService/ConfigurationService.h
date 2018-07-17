#ifndef CONFIGURATIONSERVICE_H_INCLUDED
#define CONFIGURATIONSERVICE_H_INCLUDED

#include <map>
#include <vector>
#include <string>
#include <fstream>

namespace Common
{

/*
    A Class which manages the configuration files.
    The standard version is supposed to be:  AppConfig.txt in the home dir of the App.
    Such version can be read invoking the default Ctor, which takes no parameters.
    Other configuration files can be written and used with the Ctor which takes a fullPath parameter.
    The parsing is based on token separation, by means of {blank, TAB, '\r', '\n'}.
    Each of these characters delimits a token.
    Tokens are of two kind: {key, value}. Thei correspondence is one-to-one.
    A delimiter is needed, to separate the key-section from the value-section; such delimiter
    is chosen to be the '#' char. That char is so reserved for the purpose, and can not be used for any other purpose.
    The order of the keys must strictly respect that of the values.
    Keys and values having different cardinalities, results in a ill configFile, which stops the service.
    Keys and values having different order, results in a ill configFile, which lets the service work anyway, but in
    a wrong fashion, which is worse.
    Follows a simple example of how the service could be used:
    key_1 key_2 key_3 # val_1 val_2 val_3
    this is the same as
    key_1
    key_2
    key_3
    #
    val_1
    val_2
    val_3
    which is the same as
    key_1     key_2     key_3 # val_1

     val_2
          val_3
    Hope it's useful.
*/



    class ConfigurationService
    {
    private:
        std::fstream theInStream;// i.e. the ConfigFile eg. AppConfig.txt in the home dir of the App.
        std::map<std::string, std::string> * key_val_map;
        std::vector<std::string> * key_all_together;
        std::string whyNotHealtlyConstructed;
        std::string whyNotAvailableKeyVal;
        bool isConstructorStillAlive;// the Constructors( both) use it for the onion-peeling technique. NB. a Ctor cannot return from inside a try block.
        bool isHealtlyConstructed;// methods check it, after construction; it says if the service is constructed or aborted.
        // priv. methods
        ConfigurationService(const ConfigurationService &other);
        ConfigurationService &operator=(const ConfigurationService &other);
        inline bool FileExists_test( const std::string& name) const;


    public:
        // NB. no empty Ctor provided:
        /// NB. : feed with config_path : "default" stands for "./AppConfig.txt" ; whatever else for specific configs;
        /// NB. on Unix the process has allowance to write only in its working dir (i.e. "./" ).
        ConfigurationService( const std::string &fullPath) ;
        ~ConfigurationService();
        std::string * getValue( const std::string &key); // cannot be const, since it writes to this->whyNotAvailableKeyVal.
        std::vector<std::string> * getAllKeys() const;
        std::vector<std::string> * showInstanceHealtCondition() const;
        bool readIsHealtlyConstructed() const;
    };

}// END namespace Common

#endif // CONFIGURATIONSERVICE_H_INCLUDED
