#pragma once
#include "../../ORM.hpp"
#include <utility>
#include <iostream>
#include "../../std/string_view"

namespace ORM
{
	class MockSQL : public DBConnection {
	private:
		class MockSQLDialect;

		template<typename RowT>
		class IExecutor;

	public:
		template <typename RowT>
		class Executor;

		using params_t = std::tuple<std::string_view>;

	protected:
		void init_conn(const params_t& conn);

	public:
		MockSQL(const params_t& conn);

		~MockSQL();

		void commit();

		void rollback();

		std::shared_ptr<Dialect> get_dialect() const;
	};

	class MockSQL::MockSQLDialect : public Dialect {
		std::string escape(const std::string& value) const;
		std::string escape(const char* value) const;
		std::string escape(const std::string_view& value) const;
	public:
		std::string parse(char value) const;
		std::string parse(const char* value) const;
		std::string parse(const std::string& value) const;
		std::string parse(const std::string_view& value) const;
		std::regex get_var_regex() const;

		Page get_pagification() const;
	};

	template<typename RowT>
	class MockSQL::IExecutor : public DBExecutor<RowT> {
	protected:
		std::shared_ptr<MockSQL> conn;
		void init_conn(std::shared_ptr<MockSQL> connection) { conn = connection; }

	public:
		std::shared_ptr<Dialect> get_dialect() const { return conn->get_dialect(); }

		void commit() { conn->commit(); }

		void rollback() { conn->rollback(); }
	};

	template <typename RowT>
	class MockSQL::Executor : public MockSQL::IExecutor<RowT> {
	public:
		Executor(std::shared_ptr<MockSQL> connection) {
			this->init_conn(connection);
		}
		std::list<RowT> execute(const std::string& query) { // non-transactional
			std::cout << "MockSQL::execute: " << query << std::endl;
			std::list<RowT> answer;
			return answer;
		}
	};

	template <>
	class MockSQL::Executor<void> : public MockSQL::IExecutor<void> {
	public:
		Executor(std::shared_ptr<MockSQL> connection) {
			this->init_conn(connection);
		}
		void execute(const std::string& query) { // transactional
			std::cout << "Query: " << query << std::endl;
			return;
		}
	};
}

#include "MockSQL_impl.hpp"