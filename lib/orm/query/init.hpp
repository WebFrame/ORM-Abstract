/** 
 *  @file   init.hpp 
 *  @brief  _Query abstract definition for the ORM 
 *  @author Alex Tsvetanov
 *  @date   2022-09-09
 ***********************************************/

#pragma once
#include "../ORM.hpp"
#include <list>
#include <iostream>
#include <type_traits>
#include "../std/string_view"

namespace ORM {

    template<typename DB_t, typename RowT, typename... Columns>
    /* abstract */ class _Query {
    protected:
        std::string_view _query;
        const DB_t& DB;
    protected:        
        std::string query() const { return _query.data(); }
        
        std::list<RowT> execute(typename Columns::type... args) const {
            auto exec = DB.template get_executor<RowT>();
            return exec->execute(
                exec->get_dialect()->get_query(this->query(), args...)
            );
        }
    public:
        constexpr _Query(const DB_t& _db, const std::string_view& query_prepare_statement) : _query(query_prepare_statement), DB(_db) {
            static_assert(std::is_base_of<DBConnection, typename DB_t::Connection>::value, "_Query should use class derived from DBConnection.");
            static_assert(std::is_base_of<DBExecutor<RowT>, typename DB_t::Connection::template Executor<RowT>>::value, "_Query should use executor class derived from DBExecutor.");
        }

        std::list<RowT> operator()(typename Columns::type... args) const {
            return this->execute(args...);
        }
    };

    template<typename DB_t, typename... Columns>
    /* abstract */ class _Query<DB_t, void, Columns...> {
    protected:
        const DB_t& DB;
        std::string_view _query;
    protected:
        std::string query() const { return _query.data(); }
        
        void execute(typename Columns::type... args) const {
            auto exec = DB.template get_executor<void>();
            exec->execute(
                exec->get_dialect()->get_query(this->query(), args...)
            );
        }
    public:
        constexpr _Query(const DB_t& _db, const std::string_view& query_prepare_statement) : DB(_db) {
            static_assert(std::is_base_of<DBConnection, typename DB_t::Connection>::value, "_Query should use class derived from DBConnection.");
            static_assert(std::is_base_of<DBExecutor<void>, typename DB_t::Connection::template Executor<void>>::value, "_Query should use executor class derived from DBExecutor.");
            _query = query_prepare_statement;
        }
        
        void operator()(typename Columns::type... args) const {
            this->execute(args...);
        }
    };

    template<typename DB_t, typename RowT, typename... Columns>
    class _PagedQuery : public _Query<DB_t, RowT, Columns...> {
    public:
        constexpr _PagedQuery(const DB_t& _db, const std::string_view& query_prepare_statement) : _Query<DB_t, RowT, Columns...>(_db, query_prepare_statement) {
        }

        std::list<RowT> execute(size_t ItemsPerPage, std::size_t page, typename Columns::type... args) const {
            auto exec = this->DB.template get_executor<RowT>();
            return exec->execute(
                exec->get_dialect()->get_query(
                    this->query() + " " + exec->get_dialect()->get_pagification().get_page(ItemsPerPage, page),
                    args...
                )
            );
        }

        std::list<RowT> operator()(size_t ItemsPerPage, std::size_t page, typename Columns::type... args) const {
            return this->execute(ItemsPerPage, page, args...);
        }
    };

    template<typename DB_t, typename... Columns>
    class _PagedQuery<DB_t, void, Columns...> : public _Query<DB_t, void, Columns...> {
    public:
        constexpr _PagedQuery(const DB_t& _db, const std::string_view& query_prepare_statement) : _Query<DB_t, void, Columns...>(_db, query_prepare_statement) {
        }

        void execute(size_t ItemsPerPage, std::size_t page, typename Columns::type... args) const {
            auto exec = this->DB.template get_executor<void>();
            exec->execute(
                exec->get_dialect()->get_query(
                    this->query() + " " + exec->get_dialect()->get_pagification().get_page(ItemsPerPage, page),
                    args...
                )
            );
        }

        void operator()(size_t ItemsPerPage, std::size_t page, typename Columns::type... args) const {
            this->execute(ItemsPerPage, page, args...);
        }
    };

    template<typename RowT, typename... Columns, typename DB_t>
    constexpr _Query<DB_t, RowT, Columns...> Query(const DB_t& DB, const std::string_view& query) {
        return _Query<DB_t, RowT, Columns...>(DB, query);
    }

    template<typename RowT, typename... Columns, typename DB_t>
    constexpr _PagedQuery<DB_t, RowT, Columns...> PagedQuery(const DB_t& DB, const std::string_view& query) {
        return _PagedQuery<DB_t, RowT, Columns...>(DB, query);
    }
}