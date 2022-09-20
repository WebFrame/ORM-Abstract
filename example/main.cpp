#include <orm/orm.hpp>
#include "./obj/users.hpp"

int main () {
    Users::insertUser(5, "a\'lex");
    Users::selectUser("a\'lex");
    Users::selectAllUsers();
}