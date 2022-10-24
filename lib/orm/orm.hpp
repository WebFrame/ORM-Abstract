/** 
 *  @file   ORM.hpp 
 *  @brief  Single header containing all the necessary tools regarding the abstract ORM 
 *  @author Alex Tsvetanov
 *  @date   2022-09-09
 ***********************************************/

#pragma once
#include "ORM-version.hpp"
#include <string>
#include <list>
#include <variant>
#include <memory>
#include <regex>
#ifdef __cpp_lib_source_location
#include <source_location>
#endif
#include <string>
#include "std/string_view"

namespace ORM {
    class Dialect;

    template <typename DB_t>
    struct Database;

    class Table;

    struct conststr
    {
        const char* const p;
        template<std::size_t N>
        constexpr conststr(const char(&a)[N]) : p(a) {}
    };

    template<typename, size_t>
    class _Column;

#ifdef __cpp_lib_source_location
    #define Column(T) _Column<T, __ORM_LOCATION>
#else
    #define Column(T) _Column<T, __ORM_LOCATION>
#endif

    class DBConnection;

    template<typename RowT>
    class DBExecutor;

    template<typename DB_t, typename RowT, typename... Columns>
    class _Query;

    template <typename... Columns>
    class Result;

    template<typename DB_t, typename RowT, typename... Columns>
    using NonTransactionalQuery = _Query<DB_t, RowT, Columns...>;

    template<typename DB_t, typename... Columns>
    using TransactionalQuery = _Query<DB_t, void, Columns...>;

    class Page;

    template<typename DB_t, typename RowT, typename... Columns>
    class _PagedQuery;

    template<typename RowT, typename... Columns, typename DB_t>
    constexpr _Query<DB_t, RowT, Columns...> Query(const DB_t& DB, const std::string_view& query);

    template<typename RowT, typename... Columns, typename DB_t>
    constexpr _PagedQuery<DB_t, RowT, Columns...> PagedQuery(const DB_t& DB, const std::string_view& query);
}

#include "column/init.hpp"
#include "result/init.hpp"
#include "dialects/init.hpp"
#include "connections/init.hpp"
#include "database/init.hpp"
#include "table/init.hpp"
#include "query/init.hpp"