/** 
 *  @file   init.hpp 
 *  @brief  Column abstract definition for the ORM 
 *  @author Alex Tsvetanov
 *  @date   2022-09-09
 ***********************************************/

#pragma once
#include "../ORM.hpp"
#include <string>
#include "uid.hpp"

namespace webframe::ORM {
    template<typename T, size_t uid>
    class _Column {
    public:
        using type = T;
        static constexpr size_t UUID = uid;
        constexpr _Column() {}
    };
}