#pragma once
#include "../ConnectionProvider/ConnectionProviderPostgres.h"
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>

class LogSinkDb {
private:
    ConnectionProviderPostgres provider;
    std::string log_table_name;

    // Helper to generate current local time in "YYYY-MM-DD HH:MM:SS" format
    std::string get_current_timestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm buf;
        localtime_r(&in_time_t, &buf); // Thread-safe local time on Linux

        std::ostringstream ss;
        ss << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

public:
    // Constructor: initializes connection and ensures log table exists via logging.create_log_table
    LogSinkDb(const std::string& table_name, const std::string& server_name, const std::string& conn_string)
        : provider(server_name, conn_string), log_table_name(table_name) {

        std::cout << "Database connection initialized for table: " << log_table_name << "\n";

        pqxx::connection* conn = provider.get_connection();
        if (!conn) {
            throw std::runtime_error("Failed to obtain active connection for table creation.");
        }

        try {
            pqxx::work txn(*conn);
            // Call the database function securely using parameters
            txn.exec("SELECT logging.create_log_table($1)", pqxx::params{log_table_name});
            txn.commit();
            std::cout << "-> Checked/Created log table: " << log_table_name << "\n";
        } catch (const std::exception& e) {
            throw std::runtime_error(std::string("Failed to create log table: ") + e.what());
        }
    }

    // Trace method optimized for numerical analysis loops (matches Rust signature)
    void trace(const std::string& row_nature,
               const std::string& stack_depth,
               const std::string& function_name,
               const std::string& log_content) {

        pqxx::connection* conn = provider.get_connection();
        if (!conn || !conn->is_open()) {
            std::cerr << "Failed to write trace log: Connection is closed or unavailable.\n";
            return;
        }

        try {
            std::string current_formatted_date = get_current_timestamp();
            pqxx::work txn(*conn);

            // Calls logging.trace with 6 parameters safely
            txn.exec_params(
                "SELECT logging.trace($1, $2, $3, $4, $5, $6)",
                log_table_name,
                current_formatted_date,
                row_nature,
                stack_depth,
                function_name,
                log_content
            );
            txn.commit();
        } catch (const std::exception& e) {
            std::cerr << "Error writing trace: " << e.what() << "\n";
        }
    }

    // Explicit dispose method
    void dispose() {
        provider.close_connection();
    }

    // Destructor to ensure clean resource close
    ~LogSinkDb() {
        try {
            dispose();
        } catch (...) {}
    }
};
