#pragma once
#include "MockSQL.hpp"
#include <utility>
#include <iostream>

namespace webframe::ORM
{
	void MockSQL::init_conn(const params_t& conn)
	{
		std::cout << "Connection string: " << std::get<0>(conn) << std::endl;
	}
	MockSQL::MockSQL(const params_t& conn)
	{
		init_conn(conn);
	}
	MockSQL::~MockSQL()
	{
	}
	void MockSQL::commit()
	{
		std::cout << "Commit" << std::endl;
	}
	void MockSQL::rollback()
	{
		std::cout << "Rollback" << std::endl;
	}
	void MockSQL::begin_transaction() 
	{
		std::cout << "START TRANSACTION;" << std::endl;
	}
	std::shared_ptr<Dialect> MockSQL::get_dialect() const
	{
		static std::shared_ptr<Dialect> MySQLDialect = std::static_pointer_cast<Dialect>(std::make_shared<MockSQLDialect>());
		return MySQLDialect;
	}

	std::string MockSQL::MockSQLDialect::escape(const std::string& value) const
	{
		return this->escape(value.c_str());
	}
	std::string MockSQL::MockSQLDialect::escape(const char* value) const
	{ // https://dev.MockSQL.com/doc/refman/8.0/en/string-functions.html#function_quote
		std::stringstream ss;
		const size_t size = strlen(value);
		for (size_t i = 0; i < size; i++)
		{
			switch (value[i])
			{
			case '\b':
				break;
			case '\n':
				break;
			case '\r':
				break;
			case '\t':
				break;
			case 26: // CTRL+Z
				break;
			case '\'':
			case '\\':
				ss << "\\" << value[i];
				break;
			default:
				ss << value[i];
				break;
			}
		}
		return ss.str();
	}
	std::string MockSQL::MockSQLDialect::escape(const std::string_view& value) const
	{
		return this->escape(std::string(value).c_str());
	}
	std::string MockSQL::MockSQLDialect::parse(char value) const
	{
		std::stringstream ss;
		ss << this->escape(std::string() + value);
		return ss.str();
	}
	std::string MockSQL::MockSQLDialect::parse(const char* value) const
	{
		std::stringstream ss;
		ss << "'" << this->escape(value) << "'";
		return ss.str();
	}
	std::string MockSQL::MockSQLDialect::parse(const std::string& value) const
	{
		return std::string("'") + this->escape(value) + std::string("'");
	}
	std::string MockSQL::MockSQLDialect::parse(const std::string_view& value) const
	{
		return std::string("'") + this->escape(value) + std::string("'");
	}
	std::regex MockSQL::MockSQLDialect::get_var_regex() const
	{
		return std::regex("\\?");
	}
	Page MockSQL::MockSQLDialect::get_pagification() const
	{
		static const Page pagification = Page("LIMIT %zu, %zu");
		return pagification;
	}

}
