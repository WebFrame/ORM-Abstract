/** 
 *  @file   orm.hpp 
 *  @brief  Single header containing all the necessary tools regarding the abstract ORM 
 *  @author Alex Tsvetanov
 *  @date   2022-09-09
 ***********************************************/

#pragma once
#include <string>
#include <list>
#include <variant>
#include <memory>
#include <regex>
#include <string>
#include <string_view>

namespace ORM {
    class Dialect;
    
    class Table;

    template<typename T>
    class Column;

    class DBConnection;
    
    template<typename RowT>
    class DBExecutor;
    
    template<typename TableT,
             typename RowT, Column... Columns>
    class Query;

    template <Column... vars>
    using Result = std::tuple<typename decltype(vars)::type...>;

    template<typename TableT, typename RowT, Column... ArgsTs>
    using NonTransactionalQuery = Query<TableT, RowT, ArgsTs...>;
    
    template<typename TableT, Column... ArgsTs>
    using TransactionalQuery = Query<TableT, void, ArgsTs...>;

    class Page;

    template<typename TableT,
             typename RowT, Column... Columns>
    class PagedQuery;
}

#include <orm/connections/init.hpp>
#include <orm/dialects/init.hpp>
#include <orm/query/init.hpp>
#include <orm/table/init.hpp>
#include <orm/column/init.hpp>