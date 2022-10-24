#pragma once
#include <ORM/ORM.hpp>
#include <string>

#ifdef USE_MYSQL
#include <ORM/database/MySQL/mysql.hpp>

constexpr const auto DB = webframe::ORM::Database<webframe::ORM::MySQL>("localhost", DB_USERNAME, DB_PASSWORD, "DB", 3306, nullptr, 0, false);
#else
#include <ORM/database/MockSQL/MockSQL.hpp>

constexpr const auto DB = webframe::ORM::Database<webframe::ORM::MockSQL>("connection string");
#endif

class Users : webframe::ORM::Table {
public:
    using id = webframe::ORM::Column(int);
    using username = webframe::ORM::Column(std::string);
    
    using Entity = webframe::ORM::Result<id, username>;
    
    static constexpr auto createTable         = DB.     Query<void                                         >("CREATE TABLE IF NOT EXISTS Users (id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY, username VARCHAR(1024) NOT NULL);");
    static constexpr auto insertUser          = DB.     Query<void                               , username>("INSERT INTO Users (username) VALUES (?)");
    static constexpr auto selectUser          = DB.     Query<id                                 , username>("SELECT Users.id FROM Users WHERE Users.username=?");
    static constexpr auto selectAllUsers      = DB.     Query<webframe::ORM::Result<id, username>          >("SELECT Users.id, Users.username FROM Users");
    static constexpr auto pagedSelectAllUsers = DB.PagedQuery<Entity                                       >("SELECT Users.id, Users.username FROM Users");
};