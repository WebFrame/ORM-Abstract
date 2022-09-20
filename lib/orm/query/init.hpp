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

namespace ORM {
    template<typename RowT, Column... Columns>
    /* abstract */ class Query {
        std::string_view _query;
        std::function<std::shared_ptr<DBExecutor<RowT>>()> get_conn;
        
        virtual std::string query() const { return _query.data(); }
        
        std::list<RowT> execute(typename decltype(Columns)::type... args) const {
            return this->get_conn()->execute(
                this->get_conn()->get_dialect()->get_query(this->query(), args...)
            );
        }
    public:
        constexpr Query(std::function<std::shared_ptr<DBExecutor<RowT>>()> conn, const std::string_view& query_prepare_statement) : _query(query_prepare_statement), get_conn(conn) {
        }

        std::list<RowT> operator()(typename decltype(Columns)::type... args) const {
            return this->execute(args...);
        }
    };

    template<Column... Columns>
    /* abstract */ class Query<void, Columns...> {
        std::string_view _query;
        std::function<std::shared_ptr<DBExecutor<void>>()> get_conn;
        
        virtual std::string query() const { return _query.data(); }
        
        void execute(typename decltype(Columns)::type... args) const {
            this->get_conn()->execute(
                this->get_conn()->get_dialect()->get_query(this->query(), args...)
            );
        }
    public:
        Query(std::function<std::shared_ptr<DBExecutor<void>>()> conn, const std::string_view& query_prepare_statement) : _query(query_prepare_statement), get_conn(conn) {
        }
        
        void operator()(typename decltype(Columns)::type... args) const {
            this->execute(args...);
        }
    };
    
    template <Column... vars>
    using Result = std::tuple<typename decltype(vars)::type...>;

    template<typename RowT, Column... ArgsTs>
    using NonTransactionalQuery = Query<RowT, ArgsTs...>;
    
    template<Column... ArgsTs>
    using TransactionalQuery = Query<void, ArgsTs...>;
};
