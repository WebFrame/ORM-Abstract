#pragma once
#include "orm.hpp"
#include <utility>
#include <iostream>

#include <mysql.h>

namespace ORM
{
    template <typename RowT>
    class MySQLExecutor;
    class MySQL;

    class MySQL : public DBConnection
    {
    protected:
        class MySQLDialect_t;
        std::unique_ptr<MYSQL> con;

        template <typename T, size_t ind>
        inline T get_column(MYSQL_ROW& obj, const MYSQL_FIELD* fields)
        {
            T answer;
            std::istringstream in (obj[ind]);
            if constexpr (std::is_integral_v<T> || std::is_same_v<T, bool>) {
                switch(fields[ind].type) {
                    case MYSQL_TYPE_TINY:
                    case MYSQL_TYPE_SHORT:
                    case MYSQL_TYPE_LONG:
                    case MYSQL_TYPE_INT24:
                    case MYSQL_TYPE_LONGLONG:
                    case MYSQL_TYPE_DECIMAL:
                    case MYSQL_TYPE_NEWDECIMAL:
                    case MYSQL_TYPE_FLOAT:
                    case MYSQL_TYPE_DOUBLE:
                    case MYSQL_TYPE_BIT:
                        in >> answer;
                        return answer;
                    default:
                        throw "";
                }
            }
            else {
                switch(fields[ind].type) {
                    case MYSQL_TYPE_TINY:
                    case MYSQL_TYPE_SHORT:
                    case MYSQL_TYPE_LONG:
                    case MYSQL_TYPE_INT24:
                    case MYSQL_TYPE_LONGLONG:
                    case MYSQL_TYPE_DECIMAL:
                    case MYSQL_TYPE_NEWDECIMAL:
                    case MYSQL_TYPE_FLOAT:
                    case MYSQL_TYPE_DOUBLE:
                    case MYSQL_TYPE_BIT:
                        in >> answer;
                        return answer;
                    case MYSQL_TYPE_TIMESTAMP:
                    case MYSQL_TYPE_DATE:
                    case MYSQL_TYPE_TIME:
                    case MYSQL_TYPE_DATETIME:
                    case MYSQL_TYPE_YEAR:
                        return T(obj[ind]);
                    case MYSQL_TYPE_STRING:
                    case MYSQL_TYPE_VAR_STRING:
                    case MYSQL_TYPE_BLOB:
                        return T(obj[ind]);
                    case MYSQL_TYPE_SET:
                    case MYSQL_TYPE_ENUM:
                    case MYSQL_TYPE_GEOMETRY:
                    case MYSQL_TYPE_NULL:
                        return T(obj[ind]);
                    default:
                        throw std::invalid_argument("Unknown Type in ResultSet.");
                }
            }
        }

        template <typename RowT, size_t... inds>
        inline RowT make_item(MYSQL_ROW& res, const MYSQL_FIELD* fields, std::index_sequence<inds...>)
        {
            return std::make_tuple(get_column<typename std::tuple_element<inds, RowT>::type, inds>(res, fields)...);
        }

        template <typename RowT>
        inline RowT make_item(MYSQL_ROW& res, const MYSQL_FIELD* fields)
        {
            return make_item<RowT>(res, fields, std::make_index_sequence<std::tuple_size<RowT>::value>{});
        }

        void init_conn(__attribute__((unused)) const std::string &conn)
        {
            if (mysql_real_connect(con.get(), "localhost", "root", "root_passwd", NULL, 0, NULL, 0) == NULL)
            {
                fprintf(stderr, "%s\n", mysql_error(con.get()));
                mysql_close(con.get());
                throw "Connection failed.";
            }

        }

    public:
        MySQL(const std::string &conn)
        {
            init_conn(conn);
        }

        ~MySQL()
        {
        }

        void commit()
        {
            mysql_commit(con.get());
        }

        void rollback()
        {
            mysql_rollback(con.get());
        }

        std::shared_ptr<Dialect> get_dialect() const
        {
            static std::shared_ptr<Dialect> mysqlDialect = std::static_pointer_cast<Dialect>(std::make_shared<MySQLDialect>());
            return mysqlDialect;
        }

        MYSQL* get() {
            return con.get();
        }

    private:
        class MySQLDialect : public Dialect
        {
            std::string escape(const std::string &value) const
            {
                return this->escape(value.c_str());
            }
            std::string escape(const char *value) const
            { // https://dev.mysql.com/doc/refman/8.0/en/string-functions.html#function_quote
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
        friend class MySQLExecutor;
    };

    template <typename RowT>
    class MySQLExecutor : public DBExecutor<RowT>
    {
    protected:
        std::shared_ptr<MySQL> conn;
        void init_conn(const std::string &connection_str) { conn->init_conn(connection_str); }

    public:
        std::shared_ptr<Dialect> get_dialect() const { return conn->get_dialect(); }

        void commit() { conn->commit(); }

        void rollback() { conn->rollback(); }

        MySQLExecutor(std::shared_ptr<MySQL> connection)
        {
            conn = connection;
        }

        std::list<RowT> execute(const std::string &query)
        { // non-transactional
            std::cout << "MySQL::execute: " << query << std::endl;
            std::list<RowT> answer;

            MYSQL_RES *result = mysql_store_result(conn->get());
            if (result == NULL)
            {
                throw "Result was not generated.";
            }
            
            MYSQL_ROW row;
            //const int num_fields = mysql_num_fields(result);
            const MYSQL_FIELD* fields = mysql_fetch_fields(result);

            while ((row = mysql_fetch_row(result)))
            {
                answer.push_back(conn->make_item<RowT>(row, fields));
            }

            mysql_free_result(result);

            return answer;
        }
    };

    template <>
    class MySQLExecutor<void> : public DBExecutor<void>
    {
    protected:
        std::shared_ptr<MySQL> conn;
        void init_conn(const std::string &connection_str) { conn->init_conn(connection_str); }

    public:
        std::shared_ptr<Dialect> get_dialect() const { return conn->get_dialect(); }

        void commit() { conn->commit(); }

        void rollback() { conn->rollback(); }

        MySQLExecutor(std::shared_ptr<MySQL> connection)
        {
            conn = connection;
        }

        void execute(const std::string &query)
        { // transactional
            std::cout << "MySQL::execute: " << query << std::endl;
            if (mysql_query(conn->get(), query.c_str())) {
                throw std::string("Query ") + query + " was not successful.";
            }
            return;
        }
    };
}