/** 
 *  @file   init.hpp 
 *  @brief  DB connection abstract definition for the ORM 
 *  @author Alex Tsvetanov
 *  @date   2022-09-09
 ***********************************************/

#pragma once
#include "../ORM.hpp"
#include <string>
#include <iostream>

namespace ORM {
    /* abstract */ class DBConnection {
    public:
        DBConnection() {}
        virtual ~DBConnection() = default;

        virtual std::shared_ptr<Dialect> get_dialect() const = 0;
        
        virtual void commit() = 0;
        
        virtual void rollback() = 0;
    };

    template<typename RowT>
    /* abstract */ class DBExecutor : public DBConnection {
    public:
        virtual std::list<RowT> execute(const std::string&) = 0;
        virtual ~DBExecutor() = default;
    };

    template<>
    /* abstract */ class DBExecutor<void> : public DBConnection { 
    public:   
        virtual void execute(const std::string&) = 0;
        virtual ~DBExecutor() = default;
    };
}