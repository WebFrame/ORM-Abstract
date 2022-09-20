/** 
 *  @file   init.hpp 
 *  @brief  DB connection abstract definition for the ORM 
 *  @author Alex Tsvetanov
 *  @date   2022-09-09
 ***********************************************/

#pragma once
#include <orm/orm.hpp>
#include <string>
#include <iostream>

namespace ORM {
    /* abstract */ class DBConnection {
    protected:
        virtual void init_conn(const std::string& conn) = 0;
    public:
        DBConnection() {}

        virtual std::shared_ptr<Dialect> get_dialect() const = 0;
        
        virtual void commit() = 0;
        
        virtual void rollback() = 0;
    };

    template<typename RowT>
    /* abstract */ class DBExecutor : public DBConnection {
    public:
        virtual std::list<RowT> execute(const std::string&) = 0;
    };

    template<>
    /* abstract */ class DBExecutor<void> : public DBConnection { 
    public:   
        virtual void execute(const std::string&) = 0;
    };
}