/** 
 *  @file   init.hpp 
 *  @brief  Column abstract definition for the ORM 
 *  @author Alex Tsvetanov
 *  @date   2022-09-09
 ***********************************************/

#pragma once
#include <orm/orm.hpp>

namespace ORM {
    template<typename T>
    class Column {
    public:
        using type = T;
        constexpr Column() {  }
    };
}