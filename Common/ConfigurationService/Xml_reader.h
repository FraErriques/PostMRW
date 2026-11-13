/*
### Step 1: Install PugiXML via MSYS2 Pacman
Open your **MSYS2 MinGW64 terminal** and run the following command:

```bash
pacman -S mingw-w64-x86_64-pugixml

2. **Add Linker Flag:** Add library option.
Under the **Other linker options** box, add:
-lpugixml
*/


#include <iostream>
#include <string>
#include "pugixml.hpp"
// dbg
#include "../LogSinkDb/LogSinkDb.h"
#include <thread>
#include <chrono>

/*
<?xml version="1.0" encoding="utf-8"?>
*/


void xml_config_read() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("config.xml");

    if (!result) {
        std::cerr << "XML parse error: " << result.description() << std::endl;
        return;
    }

    // 2. Extracting nested database config (matching your commented Rust code)
    pugi::xml_node db = doc.select_node("//database").node();
    if (db) {
        std::string server     = db.child("server").child_value();
        std::string port    = db.child("port").child_value();
        std::string db_name    = db.child("db_name").child_value();
        std::string user       = db.child("user").child_value();
        std::string password   = db.child("password").child_value();
        std::string schema     = db.child("schema").child_value();

        // Build a PostgreSQL connection string for libpqxx:
        std::string conn_str = "host=" + server +
                               " port=" + port +
                               " dbname=" + db_name +
                               " user=" + user +
                               " password=" + password +
                               " options=-csearch_path=" + schema;
        // valid template is:
        //std::string conn_str = "host=ITFORS1011 port=5432 dbname=logging user=app_user password=application-user options=-csearch_path=logging";
        // Initialize log-sink-db (automatically creates table 'mercurio' if missing)
        LogSinkDb logger("mercurio", "ITFORS1011", conn_str);
        for(int c=0; c<3;c++){
            // Record a trace log
            logger.trace("C/C++", "1", "main", "conn-str from PugiXml.");
        }
        logger.dispose();
        std::cout << "Connection string: " << conn_str << std::endl;
    }
}//

void agent_read_all_db() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("config.xml");

    if (!result) {
        std::cerr << "XML parse error: " << result.description() << std::endl;
        return;
    }

    // Iterate through each server instance inside the database block
    for (pugi::xml_node srv : doc.child("database").children("server")) {
        // Read the server name from the attribute
        std::string server_name = srv.attribute("name").value();

        // Read child elements relative to the current server node
        std::string db_model  = srv.child("db-model").child_value();
        std::string port      = srv.child("port").child_value();
        std::string db_name   = srv.child("db_name").child_value();
        std::string user      = srv.child("user").child_value();
        std::string password  = srv.child("password").child_value();
        std::string schema    = srv.child("schema").child_value();

        // Build a PostgreSQL connection string for libpqxx:
        std::string conn_str = "host=" + server_name +
                               " port=" + port +
                               " dbname=" + db_name +
                               " user=" + user +
                               " password=" + password +
                               " options=-csearch_path=" + schema;

        std::cout << "Loaded connection for server: " << conn_str << "\n";

        if(server_name=="Cantor" && db_model=="PostgreSql")
        {
            LogSinkDb logger("mercurio", "Cantor", conn_str);
            for(int c=0; c<3;c++){
                // Record a trace log
                logger.trace("C/C++", "1", "main", "conn-str from PugiXml.");
            }
            logger.dispose();
            std::cout << "Connection string: " << conn_str << std::endl;
        }// if
    }// for each xml-node
}// read all of <database> entries. agent_read-all-db


void agent_fetch_single_connection()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("config.xml");

    if (!result) {
        std::cerr << "XML parse error: " << result.description() << std::endl;
        return;
    }
    // Query directly using XPath to find the specific server node matching both criteria
    //pugi::xpath_node match = doc.select_node("//database/server[@name='Cantor' and db-model='PostgreSql']");
    pugi::xpath_node match = doc.select_node("//database/server[@name='ITFORS1011' and db-model='PostgreSql']");

    if (match) {// if Xpath results valid.
        pugi::xml_node srv = match.node();

        // Read attributes and child elements directly from the targeted node
        std::string server_name = srv.attribute("name").value();
        std::string db_model  = srv.child("db-model").child_value();
        std::string port      = srv.child("port").child_value();
        std::string db_name   = srv.child("db_name").child_value();
        std::string user      = srv.child("user").child_value();
        std::string password  = srv.child("password").child_value();
        std::string schema    = srv.child("schema").child_value();

        // Build the PostgreSQL connection string
        std::string conn_str = "host=" + server_name +
                               " port=" + port +
                               " dbname=" + db_name +
                               " user=" + user +
                               " password=" + password +
                               " options=-csearch_path=" + schema;

        std::cout << "Loaded connection for target server: " << conn_str << "\n";

        LogSinkDb logger("mercurio", server_name, conn_str);
        for (int c = 0; c < 3; c++) {
            logger.trace("C/C++", "1", "main", "PugiXml : specific connection via Xpath.");
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }
        logger.dispose();
        std::cout << "Connection string: " << conn_str << std::endl;
    }//if (match) {// if Xpath results valid.
     else {
        std::cerr << "Error: Specified server configuration not found in XML." << std::endl;
    }// else invalid Xpath
}//agent_fetch_single_connection()
