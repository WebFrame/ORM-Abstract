/**
 *  @file   init.hpp
 *  @brief  Result abstract definition for the ORM
 *  @author Alex Tsvetanov
 *  @date   2022-10-18
 ***********************************************/

#pragma once
#include "../ORM.hpp"
#include <tuple>
#include <iostream>
#include "../std/string_view"

namespace webframe::ORM {
	template <typename... Columns>
	class Result : public std::tuple<typename Columns::type...> {
	private:
		/*template<typename LookUp, typename... CColumns, size_t... Inds>
		constexpr size_t get_column_index(std::index_sequence<Inds...>) const {
			return (((LookUp::UUID == CColumns::UUID) * Inds) | ...);
		}*/
		template<typename LookUp, size_t UUID1>
		static constexpr size_t get_column_index(size_t ind = 0) {
			return (LookUp::UUID == UUID1) ? ind : throw "shit";
		}
		template<typename LookUp, size_t UUID1, size_t UUID2, size_t... UUIDs>
		static constexpr size_t get_column_index(size_t ind = 0) {
			return ((LookUp::UUID == UUID1) ? ind : get_column_index<LookUp, UUID2, UUIDs...>(ind + 1));
		}
		const std::tuple<typename Columns::type...>& get_tuple() const {
			return *this;
		}
	public:
		Result(std::tuple<typename Columns::type...>&& val) : std::tuple<typename Columns::type...>(val) {}
		template <typename Column>
		typename Column::type get() {
			return std::get<get_column_index<Column, Columns::UUID...>()>(this->get_tuple());
		}
	};

	template <typename T>
	struct ResultOf {
	};

	template <typename T, size_t UUID>
	struct ResultOf<_Column<T, UUID>> {
		using type = Result<_Column<T, UUID>>;
	};
	template <typename... Ts>
	struct ResultOf<Result<Ts...>> {
		using type = Result<Ts...>;
	};

	template <>
	struct ResultOf<void> {
		using type = void;
	};
}
namespace std {
	template< typename... Types >
	struct tuple_size<webframe::ORM::Result<Types...> > : public std::tuple_size< std::tuple<typename Types::type...> > {};

	template< std::size_t I, typename... Types >
	struct tuple_element< I, webframe::ORM::Result<Types...> > : public tuple_element< I, std::tuple<typename Types::type...> > {};
}