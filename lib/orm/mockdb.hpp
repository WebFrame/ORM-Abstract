#pragma once
#include "orm.hpp"
#include <utility>
#include <iostream>

namespace ORM
{
    template <typename RowT>
    class Mock_MySQLExecutor;
    class Mock_MySQL;

    class Mock_MySQL : public DBConnection
    {
    private:
        class Mock_MySQLDialect;
    protected:
        void init_conn(const std::string &conn)
        {
            std::cout << "Connection string: " << conn << std::endl;
        }

    public:
        Mock_MySQL(const std::string &conn)
        {
            init_conn(conn);
        }

        ~Mock_MySQL()
        {
        }

        void commit()
        {
            std::cout << "Commit" << std::endl;
        }

        void rollback()
        {
            std::cout << "Rollback" << std::endl;
        }

        std::shared_ptr<Dialect> get_dialect() const
        {
            static std::shared_ptr<Dialect> MySQLDialect = std::static_pointer_cast<Dialect>(std::make_shared<Mock_MySQLDialect>());
            return MySQLDialect;
        }

    private:
        class Mock_MySQLDialect : public Dialect
        {
            std::string escape(const std::string &value) const
            {
                return this->escape(value.c_str());
            }
            std::string escape(const char *value) const
            { // https://dev.Mock_MySQL.com/doc/refman/8.0/en/string-functions.html#function_quote
                std::stringstream ss;
                const size_t size = strlen(value);
                for (size_t i = 0; i < size; i++)
                {
                    switch (value[i])
                    {
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
                        ss << "\\" << value[i];
                        break;
                    default:
                        ss << value[i];
                        break;
                    }
                }
                return ss.str();
            }
            std::string escape(const std::string_view &value) const
            {
                return this->escape(std::string(value).c_str());
            }

        public:
            std::string parse(char value) const
            {
                std::stringstream ss;
                ss << this->escape(std::string() + value);
                return ss.str();
            }
            std::string parse(const char *value) const
            {
                std::stringstream ss;
                ss << "'" << this->escape(value) << "'";
                return ss.str();
            }
            std::string parse(const std::string &value) const
            {
                return std::string("'") + this->escape(value) + std::string("'");
            }
            std::string parse(const std::string_view &value) const
            {
                return std::string("'") + this->escape(value) + std::string("'");
            }
            std::regex get_var_regex() const
            {
                return std::regex("\\?");
            }

            Page get_pagification() const
            {
                static const Page pagification = Page("LIMIT %zu, %zu");
                return pagification;
            }

        public:
        };

        template <typename RowT>
        friend class Mock_MySQLExecutor;
    };

    template <typename RowT>
    class Mock_MySQLExecutor : public DBExecutor<RowT>
    {
    protected:
        std::shared_ptr<Mock_MySQL> conn;
        void init_conn(const std::string &connection_str) { conn->init_conn(connection_str); }

    public:
        std::shared_ptr<Dialect> get_dialect() const { return conn->get_dialect(); }

        void commit() { conn->commit(); }

        void rollback() { conn->rollback(); }

        Mock_MySQLExecutor(std::shared_ptr<Mock_MySQL> connection)
        {
            conn = connection;
        }

        std::list<RowT> execute(const std::string &query)
        { // non-transactional
            std::cout << "Mock_MySQL::execute: " << query << std::endl;
            std::list<RowT> answer;
            return answer;
        }
    };

    template <>
    class Mock_MySQLExecutor<void> : public DBExecutor<void>
    {
    protected:
        std::shared_ptr<Mock_MySQL> conn;
        void init_conn(const std::string &connection_str) { conn->init_conn(connection_str); }

    public:
        std::shared_ptr<Dialect> get_dialect() const { return conn->get_dialect(); }

        void commit() { conn->commit(); }

        void rollback() { conn->rollback(); }

        Mock_MySQLExecutor(std::shared_ptr<Mock_MySQL> connection)
        {
            conn = connection;
        }

        void execute(const std::string &query)
        { // transactional
            std::cout << "Query: " << query << std::endl;
            return;
        }
    };
}
