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
    
    template<typename RowT, Column... Columns>
    class Query;
}

#include <orm/connections/init.hpp>
#include <orm/dialects/init.hpp>
#include <orm/query/init.hpp>
#include <orm/table/init.hpp>
#include <orm/column/init.hpp>