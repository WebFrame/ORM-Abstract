#pragma once

#include "../ORM.hpp"
#include "permissions/init.hpp"
#include <string>
#include "../std/string_view"
#include <memory>
#include <map>

namespace ORM {
    template <typename DB_t>
	struct Database {
    private:
		typename DB_t::params_t connection_params;
	public:
        using Connection = DB_t;
        
        explicit constexpr Database(typename DB_t::params_t params) : connection_params(params) {


        } 
    
        std::shared_ptr<DB_t> get_native_conn() const {
            static const auto conn = std::make_shared<DB_t>(connection_params);
            return conn;
        }

        template<typename RowT>
        std::shared_ptr<ORM::DBExecutor<RowT>> get_executor() const {
            static const auto exec = std::static_pointer_cast<ORM::DBExecutor<RowT>>(std::make_shared<typename DB_t::template Executor<RowT>>(get_native_conn()));
            return exec;
        }

        template<typename RowT, typename... Columns>
        constexpr _Query<Database<DB_t>, RowT, Columns...> Query(const std::string_view& query) const {
            return _Query<Database<DB_t>, RowT, Columns...>(*this, query);
        }

        template<typename RowT, typename... Columns>
        constexpr _PagedQuery<Database<DB_t>, RowT, Columns...> PagedQuery(const std::string_view& query) const {
            return _PagedQuery<Database<DB_t>, RowT, Columns...>(*this, query);
        }
	};
}