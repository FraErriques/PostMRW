#ifndef DBCONNECTIONSERVICE_H_INCLUDED
#define DBCONNECTIONSERVICE_H_INCLUDED


#include <string>


namespace Common
{

namespace DbConnectionService
{


sql::ResultSet * dbName_tableName_LOAD_MULTI_( const std::string &dbName, const std::string &tableName);
int dbName_tableName_paramsList_INSERT_SINGLE( const std::string &dbName, const std::string &tableName, const std::string &paramsList );

}// nmsp
}// nmsp

#endif // DBCONNECTIONSERVICE_H_INCLUDED
