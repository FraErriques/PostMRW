//#include "../../Common/StringBuilder/StringBuilder.h"
//#include <iostream>
//#include <iterator>
//#include <algorithm>
//#include <exception>
//#include <stdlib.h>
////-------------------#include <boost/lambda/lambda.hpp>
//#include "mysql_connection.h"
//#include <cppconn/driver.h>
//#include <cppconn/exception.h>
//#include <cppconn/resultset.h>
//#include <cppconn/statement.h>
//#include <cppconn/prepared_statement.h>
//#include "DbConnectionService.h"
//
//
//
//namespace Common
//{
//
//namespace dbCall
//{
//
//
//
//
//
//using namespace std;
//
//
//
//int cantiere_sede_LOAD_MULTI_( )
//{
//    cout << endl;
//    cout << "\n\n\t" << "cantiere_sede_LOAD_MULTI_" <<" : " << endl;
//    int rowCard = 0;
//    //
//    try
//    {
//        sql::Driver *driver = nullptr;
//        sql::Connection *con = nullptr;
//        sql::Statement *stmt = nullptr;
//        sql::ResultSet *res = nullptr;
//
//        // Create a connection : steps follow:
//        driver = get_driver_instance();// get the driver
//        con = driver->connect( "tcp://127.0.0.1:3306", "root", "Riemann0");// get the connection to the specified instance.
//        //
//        // execute something, with the current connection:
//        stmt = con->createStatement();// prepare a statement : it will be returned and then deleted by the caller :
//        Common::StringBuilder sb(500);
//        sb.append( " call cantiere.usp_SEDE_LOAD_MULTI");
//        res = stmt->executeQuery( sb.str() );// execute the statement and init the Cursor to be returned: the caller will have to fetch it.
//        //
//        //-------fetch the resultset--------------------
//        while( res->next() )
//        {
//            cout << "\t... MySQL replies: ";
//            cout << res->getString("id") << "   ";// Access column data by alias or column name
//            cout << res->getString("Baustelle_name") << "   ";// Access column data by alias or column name
//            cout << res->getString("role") << "   ";// Access column data by alias or column name
//            cout << res->getString("set_time") << "   ";// Access column data by alias or column name
//            cout << res->getString("set_by") << endl;// Access column data by alias or column name // NewLine on end of Record (which is a table's line).
//            //cout << res->getString(1) << endl;// Access column data by numeric offset, 1 is the first column
//            ++rowCard;// done a line.
//        }//--END-----fetch the resultset--------------------
//        //
//        std::cout << "\n\n\t Processed lines # " << rowCard << std::endl;
//        //--- cleanup------
//        delete res;
//        delete stmt;
//        if( ! con->isClosed() )
//        {
//            con->close();
//        }// else already closed.
//        delete con;
//    }// END try
//    catch( sql::SQLException &e)
//    {
//        cout << "# ERR: SQLException in " << __FILE__;
//        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
//        cout << "# ERR: " << e.what();
//        cout << " (MySQL error code: " << e.getErrorCode();
//        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
//    }// END catch( sql::SQLException &e)
//    catch( const std::exception& ex)
//    {
//        std::string HappenedException = ex.what();
//    }
//    catch (const std::string& ex)
//    {
//        std::string HappenedException = ex.c_str();
//    }
//    catch(...)
//    {
//        std::string HappenedException = "this was (sadly) an unnamed exception, which -therefore- has no content.";
//    }// END catch(...)
//
//    // ready.
//    return rowCard;
//}// end cantiere_sede_LOAD_MULTI_
//
//
//
//
//
//
//bool cantiere_sede_INSERT_SINGLE(void)
//{
//    cout << endl;
//    cout << " cantiere_sede_INSERT_SINGLE : " << endl;
//    bool insertionHasError = true;// init insertion result
//
//    try
//    {
//        insertionHasError = Common::DbConnectionService::dbName_tableName_paramsList_INSERT_SINGLE(
//                                                       "cantiere"
//                                                       ,"usp_SEDE_INSERT_SINGLE"
//                                                       ,"'2018#marzo#17_fromCPP' , '2018#marzo#17_fromCPP' , '2018#marzo#17_fromCPP'" // NB. 'fsdf' around sql_Varchar_params
//                                                                                                   );// execute the statement
//    }// END try
//    catch( sql::SQLException &e)
//    {
//        cout << "# ERR: SQLException in " << __FILE__;
//        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
//        cout << "# ERR: " << e.what();
//        cout << " (MySQL error code: " << e.getErrorCode();
//        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
//    }// END catch( sql::SQLException &e)
//    catch( const std::exception& ex)
//    {
//        std::string HappenedException = ex.what();
//    }
//    catch (const std::string& ex)
//    {
//        std::string HappenedException = ex.c_str();
//    }
//    catch(...)
//    {
//        std::string HappenedException = "this was (sadly) an unnamed exception, which -therefore- has no content.";
//    }// END catch(...)
//
//    // ready.
//    cout << endl;
//    return insertionHasError;// false on insertionSuccess( 1 row i.e. INSERT_SINGLE), ture on error.
//}// end cantiere_sede_INSERT_SINGLE
//
//
//bool cantiere_numericTable_INSERT_SINGLE( double calculatedVal)
//{
//    cout << endl;
//    cout << " cantiere_numericTable_INSERT_SINGLE : " << endl;
//    bool insertionHasError = true;// init insertion result
//
//    try
//    {
//        const std::string * theVal = Common::StrManipul::doubleToString( calculatedVal);
//        insertionHasError = Common::DbConnectionService::dbName_tableName_paramsList_INSERT_SINGLE(
//            "cantiere"
//            ,"usp_intg_Sin_0_Pi_INSERT_SINGLE"
//            ,*theVal // NB. 'fsdf' around sql_Varchar_params but not around numbers
//            );// execute the statement
//        delete theVal;
//    }// END try
//    catch( sql::SQLException &e)
//    {
//        cout << "# ERR: SQLException in " << __FILE__;
//        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
//        cout << "# ERR: " << e.what();
//        cout << " (MySQL error code: " << e.getErrorCode();
//        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
//    }// END catch( sql::SQLException &e)
//    catch( const std::exception& ex)
//    {
//        std::string HappenedException = ex.what();
//    }
//    catch (const std::string& ex)
//    {
//        std::string HappenedException = ex.c_str();
//    }
//    catch(...)
//    {
//        std::string HappenedException = "this was (sadly) an unnamed exception, which -therefore- has no content.";
//    }// END catch(...)
//
//    // ready.
//    cout << endl;
//    return insertionHasError;// false on insertionSuccess( 1 row i.e. INSERT_SINGLE), ture on error.
//}// end cantiere_numericTable_INSERT_SINGLE
//
//
//bool cantiere_intg_Sin_0_Pi_INSERT_SINGLE( double par)
//{
//    cout << endl;
//    cout << " usp_intg_Sin_0_Pi_INSERT_SINGLE : " << endl;
//    bool insertionHasError = true;// init insertion result
//
//    try
//    {
//        sql::Driver *driver;
//        sql::Connection *con;
//        sql::Statement *stmt;
//
//        // Create a connection : steps follow:
//        driver = get_driver_instance();// get the driver
//        con = driver->connect( "tcp://127.0.0.1:3306", "root", "Riemann0");// get the connection to the specified instance.
//        //
//        // execute something, with the current connection:
//        stmt = con->createStatement();// prepare a statement
//        Common::StringBuilder sb(400);
//        sb.append("call cantiere.usp_intg_Sin_0_Pi_INSERT_SINGLE ( ");
//        std::string * tmp = Common::StrManipul::doubleToString( par);
//        sb.append( *tmp );
//        sb.append( " )");
//        const std::string theStatement = sb.str();
//        insertionHasError = stmt->execute( theStatement);// execute the statement
//        //--- cleanup------
//        delete tmp;
//        delete stmt;
//        if( ! con->isClosed() )
//        {
//            con->close();
//        }// else already closed.
//        delete con;
//    }// END try
//    catch( sql::SQLException &e)
//    {
//        cout << "# ERR: SQLException in " << __FILE__;
//        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
//        cout << "# ERR: " << e.what();
//        cout << " (MySQL error code: " << e.getErrorCode();
//        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
//    }// END catch( sql::SQLException &e)
//    catch( const std::exception& ex)
//    {
//        std::string HappenedException = ex.what();
//    }
//    catch (const std::string& ex)
//    {
//        std::string HappenedException = ex.c_str();
//    }
//    catch(...)
//    {
//        std::string HappenedException = "this was (sadly) an unnamed exception, which -therefore- has no content.";
//    }// END catch(...)
//
//    // ready.
//    cout << endl;
//    return insertionHasError;// false on insertionSuccess( 1 row i.e. INSERT_SINGLE), ture on error.
//}// end cantiere_sede_INSERT_SINGLE
//
//
//
//
//// --- collateralia ------------------
//
//
//
//void boost_test()
//{
//    using namespace boost::lambda;
//    typedef std::istream_iterator<int> in;
//
//    std::for_each(
//                    in(std::cin)
//                    , in()
//                    , std::cout << (_1 * 3) << " "
//                  );
//}// end boost_test
//
//
//
//
//int OracleSuggests()
//{
//    cout << endl;
//    cout << "Running 'SELECT 'Hello World!' AS _message'..." << endl;
//
//    try
//    {
//        sql::Driver *driver;
//        sql::Connection *con;
//        sql::Statement *stmt;
//        sql::ResultSet *res;
//
//        // Create a connection : steps follow:
//        driver = get_driver_instance();// get the driver
//        con = driver->connect( "tcp://127.0.0.1:3306", "root", "Riemann0");// get the connection to the specified instance.
//        //con = driver->connect( "tcp://Eulero:3306", "root", "Riemann0");// get the connection to the specified instance.
//        // con->setAutoCommit( false);
//        //con->setAutoCommit( true);
//        //con->setSchema("cantiere");// choose which database, within the instance.
//        //
//        // execute something, with the current connection:
//        stmt = con->createStatement();// prepare a statement
//        res = stmt->executeQuery("SELECT 'Hello World!' AS _message");// execute the statement
//        //
//        //-------fetch the resultset--------------------
//        while (res->next())
//        {
//            cout << "\t... MySQL replies: ";
//            // Access column data by alias or column name
//            cout << res->getString("_message") << endl;
//            cout << "\t... MySQL says it again: ";
//            // Access column data by numeric offset, 1 is the first column
//            cout << res->getString(1) << endl;
//        }//--END-----fetch the resultset--------------------
//        //--- cleanup------
//        delete res;
//        delete stmt;
//        delete con;
//    }// END try
//    catch( sql::SQLException &e)
//    {
//        cout << "# ERR: SQLException in " << __FILE__;
//        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
//        cout << "# ERR: " << e.what();
//        cout << " (MySQL error code: " << e.getErrorCode();
//        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
//    }// END catch( sql::SQLException &e)
//    catch( const std::exception& ex)
//    {
//        std::string HappenedException = ex.what();
//    }
//    catch (const std::string& ex)
//    {
//        std::string HappenedException = ex.c_str();
//    }
//    catch(...)
//    {
//        std::string HappenedException = "this was (sadly) an unnamed exception, which -therefore- has no content.";
//    }// END catch(...)
//
//    // ready.
//    cout << endl;
//    getchar();
//    return EXIT_SUCCESS;
//}// end OracleSuggests.
//
//
//
//
//
//    int command_PREPARE(void)
//    {
//        cout << endl;
//        cout << "Let's have MySQL use the PREPARE command : " << endl;
//        //
//        try
//        {
//            sql::Driver *driver = nullptr;
//            sql::Connection *con = nullptr;
//            sql::PreparedStatement *pstmt = nullptr;
//            //
//            /* Create a connection */
//            driver = get_driver_instance();
//            con = driver->connect("tcp://127.0.0.1:3306", "root", "Riemann0");
//            /* Connect to the MySQL test database */
//            // con->setAutoCommit( false);// test : resulted in : no_except but process hanging.
//            // con->setSchema("cantiere");// test : resulted in : no_except but process hanging.
//            //
//            //-----PREPARE example------------------
//            //  /* '?' is the supported placeholder syntax */
//            //  SET @s = 'SELECT SQRT(POW(?,2) + POW(?,2)) AS hypotenuse';
//            //  PREPARE stmt2 FROM @s;
//            //  SET @a = 6;
//            //  SET @b = 8;
//            //  EXECUTE stmt2 USING @a, @b;
//            //-- +------------+
//            //--  hypotenuse == 10
//            //-- +------------+
//            //  DEALLOCATE PREPARE stmt2;
//            //--end---PREPARE example------------------
//            //
//            //pstmt = new sql::PreparedStatement();// Abstract class : cannot instantiate.
//            std::string * stdStringCommandText = new std::string( "SELECT SQRT(POW(?,2) + POW(?,2)) AS hypotenuse");
//            sql::SQLString * cmdText = new sql::SQLString( *stdStringCommandText );
//            pstmt = con->prepareStatement( *cmdText);// have to call sql::Connection::prepareStatement method to initialize
//            pstmt->setInt( 1, 6);// first  '?' gets value==+6 //  SET @a = 6;
//            pstmt->setInt( 2, 6);// second '?' gets value==+8 //  SET @b = 8;
//            pstmt->executeUpdate();           //  EXECUTE stmt2 USING @a, @b;
//            delete pstmt;// delete each PREPARE
//            //
//            if( ! con->isClosed() )
//            {
//                con->close();
//            }// else already closed.
//            delete con;// delete the whole connection.
//            delete stdStringCommandText;
//            delete cmdText;// sqlString
//        }
//        catch (sql::SQLException &e)
//        {
//            cout << "# ERR: SQLException in " << __FILE__;
//            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
//            cout << "# ERR: " << e.what();
//            cout << " (MySQL error code: " << e.getErrorCode();
//            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
//        }
//        catch( const std::exception& ex)
//        {
//            std::string HappenedException = ex.what();
//        }
//        catch (const std::string& ex)
//        {
//            std::string HappenedException = ex.c_str();
//        }
//        catch(...)
//        {
//            std::string HappenedException = "this was (sadly) an unnamed exception, which -therefore- has no content.";
//        }// END catch(...)
//        cout << endl;
//        return EXIT_SUCCESS;
//    }
//
//
//
//}// nmsp dBCall
//}// end nmsp Common
