/** 
 *  @file   init.hpp 
 *  @brief  Query abstract definition for the ORM 
 *  @author Alex Tsvetanov
 *  @date   2022-09-09
 ***********************************************/

#pragma once
#include <orm/orm.hpp>
#include <list>
#include <iostream>
#include <type_traits>

namespace ORM {
    template<typename TableT,
             typename RowT, Column... Columns>
    /* abstract */ class Query {
        std::string_view _query;
    protected:        
        std::string query() const { return _query.data(); }
        
        std::list<RowT> execute(typename decltype(Columns)::type... args) const {
            auto exec = TableT::template get_executor<RowT>();
            return exec->execute(
                exec->get_dialect()->get_query(this->query(), args...)
            );
        }
    public:
        constexpr Query(const std::string_view& query_prepare_statement) : _query(query_prepare_statement) {
            //static_assert(std::is_base_of<Table, TableT>::value, "Query should be applied onto class derived from Table.");
        }

        std::list<RowT> operator()(typename decltype(Columns)::type... args) const {
            return this->execute(args...);
        }
    };

    template<typename TableT,
             Column... Columns>
    /* abstract */ class Query<TableT, void, Columns...> {
        std::string_view _query;
    protected:
        std::string query() const { return _query.data(); }
        
        void execute(typename decltype(Columns)::type... args) const {
            auto exec = TableT::template get_executor<void>();
            exec->execute(
                exec->get_dialect()->get_query(this->query(), args...)
            );
        }
    public:
        constexpr Query(const std::string_view& query_prepare_statement) : _query(query_prepare_statement) {
           //static_assert(std::is_base_of<Table, TableT>::value, "Query should be applied onto class derived from Table.");
        }
        
        void operator()(typename decltype(Columns)::type... args) const {
            this->execute(args...);
        }
    };
    
    template <Column... vars>
    using Result = std::tuple<typename decltype(vars)::type...>;

    template<typename TableT, typename RowT, Column... ArgsTs>
    using NonTransactionalQuery = Query<TableT, RowT, ArgsTs...>;
    
    template<typename TableT, Column... ArgsTs>
    using TransactionalQuery = Query<TableT, void, ArgsTs...>;
};
