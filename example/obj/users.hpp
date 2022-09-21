#pragma once
#include <orm/orm.hpp>
#include <orm/mysql.hpp>

#include <string>

class Users : public ORM::Table {
public:
    static constexpr auto id = ORM::Column<int>();
    static constexpr auto username = ORM::Column<std::string>();
    
    using Entity = ORM::Result<id, username>;

    static constexpr auto insertUser          = ORM::     Query<Users, void           , id, username>("INSERT INTO Users VALUES (?, ?)");
    static constexpr auto selectUser          = ORM::     Query<Users, ORM::Result<id>, username    >("SELECT Users.id FROM Users WHERE Users.username=?");
    static constexpr auto selectAllUsers      = ORM::     Query<Users, Entity                       >("SELECT Users.id, Users.username FROM Users");
    static constexpr auto pagedSelectAllUsers = ORM::PagedQuery<Users, Entity                       >("SELECT Users.id, Users.username FROM Users");

public:
    static std::shared_ptr<ORM::MySQL> get_native_conn() {
        static const std::string connection_string = "connection string";
        static const auto conn = std::make_shared<ORM::MySQL>(connection_string);
        return conn;
    }
    
    template<typename RowT>
    static std::shared_ptr<ORM::DBExecutor<RowT>> get_executor() {
        static const auto exec = std::static_pointer_cast<ORM::DBExecutor<RowT>>(std::make_shared<ORM::MySQLExecutor<RowT>>(get_native_conn()));
        return exec;
    }
};
