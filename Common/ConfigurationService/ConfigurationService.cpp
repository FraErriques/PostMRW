
#include <sys/stat.h> // test file existence.
#include <cmath>
#include <sstream>
#include "ConfigurationService.h"
#include "../StringBuilder/StringBuilder.h"


/*
    A Class which manages the configuration files.
    The standard version is supposed to be:  AppConfig.txt in the home dir of the App.
    Such version can be read invoking the default Ctor, which takes no parameters.
    Other configuration files can be written and used with the Ctor which takes a fullPath parameter.
    The parsing is based on token separation, by means of {blank, TAB, '\r', '\n'}.
    Each of these characters delimits a token.
    Tokens are of two kind: {key, value}. Their correspondence is one-to-one.
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


// private: unusable.
Common::ConfigurationService::ConfigurationService(const Common::ConfigurationService &other)
{}

// private: unusable.
Common::ConfigurationService & Common::ConfigurationService::operator=(const Common::ConfigurationService &other)
{
    Common::ConfigurationService * res = nullptr;
    return *res;
}

inline bool Common::ConfigurationService::FileExists_test( const std::string& name) const
{
    struct stat buffer;// local variable, whose address is taken to get an output parameter.
    int result = stat( name.c_str(), &buffer);// NB. buffer contains a lot of interesting fields after the call.
    bool FileExists = (result==0);
    return FileExists;
}


/// NB. : feed with config_path : "default" stands for "./AppConfig.txt" ; whatever else for specific configs;
/// NB. on Unix the process has allowance to write only in its working dir (i.e. "./" ).
Common::ConfigurationService::ConfigurationService( const std::string & configPath)
{
    std::vector<std::string> * local_beforeClean_tokenArray = nullptr;
    std::vector<std::string> * local_tokenArray = nullptr;// this will receive the content of local_beforeClean_tokenArray.
    try
    {// first: book the memory. In case of Ctor failure, it will be anyway necessary to answer the queries.
        int configPathAnalysis = configPath.compare("default");
        std::string defaultConfigPath;
        switch( configPathAnalysis)
        {
        case 0:// strings are= so configPath=="default" -> "./AppConfig.txt"
            {
                defaultConfigPath = "./AppConfig.txt";// this is the default configFile, specifiable by explicit name xor by the word "default".
                break;
            }
            default:// which means configPath!="default"
            {// support for nonStandard configs:
                defaultConfigPath = configPath;// support for nonStandard configs:
                break;
            }
        }// end switch
        this->key_all_together = new std::vector<std::string>();// NB. do not book cardinalities!
        this->key_val_map = new std::map<std::string, std::string>();// NB. do not book cardinalities!
        this->key_all_together->push_back( std::string("Invalid configuration file."));// if Ctor fail this will be necessary.
        // the map key-val does not require prudential init.
        this->isConstructorStillAlive = true;// by now... NB. remember RAII== Resource Allocation Is Initialization !
        this->isHealtlyConstructed = true;// by now...
        this->whyNotHealtlyConstructed = "not yet initialized";
        this->whyNotAvailableKeyVal = "not yet initialized";
        // here starts the onion of the Ctor: it's necessary, since there's no way to thow or return from inside a try-block in a Ctor.
        if(isConstructorStillAlive)
        {// se esiste il FILE
            if( this->FileExists_test(defaultConfigPath) )
            {
                isConstructorStillAlive = true;
            }
            else
            {
                isConstructorStillAlive = false;
                isHealtlyConstructed = false;
                this->whyNotHealtlyConstructed = "\n\t NOT healtly constructed: row 104 : ConfigFile does not exist on FileSystem.";
            }
            if( isConstructorStillAlive)
            {// se riesco ad aprirlo in lettura
                this->theInStream.open( defaultConfigPath, std::ios::in );
                if( this->theInStream.is_open() )
                {
                    isConstructorStillAlive = true;
                }
                else
                {
                    isConstructorStillAlive = false;
                    this->whyNotHealtlyConstructed = "\n\t NOT healtly constructed: row 116 : ConfigFile does not Open() for read.";
                }
                if(isConstructorStillAlive)
                {// se la sua sintassi e' valida : i token devono essere n + 1 + n == keys + separator + values.
                    local_beforeClean_tokenArray = new std::vector<std::string>;
                    //
                    int c=0;
                    for( c=0; !theInStream.eof();  )
                    {
                        std::string buf;// tmp for current token.
                        theInStream >> buf;// cosi' va per WORD separate da blank o TAB o new line
                        local_beforeClean_tokenArray->push_back( buf );
                        ++c;
                    }
                    local_tokenArray =
                        StrManipul::removeEmptyEntries( local_beforeClean_tokenArray );// NB. remove empties
                    delete local_beforeClean_tokenArray;//NB. clean the original!
                    int cardToken = local_tokenArray->size();// keys + '#' + values : must be n + 1 + n ==2*n+1 so odd (dispari).
                    double ratio = (double)cardToken / 2.0;
                    double fractionalPart = ratio - (int)ratio;
                    if( fabs(fractionalPart) < +1.0E-16 )// was even (pari) -> errore di configurazione (must be 2*n+1).
                    {
                        this->isHealtlyConstructed = false;
                        this->isConstructorStillAlive = false;
                        this->whyNotHealtlyConstructed = "\n\t NOT healtly constructed: row 140 : ConfigFile does not have odd tokens : they have to be n + 1 + n ==2*n+1 so odd (dispari).";
                    }// else can continue.
                    //# END se la sua sintassi e' valida : i token devono essere n + 1 + n == keys + separator + values.
                    if(isConstructorStillAlive)
                    {//# se il separatore è al suo posto : n + 1 + n.
                        int candidateSeparatorPosition = (int)ratio;// not+1 because we count from 0.
                        if( 0 != ((std::string)((*local_tokenArray)[candidateSeparatorPosition])).compare("#") )
                        {
                            this->isHealtlyConstructed = false;
                            this->isConstructorStillAlive = false;
                            this->whyNotHealtlyConstructed = "\n\t NOT healtly constructed: row 150 : ConfigFile does not have the separator token between keys#values (i.e. the # token).";
                        }// else can continue.
                        // END # se il separatore è al suo posto : n + 1 + n.
                        if(isConstructorStillAlive)
                        {// popola la mappa
                            this->key_all_together->clear();// NB. if you get here, you have to removethe init-to-invalid record( which is the only one by now).
                            int c=0;
                            int n = (int)ratio;// n means how many keys in configuration.
                            for( std::vector<std::string>::iterator curToken=local_tokenArray->begin();
                                 curToken != local_tokenArray->begin()+n; //curToken!=local_tokenArray->end();
                                 curToken++ )
                            {// start for each key in local_tokenArray : prepare this->key_all_together and key_val_map.
                                // NB. indici {key,value} corrispondenti sono {[c],[c+n+1]}
                                this->key_all_together->push_back( *curToken );// push all keys in a vector.
                                (*(this->key_val_map))[*curToken] = (*(curToken+n+1)).c_str();// push such key together with its corresponding value, in the map.
                                c++;// keep count of how many couples {key,value}.
                                if( n == c)// if keys are over:
                                {// if keys are over:
                                    break;// keys are over; their corresponding values, have already been treated.
                                }// else have to continue.
                            }// end for each key in local_tokenArray : prepare this->key_all_together and key_val_map.
                            if( this->theInStream.is_open() )
                            {
                                this->theInStream.close();
                            }// else config file already closed.
                            // final usage of bool_result
                            isConstructorStillAlive &= (c>0);// have found at least one key ?
                            delete local_tokenArray;// this was a temporary, for the first scan; then the file contents have been categorized in specialized data structs.
                            local_tokenArray = nullptr;
                            isConstructorStillAlive = true;
                        }// END popola la mappa
                    }// END se il separatore è al suo posto : n + 1 + n.
                }// END se la sua sintassi e' valida : i token devono essere n + 1 + n == keys + separator + values.
            }// END   se riesco ad aprirlo in lettura
        }// END  se esiste
    }// END try
    catch(...)
    {
        this->isHealtlyConstructed = false;
        this->whyNotHealtlyConstructed = "\n\t NOT healtly constructed: row 177 : Catch(...) in Ctor().";
        this->isConstructorStillAlive = false;
        if( nullptr != local_tokenArray )
        {
            delete local_tokenArray;// this was a temporary, for the first scan; then the file contents have been categorized in specialized data structs.
            local_tokenArray = nullptr;
        }
    }// END catch
    // ready.
}// END default Ctor


std::vector<std::string> * Common::ConfigurationService::showInstanceHealtCondition() const
{
    //  std::string whyNotHealtlyConstructed;
    //  std::string whyNotAvailableKeyVal;
    //  bool isConstructorStillAlive;// the Constructor use it for the onion-peeling technique. NB. a Ctor cannot return from inside a try block.
    //  bool isHealtlyConstructed;//
    std::vector<std::string> * result = new std::vector<std::string>();
    std::stringstream formatConverter;
    formatConverter << "this->isConstructorStillAlive == ";
    formatConverter << (this->isConstructorStillAlive) ;
    formatConverter << " this->isHealtlyConstructed == ";
    formatConverter << this->isHealtlyConstructed;
    result->push_back( formatConverter.str() );
    formatConverter.clear();
    result->push_back( this->whyNotHealtlyConstructed);
    result->push_back( this->whyNotAvailableKeyVal);
    return result;
}// end showInstanceHealtCondition

bool Common::ConfigurationService::readIsHealtlyConstructed() const
{
    return this->isHealtlyConstructed;
}


// Dtor
Common::ConfigurationService::~ConfigurationService()
{// the config_file has already been closed by the Ctor.
    this->key_val_map->clear();
    delete this->key_val_map;
    this->key_all_together->clear();
    delete this->key_all_together;
}// end Dtor



/// give READ-ONLY just ONE of the values, the one that corresponds to the given key.
std::string * Common::ConfigurationService::getValue(const std::string &key)
{// give READ-ONLY just ONE of the values, the one that corresponds to the given key.
    std::string * res = new std::string( "key not found.");// init prudentially.
    if( this->isHealtlyConstructed)
    {
        std::map<std::string, std::string>::iterator mapPointer;
        try
        {
            mapPointer = this->key_val_map->find( key );
            if( this->key_val_map->end() != mapPointer)// this avoids adding void knots to the tree.
            {
                delete res;// the record of init-prudentially.
                //NB. due scritture equivalenti res = new std::string( (*this->key_val_map)[key.c_str()] );// and re-alloc. NB. due scritture equivalenti
                res = new std::string( (*mapPointer).second );// and re-alloc.
            }// END if key presente in mappa
            else
            {// else : key non presente in mappa
                this->whyNotAvailableKeyVal = "\n\t map points to end. getValue::row 255.";
                // already initialized prudentially : res = new std::string( "key not found.");
            }// END else : key non presente in mappa
        }// end try
        catch(...)
        {// already initialized prudentially : res = new std::string( "key not found.");
            this->whyNotAvailableKeyVal = "\n\t  map catch(...). getValue::row 261.";
        }
    }// END if( this->isHealtlyConstructed)
    else
    {// else // already initialized prudentially : res = new std::string( "key not found.");
        this->whyNotAvailableKeyVal = "\n\t  map :  NOT healtly constructed getValue::row 266.";
    }// END else : NOTHealtlyConstructed
    // ready.
    return res;
}// END give READ-ONLY just ONE of the values, the one that corresponds to the given key.



/// give READ-ONLY all of the keys, just as a phone book.
std::vector<std::string> * Common::ConfigurationService::getAllKeys() const
{// give READ-ONLY all of the keys, just as a phone book.
    // NB.  do not check: if(this->isHealtlyConstructed)// do not check: this->key_all_together is prepared to reply in case of this_NOT_healtlyConstructed.
    // ready
    return (this->key_all_together);// return the pointee, by reference.
}// END give READ-ONLY all of the keys, just as a phone book.







////---------------------- start CANTINA ---------------------------------------------------------
/*
bool Common::ConfigurationService::instreamOpener( const std::string &fullPath, std::fstream &theInStream)
{
	bool result = false;
	//std::ios_base::openmode x = 1;// read mode.

	try
	{
		theInStream.open( fullPath.c_str() , std::ios::in );
		if( theInStream.is_open() )
		{
			result = true;
			// do something
		}
		else
		{
			result = false;
		}
	}
	catch(...)
	{
	}
	//finally
	//{
	//}
	// return
	return result;
}
*/

/*
bool Common::ConfigurationService::instreamCloser( std::fstream &theInStream)
{
	bool result = false;
	//
	try
	{
		if( theInStream.is_open() )
		{
			theInStream.close();
			result = true;
		}
		else
		{
			result = false;
		}
	}
	catch(...)
	{
		result = false;
	}
	//finally
	//{
	//}
	// return
	return result;
}
*/

/* TODO : cercare il separatore '#'
    controllare che siano uguali le cardinalita' di key e value
    predisporre due vector<string> , una per key e una per value (stessa cardinalita' ! )
    popolare la mappa, mediante le corrispondenze biunivoche [key, value]
    comportarsi camera sterile
*/
/*
bool Common::ConfigurationService::instream_getToken( const std::string &fullPath,  std::fstream &theInStream)
{
    bool result = instreamOpener( fullPath , theInStream );
    std::vector<std::string> * local_tokenArray = new std::vector<std::string>;
    //
    for( int c=0; !theInStream.eof();  )
    {
        std::string buf;// tmp for current token.
        theInStream >> buf;// cosi' va per WORD separate da blank o TAB o new line
        std::cout<<buf<<std::endl;
        local_tokenArray->push_back( buf );
        ++c;
    }
    int cardToken = local_tokenArray->size();// keys + '#' + values : must be n + 1 + n ==2*n+1 so odd (dispari).
    double ratio = (double)cardToken / 2.0;
    double fractionalPart = ratio - (int)ratio;
    if( fabs(fractionalPart) < +1.0E-16 )// was even (pari) -> errore di configurazione (must be 2*n+1).
    {
        this->isHealtlyConstructed = false;
        return (this->isHealtlyConstructed);
    }// else can continue.
    int candidateSeparatorPosition = (int)ratio;
    std::cout<< ((std::string)((*local_tokenArray)[candidateSeparatorPosition])) <<std::endl;
    if( ! ((std::string)((*local_tokenArray)[candidateSeparatorPosition])).compare("#") )
    {
        this->isHealtlyConstructed = false;
        return (this->isHealtlyConstructed);
    }// else can continue.
    int c=0;
    this->key_all_together = new std::vector<std::string>((int)ratio);// how many keys. // new in Ctor !
    this->key_val_map = new std::map<std::string, std::string>();
    int n = (int)ratio;
    for(std::vector<std::string>::iterator curToken=local_tokenArray->begin(); curToken!=local_tokenArray->end(); curToken++)
    {
        // indici [c] e [c+n+1]
        this->key_all_together->push_back( *curToken );// push a key
//std::string tmp(*curToken);
        (*(this->key_val_map))[*curToken] = (*(curToken+n+1)).c_str();// push such key together with its corresponding value, in the map.
        c++;
        if( (int)ratio == c)
        {
            break;// keys are over; their corresponfing values, have already been treated.
        }// else have to continue.
    }// end for each key.
    result =instreamCloser( theInStream );
    // final usage of bool_result
    result = (c>0);
    delete local_tokenArray;
    return result;
}// end  instream_getToken()
*/
/* cantina
// TODO : feed with config_path and open
Common::ConfigurationService::ConfigurationService()
{
    try
    {// first: book the memory. In case of Ctor failure, it will be anyway necessary to answer the queries.
        this->key_all_together = new std::vector<std::string>();// NB. do not book cardinalities!
        this->key_val_map = new std::map<std::string, std::string>();// NB. do not book cardinalities!
        this->key_all_together->push_back( std::string("Invalid configuration file."));// if Ctor fail this will be necessary.
        // the map key-val does not require prudential init.
        this->isConstructorStillAlive = true;// by now...
        std::string defaultConfigPath("./AppConfig.txt");// this is the default configFile TODO: add support for nonStandard configs.
        // here starts the onion of the Ctor: it's necessary, since there's no way to thow or return from inside a try-block in a Ctor.
        if(isConstructorStillAlive)
        {// se esiste il FILE
            if( this->FileExists_test(defaultConfigPath) )
            {
                isConstructorStillAlive = true;
            }
            else
            {
                isConstructorStillAlive = false;
            }
            if( isConstructorStillAlive)
            {// se riesco ad aprirlo in lettura
                this->theInStream.open( defaultConfigPath, std::ios::in );
                if( this->theInStream.is_open() )
                {
                    isConstructorStillAlive = true;
                }
                else
                {
                    isConstructorStillAlive = false;
                }
                if(isConstructorStillAlive)
                {// se la sua sintassi e' valida : i token devono essere n + 1 + n == keys + separator + values.
                    std::vector<std::string> * local_tokenArray = new std::vector<std::string>;
                    //
                    int c=0;
                    for( c=0; !theInStream.eof();  )
                    {
                        std::string buf;// tmp for current token.
                        theInStream >> buf;// cosi' va per WORD separate da blank o TAB o new line
                        std::cout<<buf<<std::endl;
                        local_tokenArray->push_back( buf );
                        ++c;
                    }
                    std::cout<<"\n DEBUG contatore keys ! "<< c <<std::endl;
                    int cardToken = local_tokenArray->size();// keys + '#' + values : must be n + 1 + n ==2*n+1 so odd (dispari).
                    std::cout<<"\n DEBUG contatore keys da vector ! "<< cardToken <<std::endl;
                    double ratio = (double)cardToken / 2.0;
                    double fractionalPart = ratio - (int)ratio;
                    if( fabs(fractionalPart) < +1.0E-16 )// was even (pari) -> errore di configurazione (must be 2*n+1).
                    {
                        this->isHealtlyConstructed = false;
                        this->isConstructorStillAlive = false;
                    }// else can continue.
                    //# END se la sua sintassi e' valida : i token devono essere n + 1 + n == keys + separator + values.
                    if(isConstructorStillAlive)
                    {//# se il separatore è al suo posto : n + 1 + n.
                        int candidateSeparatorPosition = (int)ratio;// not+1 because we count from 0.
                        std::cout<<"\n\n DEBUG ! "<< ((std::string)((*local_tokenArray)[candidateSeparatorPosition])) <<std::endl;
                        if( 0 != ((std::string)((*local_tokenArray)[candidateSeparatorPosition])).compare("#") )
                        {
                            this->isHealtlyConstructed = false;
                            this->isConstructorStillAlive = false;
                        }// else can continue.
                        // END # se il separatore è al suo posto : n + 1 + n.
                        if(isConstructorStillAlive)
                        {// popola la mappa
                            this->key_all_together->clear();// NB. if you get here, you have to removethe init-to-invalid record( which is the only one by now).
                            int c=0;
                            int n = (int)ratio;
                            for( std::vector<std::string>::iterator curToken=local_tokenArray->begin();
                                 curToken != local_tokenArray->begin()+n; //curToken!=local_tokenArray->end();
                                 curToken++ )
                            {// for each key:
                                // indici {key,value} corrispondenti sono {[c],[c+n+1]}
                                std::cout<<"\t DEBUG inserim key_all_together in posiz. : "<<c<<"  "<< *curToken<<std::endl;
                                std::cout<<"\n\t DEBUG *external break* cardinalita key_all_together  : "<< this->key_all_together->size() <<std::endl;
                                this->key_all_together->push_back( *curToken );// push all keys in a vector.
                                (*(this->key_val_map))[*curToken] = (*(curToken+n+1)).c_str();// push such key together with its corresponding value, in the map.
                                c++;// keep count of how many couples {key,value}.
                                std::cout<<"\n\t contatore in inserimento in mappa : "<< c <<std::endl;
                                std::cout<<"\n\t DEBUG *external break* cardinalita key_all_together  : "<< this->key_all_together->size() <<std::endl;
                                if( n == c)
                                {// if keys are over:
                                    std::cout<<"\t DEBUG *internal break* cardinalita key_all_together  : "<< this->key_all_together->size() <<std::endl;
                                    break;// keys are over; their corresponding values, have already been treated.
                                }// else have to continue.
                            }// end for each key.
                            for(std::map<std::string, std::string>::iterator mapIter=this->key_val_map->begin();
                                 mapIter != this->key_val_map->end() ;
                                 mapIter++ )
                            {
                                 std::cout<<"\t DEBUG map cardinality : "<< this->key_val_map->size() << std::endl;
                                std::cout<<"\t DEBUG map  : "<< mapIter->first<<"   "<< mapIter->second  <<std::endl;
                            }
                            std::cout<<"\t DEBUG *** cardinalita key_all_together  : "<< this->key_all_together->size() <<std::endl;
                            // DBG
                            std::cout<<"\n\n DEBUG : "<<std::endl;
                            for(std::vector<std::string>::iterator curToken=local_tokenArray->begin(); curToken!=local_tokenArray->end(); curToken++)
                            {// start DEBUG for
                                if( ! ((std::string)(*curToken)).compare("#") )
                                {
                                    break;// END keys
                                }
                                std::cout<<" key = "<< *curToken ;
                                std::cout<<" its value is "<< (*this->key_val_map)[(*curToken).c_str()]<<std::endl;
                            }// END DEBUG for
                            std::cout<<"END DEBUG ! "<<std::endl;
                            // end DBG
                            if( this->theInStream.is_open() )
                            {
                                this->theInStream.close();
                            }// else config file already closed.
                            // final usage of bool_result
                            isConstructorStillAlive &= (c>0);// have found at least one key ?
                            delete local_tokenArray;// this was a temporary, for the first scan; then the file contents have been categorized in specialized data structs.
                            isConstructorStillAlive = true;
                        }// END popola la mappa
                    }// END se il separatore è al suo posto : n + 1 + n.
                }// END se la sua sintassi e' valida : i token devono essere n + 1 + n == keys + separator + values.
            }// END   se riesco ad aprirlo in lettura
        }// END  se esiste
    }// END try
    catch(...)
    {
        this->isHealtlyConstructed = false;
        this->isConstructorStillAlive = false;
    }// END catch
    // ready.
}// END default Ctor


// give READ-ONLY just ONE of the values, the one that corresponds to the given key.
std::string & Common::ConfigurationService::getValue(const std::string &key) const
{
    std::string * res = new std::string( "key not found.");// init prudentially.
    if(this->isHealtlyConstructed)
    {
        std::map<std::string, std::string>::iterator mapPointer;
        try
        {
            //std::string tmp = (std::string)((this->key_val_map)[key]);
            mapPointer = this->key_val_map->find( key );
            if( this->key_val_map->end() != mapPointer)
            {
                delete res;// the record of init-prudentially.
                res = new std::string( (*mapPointer).second );// and re-alloc.
            }
            else
            {
                // already initialized prudentially : res = new std::string( "key not found.");
            }
        }// end try
        catch(...)
        {
            // already initialized prudentially : res = new std::string( "key not found.");
        }
       #define map_DEBUG
       #ifdef map_DEBUG
        for(std::map<std::string, std::string>::iterator mapIter=this->key_val_map->begin();
             mapIter != this->key_val_map->end() ;
             mapIter++ )
        {
             std::cout<<"\n\n\t DEBUG map cardinality : "<< this->key_val_map->size() << std::endl;
            std::cout<<"\t DEBUG map  : "<< mapIter->first<<"   "<< mapIter->second  <<std::endl;
        }
        #endif // map_DEBUG
    }// else // already initialized prudentially : res = new std::string( "key not found.");
    return *res;
}// END give READ-ONLY just ONE of the values, the one that corresponds to the given key.



// give READ-ONLY all of the keys, just as a phone book.
std::vector<std::string> & Common::ConfigurationService::getAllKeys() const
{
    if(this->isHealtlyConstructed)
    {
        std::cout<<"\n\n\t DEBUG cardinalita key_all_together  : "<< this->key_all_together->size() <<std::endl;
        #define keys_DEBUG
        #ifdef keys_DEBUG
            int c=0;
            for(std::vector <std::string>::iterator keysIter=this->key_all_together->begin();
                keysIter != this->key_all_together->end() ;
                keysIter++ )
            {
                std::cout<<"\t DEBUG keys cardinality : "<< this->key_all_together->size() << std::endl;
                std::cout<<"\t DEBUG keys  : "<< *keysIter << "   "<< ++c <<std::endl;
            }
        #endif // keys_DEBUG
    }// else failed
    // ready
    return *(this->key_all_together);// return the pointee, by reference.
}// END give READ-ONLY all of the keys, just as a phone book.

*/
