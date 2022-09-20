#pragma once
#include <orm/orm.hpp>
#include <orm/mysql.hpp>

#include <string>

class Users : public ORM::Table {
    static constexpr auto id = ORM::Column<int>();
    static constexpr auto username = ORM::Column<std::string>();
public:
    using Entity = ORM::Result<id, username>;

    static const ORM::Query<void           , id, username> insertUser;
    static const ORM::Query<ORM::Result<id>, username    > selectUser;
    static const ORM::Query<Entity                       > selectAllUsers;

private:
    template<typename RowT>
    static std::shared_ptr<ORM::DBExecutor<RowT>> get_conn() {
        static const std::string connection_string = "connection string";
        static const auto conn = std::static_pointer_cast<ORM::DBExecutor<RowT>>(std::make_shared<ORM::MySQLExecutor<RowT>>(connection_string));
        return conn;
    }
};

// auto Users::insertUser = ORM::TransactionalQuery<Users::id, Users::username> (Users::get_conn, "INSERT INTO Users VALUES (?, ?);");
// auto Users::selectUser = ORM::NonTransactionalQuery<ORM::Result<Users::id>, Users::username> (Users::get_conn, "SELECT Users.id FROM Users WHERE Users.username=?;");
// auto Users::selectAllUsers = ORM::NonTransactionalQuery<ORM::Result<Users::User>> (Users::get_conn, "SELECT Users.id, Users.username FROM Users;");
const ORM::Query<void                  , Users::id, Users::username> Users::insertUser     = ORM::Query<void                      , Users::id, Users::username> (Users::get_conn<void>, "INSERT INTO Users VALUES (?, ?);");
const ORM::Query<ORM::Result<Users::id>, Users::username           > Users::selectUser     = ORM::Query<ORM::Result<Users::id>    , Users::username           > (Users::get_conn<ORM::Result<Users::id>>, "SELECT Users.id FROM Users WHERE Users.username=?;");
const ORM::Query<Users::Entity                                     > Users::selectAllUsers = ORM::Query<Users::Entity                                         > (Users::get_conn<Users::Entity>, "SELECT Users.id, Users.username FROM Users;");
