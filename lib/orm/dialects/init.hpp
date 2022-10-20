/** 
 *  @file   init.hpp 
 *  @brief  Dialect abstract definition for the ORM 
 *  @author Alex Tsvetanov
 *  @date   2022-09-09
 ***********************************************/

#pragma once
#include "../ORM.hpp"
#include <string>
#include "../std/string_view"
#include <sstream>
#include <regex>
#include <type_traits>

namespace ORM {
    /* abstract */ class Dialect {
    public:
        virtual ~Dialect() = default;
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
        virtual Page get_pagification() const = 0;
        template <typename ArgT1, typename... ArgTs>
        inline std::string get_query(std::string query, ArgT1 arg1, ArgTs... args) {
            // TODO: SQL injection protection
            return this->get_query(
                std::regex_replace(query, this->get_var_regex(), this->parse(arg1), std::regex_constants::format_first_only),
                args...
            );
        }

        template <typename ArgT1>
        inline std::string get_query(std::string query, ArgT1 arg1) {
            // TODO: SQL injection protection
            return this->get_query(
                regex_replace(query, this->get_var_regex(), this->parse(arg1), std::regex_constants::format_first_only)
            );
        }

        inline std::string get_query(std::string query) {
            // TODO: SQL injection protection
            return query;
        }
    };
    
    class Page {
    protected:
        static std::size_t skip_to(size_t ItemsPerPage, size_t page) {
            return (page - 1) * ItemsPerPage;
        }
        std::string_view query;
    public:
        
        constexpr Page(std::string_view _query) : query(_query) {
        
        }
        
        std::string get_page(size_t ItemsPerPage, std::size_t page) {
            const char* fmt = query.data();
            const size_t sz = snprintf(NULL, 0, fmt, skip_to(ItemsPerPage, page), ItemsPerPage);
            char* buf = new char[sz + 1];
            snprintf(buf, sizeof(char) * (sz + 1), fmt, skip_to(ItemsPerPage, page), ItemsPerPage);
            std::string get_page_query = std::string(buf);
            delete[] buf;
            return get_page_query;
        };
    };
}