#pragma once
#include <string>
#include <memory>
#include <pqxx/pqxx>
#include <iostream>

class ConnectionProviderPostgres {
private:
    std::unique_ptr<pqxx::connection> connection;
    std::string server_name;

public:
    // Constructor matching the behavior of new()
    ConnectionProviderPostgres(const std::string& server_name, const std::string& conn_string) 
        : server_name(server_name) {
        try {
            connection = std::make_unique<pqxx::connection>(conn_string);
            if (connection->is_open()) {
                std::cout << "Connection to database " << server_name << " opened successfully\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Failed to connect to " << server_name << ": " << e.what() << "\n";
            throw;
        }
    }

    pqxx::connection* get_connection() {
        if (connection && connection->is_open()) {
            return connection.get();
        }
        return nullptr;
    }

    void close_connection() {
        if (connection && connection->is_open()) {
            connection->close();
            std::cout << "Connection to database " << server_name << " closed successfully\n";
        }
    }
};
