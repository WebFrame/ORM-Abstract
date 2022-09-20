/** 
 *  @file   init.hpp 
 *  @brief  Dialect abstract definition for the ORM 
 *  @author Alex Tsvetanov
 *  @date   2022-09-09
 ***********************************************/

#pragma once
#include <orm/orm.hpp>
#include <string>
#include <regex>
#include <iostream>

namespace ORM {
    /* abstract */ class Dialect {
    public:
        virtual std::string parse(int8_t value) const {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }
        virtual std::string parse(uint8_t value) const {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }
        virtual std::string parse(int16_t value) const {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }
        virtual std::string parse(uint16_t value) const {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }
        virtual std::string parse(int32_t value) const {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }
        virtual std::string parse(uint32_t value) const {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }
        virtual std::string parse(int64_t value) const {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }
        virtual std::string parse(uint64_t value) const {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }
        virtual std::string parse(float value) const {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }
        virtual std::string parse(double value) const {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }
        virtual std::string parse(char) const = 0;
        virtual std::string parse(const char*) const = 0;
        virtual std::string parse(const std::string&) const = 0;
        virtual std::string parse(const std::string_view&) const = 0;
        virtual std::regex get_var_regex() const = 0;
        template <typename ArgT1, typename... ArgTs>
        inline std::string get_query(std::string query, ArgT1 arg1, ArgTs... args) {
            // TODO: SQL injection protection
            std::cout << "Dialect many args: " << query << std::endl;
            std::string _ = this->get_query(
                std::regex_replace(query, this->get_var_regex(), this->parse(arg1), std::regex_constants::format_first_only),
                args...
            );
            std::cout << "Dialect many args: " << query << " = " << _ << std::endl;
            return _;
        }

        template <typename ArgT1>
        inline std::string get_query(std::string query, ArgT1 arg1) {
            // TODO: SQL injection protection
            std::cout << "Dialect 1 arg: " <<query << std::endl;
            std::string _ = this->get_query(
                regex_replace(query, this->get_var_regex(), this->parse(arg1), std::regex_constants::format_first_only)
            );
            std::cout << "Dialect 1 arg: " << query << " = " << _ << std::endl;
            return _;
        }

        inline std::string get_query(std::string query) {
            // TODO: SQL injection protection
            std::cout << "Dialect no args: " << query << std::endl;
            return query;
        }
    };
    
    template<std::size_t ItemsPerPage>
    class Page {
    protected:
        static std::size_t skip(std::size_t page) {
            return (page - 1) * ItemsPerPage;
        }
        std::string_view query;
    public:
        
        constexpr Page(std::string_view _query) : query(_query) {}
        
        std::string get_page(std::size_t page) {
            return query.data();//std::format(query, skip(page), ItemsPerPage);
        };
    };
}