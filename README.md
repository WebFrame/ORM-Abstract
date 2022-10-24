# Readme
# <span style="font-size: 2.25em;">ORM Library for C++</span>
### Make your database usage eeasier and universal now!
<hr>

#### Requirements
| Compiler version | Minimum C++ standard required |
|:----------------:|:-----------------------------:|
| GCC              | -std=c++17                    |

# How to use

1. Make sure to include the library and the database interface

    ```cpp
    #include <orm/orm.hpp>
    ```
    - MySQL
        ```cpp
        #include <ORM/database/MySQL/mysql.hpp>
        ```
    - MockSQL
        ```cpp
        #include <ORM/database/MockSQL/MockSQL.hpp>
        ```
    
2. Initiate your Database
    
    **_Note:_** Make sure your database is set as ``constexpr``.

    - MySQL
        ```cpp
        constexpr const auto DB = ORM::Database<ORM::MySQL>({ DB_HOST, DB_USERNAME, DB_PASSWORD, DB_NAME, DB_PORT, nullptr, 0 });
        ``` 
    - MockSQL
        ```
        constexpr const auto DB = ORM::Database<ORM::MockSQL>({ "connection string" });
        ```
    
3. Initiate your table
    
    **_Note:_** add separate typedef/using for each column and set the C++ type this column will be cast to.
    
    ```cpp
    class Users : ORM::Table {
    public:
        using id = ORM::Column(int);
        using username = ORM::Column(std::string);
        using test = ORM::Column(int);
    
        using Entity = ORM::Result<id, username>;
    };
    ```

    **_Note:_** The Entity type is not requeired. It is used when your request will return a set of rows.

4. Initiate your requests
    
    **_Note:_** The request should be declared as ``constexpr``. For easy usage, they should be declared as ``static``.

    ```cpp
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
    ```
    
5. How to use your requests
    - Request calls
        If the database and the table(s) are setup as shown above, the requests should be used in the following way:
        ```cpp
        Users::createTable();
        Users::insertUser("Alex 'Coder of worlds' Tsvetanov");
        ```
    - Save request output
        ``cpp
        auto query_output = Users::selectUser("Alex 'Coder of worlds' Tsvetanov");
        ```
    - Access different columns of the output rows
        ```cpp
        for (auto row : query_output) {
            std::cout << row.get<Users::id>() << ", ";
        }
        ```
    **_Note:_** Even if the request's output is 1 row, it is still returned as collection (std::list) of the rows.

Check [example/](https://github.com/WebFrame/ORM-Abstract/blob/master/example) for more information.

# Benefits of the library
1. DB migration
    If a change in the database type is needed, all you have to do is to change the ``DB`` variable template parameter with the new database. Then change the SQL requests if neccessary and you are set.
1. SQL injection prevention
    All database interfaces have their own SQL injection protection in case you want to use query parameters.

# Socials
[![LinkedIn](https://img.shields.io/badge/linkedin-%230077B5.svg?logo=linkedin&logoColor=white)](https://www.linkedin.com/in/alex-tsvetanov/)