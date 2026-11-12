/*
No, you do not need to link an external compiled binary library (like a .a or .lib file) for PugiXML because it is a source-based library.

Instead of linking a pre-compiled library, you simply include its source files directly in your Code::Blocks project.

How to add PugiXML to Code::Blocks:
Download the source: Grab pugixml.hpp and pugixml.cpp from the official PugiXML website (or via MSYS2 if available via package manager, though dropping the two files into your project folder is the easiest way).

Add files to your project: Place pugixml.hpp and pugixml.cpp inside your Code::Blocks project directory (e.g., in a libs/pugixml/ folder).

Include in build target: In Code::Blocks, right-click pugixml.cpp in your project tree, select Properties, go to the Build tab, and ensure that Compile file and Link file are checked so it compiles alongside your own source files.

Header-only alternative: If you prefer not to add pugixml.cpp to your project, you can make it header-only by defining #define PUGIXML_HEADER_ONLY right before you include pugixml.hpp in your code.

Want me to show how to download and install PugiXML via MSYS2 pacman instead?
----
#######
-------
You can install PugiXML directly through your MSYS2 MinGW64 environment using `pacman`, just like you did for `libpqxx`. Because MSYS2 installs it into your standard MinGW64 directories, Code::Blocks will automatically find the headers if you already configured the base `include` path.

### Step 1: Install PugiXML via MSYS2 Pacman

Open your **MSYS2 MinGW64 terminal** and run the following command:

```bash
pacman -S mingw-w64-x86_64-pugixml

```

This places the header files (`pugixml.hpp`, `pugiconfig.hpp`) into `/mingw64/include/` and the compiled library into `/mingw64/lib/`.

---

### Step 2: Link PugiXML in Code::Blocks

Since you already added `C:\msys64\mingw64\include` to your project's **Compiler search directories** for `libpqxx`, Code::Blocks will automatically locate `<pugixml.hpp>` without extra include paths.

You only need to tell the linker to include the PugiXML library:

1. **Open Linker Settings:** Build options.
Right-click your project in Code::Blocks, select **Build options...**, and navigate to the **Linker settings** tab.
*Verification:* The Linker settings tab is open with your target/project selected.


2. **Add Linker Flag:** Add library option.
Under the **Other linker options** box, add:

```text
-lpugixml

```

Alternatively, you can click **Add** under *Link libraries* and browse to `C:\msys64\mingw64\lib\libpugixml.dll.a`. Click **OK**.
*Verification:* `-lpugixml` appears in the other linker options.


3. **Compile Your Project:** Rebuild.
Select **Build -> Rebuild** from the top menu to compile your code with PugiXML.
*Verification:* The project builds successfully with no missing symbol or library errors.

*/


#include <iostream>
#include <string>
#include "pugixml.hpp"
// dbg
#include "../LogSinkDb/LogSinkDb.h"

/*
<?xml version="1.0" encoding="utf-8"?>
<SettingsLocationTemplate>

  <database>
      <server>ITFORS1011</server>
      <port>logging</port>
      <db_name>logging</db_name>
      <user>app_user</user>
      <password>application-user</password>
      <schema>logging</schema>
  </database>

  <ID>MicrosoftNotepad6</ID>
  <Version>1</Version>
  <Processes>
    <Process>
      <Filename>NOTEPAD.EXE</Filename>
      <ProductVersion>
        <Major Minimum="6" Maximum="10" />
      </ProductVersion>
    </Process>
  </Processes>
  <Settings>
    <Registry>
      <Path>Software\Microsoft\Notepad</Path>
    </Registry>
  </Settings>

</SettingsLocationTemplate>

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
    pugi::xpath_node match = doc.select_node("//database/server[@name='Cantor' and db-model='PostgreSql']");

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
        }
        logger.dispose();
        std::cout << "Connection string: " << conn_str << std::endl;
    }//if (match) {// if Xpath results valid.
     else {
        std::cerr << "Error: Specified server configuration not found in XML." << std::endl;
    }// else invalid Xpath
}//agent_fetch_single_connection()
