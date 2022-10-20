#pragma once
#include <ORM/ORM.hpp>
#include <string>

#ifdef NO_MYSQL
#include <ORM/database/MySQL/mysql.hpp>

constexpr const auto DB = ORM::Database<ORM::MySQL>({ "localhost", "root", "test", "DB", 3306, nullptr, 0 });
#else
#include <ORM/database/MockSQL/MockSQL.hpp>

constexpr const auto DB = ORM::Database<ORM::MockSQL>({ "connection string" });
#endif

class Users : ORM::Table {
public:
    using id = ORM::Column(int);
    using username = ORM::Column(std::string);
    using test = ORM::Column(int);
    
    using Entity = ORM::Result<id, username>;
    
    static constexpr auto createTable         = DB.     Query<void                         >("CREATE TABLE IF NOT EXISTS Users (id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY, username VARCHAR(1024) NOT NULL);");
    static constexpr auto insertUser          = DB.     Query<void           , username    >("INSERT INTO Users (username) VALUES (?)");
    static constexpr auto selectUser          = DB.     Query<ORM::Result<id>, username    >("SELECT Users.id FROM Users WHERE Users.username=?");
    static constexpr auto selectAllUsers      = DB.     Query<Entity                       >("SELECT Users.id, Users.username FROM Users");
    static constexpr auto pagedSelectAllUsers = DB.PagedQuery<Entity                       >("SELECT Users.id, Users.username FROM Users");
};