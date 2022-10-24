// ORM.cpp : Defines the entry point for the testing application.
//

#include <iostream>
#include <ORM/ORM.hpp>
#include "users/users.hpp"

using namespace std;

int main()
{
    try {
        DB.Begin();
        Users::createTable();
        Users::insertUser("Alex 'Coder of worlds' Tsvetanov");
        auto q1_id = Users::selectUser("Alex 'Coder of worlds' Tsvetanov");
        for (auto el : q1_id) {
            std::cout << el.get<Users::id>() << ", ";
        }
        std::cout << std::endl;
        auto q2_id = Users::selectAllUsers();
        for (auto el : q2_id) {
            std::cout << el.get<Users::id>() << "," << el.get<Users::username>() << " ; ";
        }
        std::cout << std::endl;
        auto q3_id = Users::pagedSelectAllUsers(30, 1);
        for (auto el : q3_id) {
            std::cout << el.get<Users::id>() << "," << el.get<Users::username>() << " ; ";
        }
        std::cout << std::endl;
        auto q4_id = Users::pagedSelectAllUsers(30, 2);
        for (auto el : q4_id) {
            std::cout << el.get<Users::id>() << "," << el.get<Users::username>() << " ; ";
        }
        std::cout << std::endl;
        DB.Rollback();
    }
    catch (std::exception& msg) {
        printf("Error: %s\n", msg.what());
    }
    catch (const char* msg) {
        printf("Error: %s\n", msg);
    }
	return 0;
}
