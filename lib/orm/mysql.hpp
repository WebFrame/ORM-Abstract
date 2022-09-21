#pragma once
#include <orm/orm.hpp>
#include <utility>
#include <iostream>

//#include <mysql_connection.h>
//#include <cppconn/driver.h>
//#include <cppconn/exception.h>
//#include <cppconn/resultset.h>
//#include <cppconn/statement.h>

namespace ORM {
    template<typename RowT>
    class MySQLExecutor;
    class MySQL;

    class MySQL : public DBConnection
    {
    protected:
        class MySQLDialect_t;
        //std::unique_ptr<sql::mysql::MySQL_Driver> driver;
        //std::unique_ptr<sql::Connection> con;

        template<typename ResultSet, typename T, size_t ind>
        inline T get_column(ResultSet *obj) {
            if constexpr (std::is_same_v<T, int8_t>)           return obj->getInt(ind + 1);
            if constexpr (std::is_same_v<T, uint8_t>)          return obj->getUInt(ind + 1);
            if constexpr (std::is_same_v<T, int16_t>)          return obj->getInt(ind + 1);
            if constexpr (std::is_same_v<T, uint16_t>)         return obj->getUInt(ind + 1);
            if constexpr (std::is_same_v<T, int32_t>)          return obj->getInt(ind + 1);
            if constexpr (std::is_same_v<T, uint32_t>)         return obj->getUInt(ind + 1);
            if constexpr (std::is_same_v<T, int64_t>)          return obj->getInt(ind + 1);
            if constexpr (std::is_same_v<T, uint64_t>)         return obj->getUInt(ind + 1);
            if constexpr (std::is_same_v<T, float>)            return obj->getFloat(ind + 1);
            if constexpr (std::is_same_v<T, double>)           return obj->getDouble(ind + 1);
            if constexpr (std::is_same_v<T, char*>)            return obj->getCharSet(ind + 1).c_str();
            if constexpr (std::is_same_v<T, char>)             return obj->getCharSet(ind + 1)[0];
            if constexpr (std::is_same_v<T, std::string>)      return obj->getString(ind + 1);
            if constexpr (std::is_same_v<T, std::string_view>) return obj->getString(ind + 1);
            throw std::invalid_argument("Unknown Type in ResultSet.");
        }

        template <typename ResultSet, typename... RTs, size_t ... inds>
        inline std::tuple<RTs...> make_item(ResultSet *res, std::index_sequence<inds ...> _ = std::index_sequence_for<RTs...>()){
            return make_tuple(get_column<RTs, inds>(res)...);
        } 

        template <typename ResultSet, typename... RTs>
        inline std::tuple<RTs...> make_item(ResultSet *res){
            return make_item<RTs...>(res, std::index_sequence_for<RTs...>());
        }
        
        void init_conn(const std::string& conn) {
            std::cout << conn << std::endl;
            //con = driver->connect(conn.c_str(), user, password);
        }
    public:
        MySQL(const std::string& conn) {
            init_conn(conn);
        }

        ~MySQL() {
            
        }
        
        void commit() {
            //con->commit();
        }
        
        void rollback() {
            //con->rollback();
        }
        
        std::shared_ptr<Dialect> get_dialect() const {
            static std::shared_ptr<Dialect> mysqlDialect = std::static_pointer_cast<Dialect>(std::make_shared<MySQLDialect>());
            return mysqlDialect;
        }
    private:
        class MySQLDialect : public Dialect 
        {
            std::string escape(const std::string& value) const {
                return this->escape(value.c_str());
            }
            std::string escape(const char* value) const { // https://dev.mysql.com/doc/refman/8.0/en/string-functions.html#function_quote
                std::stringstream ss;
                const size_t size = strlen(value);
                for(size_t i = 0 ; i < size ; i ++) {
                    switch(value[i]) {
                        case '\b':
                            break;
                        case '\n':
                            break;
                        case '\r':
                            break;
                        case '\t':
                            break;
                        case 26: // CTRL+Z
                            break;
                        case '\'':
                        case '\\':
                            ss << "\\" << value[i]; break;
                        default:
                            ss << value[i]; break;
                    }
                }
                return ss.str();
            }
            std::string escape(const std::string_view& value) const {
                return this->escape(std::string(value).c_str());
            }
        public:
            std::string parse(char value) const {
                std::stringstream ss;
                ss << this->escape(std::string() + value);
                return ss.str();
            }
            std::string parse(const char* value) const {
                std::stringstream ss;
                ss << "'" << this->escape(value) << "'";
                return ss.str();
            }
            std::string parse(const std::string& value) const {
                return std::string("'") + this->escape(value) + std::string("'");
            }
            std::string parse(const std::string_view& value) const {
                return std::string("'") + this->escape(value) + std::string("'");
            }
            std::regex get_var_regex() const {
                return std::regex("\\?");
            }
            
            Page get_pagification() const {
                static const Page pagification = Page("LIMIT %zu, %zu");
                return pagification;
            }
        public:
        };

        template<typename RowT>
        friend class MySQLExecutor;
    };
    
    template<typename RowT>
    class MySQLExecutor : public DBExecutor<RowT> {
    protected:
        std::shared_ptr<MySQL> conn;
        void init_conn(const std::string& connection_str) { conn->init_conn(connection_str); }
    public:
    
        std::shared_ptr<Dialect> get_dialect() const { return conn->get_dialect(); }
        
        void commit() { conn->commit(); }
        
        void rollback() { conn->rollback(); }

        MySQLExecutor(std::shared_ptr<MySQL> connection) {
            conn = connection;
        }

        std::list<RowT> execute(const std::string& query) { // non-transactional
            std::cout << "MySQL::execute: " << query << std::endl;
            std::list<RowT> answer;
            
            //sql::Statement *stmt;
            //sql::ResultSet *res;
            //stmt = con->createStatement();
            //res = stmt->executeQuery(query.c_str());
            //while (res->next()) {
            //    answer.push_back(make_item<RTs...>(res));
            //}

            //delete stmt;
            //delete res;
            
            return answer;
        }
    };

    template<>
    class MySQLExecutor<void> : public DBExecutor<void> {
    protected:
        std::shared_ptr<MySQL> conn;
        void init_conn(const std::string& connection_str) { conn->init_conn(connection_str); }
    public:
    
        std::shared_ptr<Dialect> get_dialect() const { return conn->get_dialect(); }
        
        void commit() { conn->commit(); }
        
        void rollback() { conn->rollback(); }

        MySQLExecutor(std::shared_ptr<MySQL> connection) {
            conn = connection;
        }
        
        void execute(const std::string& query) { // transactional
            std::cout << "MySQL::execute: " << query << std::endl;
            
            //sql::Statement *stmt;
            //sql::ResultSet *res;
            //stmt = con->createStatement();
            //res = stmt->executeQuery(query.c_str());
            //delete stmt;
            //delete res;
            return;
        }
    };
}