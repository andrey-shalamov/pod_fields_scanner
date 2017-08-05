#include <cstddef>
#include <array>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>
#include <type_traits>
#include <tuple>

constexpr size_t pow(size_t base, size_t exp)
{
	size_t r = 1;
	for (size_t i = 0; i < exp; ++i) r *= base;
	return r;
}

template<size_t N, size_t M>
struct n_tuples_of_m_set
{
	static constexpr size_t size = pow(M, N);

	static constexpr std::array<std::array<size_t, N>, size> create_n_tuples()
	{
		return create_n_tuples_impl(std::make_index_sequence<size>{});
	}

	template<size_t... Indexes>
	static constexpr std::array<std::array<size_t, N>, size>
		create_n_tuples_impl(std::index_sequence<Indexes...>)
	{
		return { { create_tuple<Indexes>()... } };
	}

	template<size_t Index, typename... Elements>
	static constexpr std::enable_if_t<sizeof...(Elements) == N, std::array<size_t, N>>
		create_tuple(Elements... elements)
	{
		static_assert(Index == 0, "overflow");
		return { { elements... } };
	}

	template<size_t Index, typename... Elements>
	static constexpr std::enable_if_t<sizeof...(Elements) < N, std::array<size_t, N>>
		create_tuple(Elements... elements)
	{
		return create_tuple<Index / M>(Index % M, elements...);
	}
};

template<size_t>
struct as_any_type
{
	template<typename T>
	constexpr operator T();
};

template<typename T>
struct as_type
{
	constexpr operator T();
};

template<typename... Ts>
struct as_any_type_of : as_type<Ts...> {};

//

template<size_t MinIndex, size_t... Is>
constexpr auto make_index_sequence_range(std::index_sequence<Is...>)
{
	return std::index_sequence<(MinIndex + Is)...>{};
}

template<size_t MinIndex, size_t MaxIndex>
constexpr auto make_index_sequence_range()
{
	return make_index_sequence_range<MinIndex>(std::make_index_sequence<MaxIndex - MinIndex>{});
}

//

template<typename PodType, typename TypesTuple>
struct pod_fields_scanner
{
	static constexpr size_t detect_fields_count()
	{
		return std::get<size_t>(detect_fields_count_impl(std::make_index_sequence<sizeof(PodType)>{}));
	}

	template<size_t... Is>
	static constexpr auto detect_fields_count_impl(std::index_sequence<Is...>)
	{
		return detect_fields_count_impl(std::make_index_sequence<sizeof...(Is)-1>{});
	}

	template<size_t I0, size_t... Is>
	static constexpr auto
		detect_fields_count_impl(std::index_sequence<I0, Is...>) ->
		std::tuple<size_t, decltype(PodType{ as_any_type<I0>{}, as_any_type<Is>{}... })>
	{
		return { 1 + sizeof...(Is), {} };
	}

	//

	static constexpr auto fields_count = pod_fields_scanner<PodType, TypesTuple>::detect_fields_count();
	//static constexpr auto n_tuples = n_tuples_of_m_set<fields_count, std::tuple_size<TypesTuple>::value>::create_n_tuples();

	using n_tuples_of_m_set_type = n_tuples_of_m_set<fields_count, std::tuple_size<TypesTuple>::value>;

	//

	template<size_t TupleIndex>
	static constexpr auto get_tuple_types()
	{
		return get_tuple_types_impl<TupleIndex>(std::make_index_sequence<fields_count>{});
	}

	template<size_t TupleIndex, size_t... Is>
	static constexpr auto get_tuple_types_impl(std::index_sequence<Is...>)
	{
		constexpr auto types_tuple_indexes = n_tuples_of_m_set_type::template create_tuple<TupleIndex>();
		return std::make_tuple(decltype(std::get<types_tuple_indexes[Is]>(TypesTuple{})){}...);
	}

	//
	template<size_t I, typename T>
	struct real_type_detector
	{
		bool* value;
		constexpr real_type_detector(bool* b) noexcept : value(b) {}

		template<typename RealType>
		constexpr operator RealType()
		{
			value[I] = std::is_same<RealType, T>::value;
			return {};
		}
	};

	template<typename... Ts>
	static constexpr bool is_real_types(const std::tuple<Ts...>& typesTuple)
	{
		return is_real_types_impl(std::make_index_sequence<sizeof...(Ts)>{}, typesTuple);
	}

	template<size_t... Is, typename... Ts>
	static constexpr bool is_real_types_impl(std::index_sequence<Is...>, std::tuple<Ts...>)
	{
		bool indicators[sizeof...(Ts)]{};
		static_cast<void>(PodType{ real_type_detector<Is, Ts>{indicators}... });
		bool result = true;
		static_cast<void>(std::initializer_list<int>{((result &= indicators[Is]), 0)...});
		return result;
	}

	//

	static constexpr auto get_fields_tuple_types()
	{
		constexpr auto result = get_real_fields_tuple_types_impl(std::make_index_sequence<n_tuples_of_m_set_type::size>{}, get_tuple_types<n_tuples_of_m_set_type::size - 1>());
		return std::get<0>(result);
	}

	template<size_t... Is, typename... Ts>
	static constexpr auto get_real_fields_tuple_types_impl(std::index_sequence<Is...>, const std::tuple<Ts...>&)
	{
		constexpr size_t next_size = sizeof...(Is)-1;
		static_assert(next_size > 0, "no match");
		return get_real_fields_tuple_types_impl(std::make_index_sequence<next_size>{}, get_tuple_types<next_size - 1>());
	}

	struct helper
	{
		constexpr explicit helper(int) {}
	};

	template<size_t I0, size_t... Is, typename... Ts>
	static constexpr auto get_real_fields_tuple_types_impl(std::index_sequence<I0, Is...>, const std::tuple<Ts...>&) ->
		std::tuple < std::tuple<Ts...>, decltype(helper{ std::conditional_t < is_real_types(std::tuple<Ts...>{}), int, float > {} }) >
	{
		return { std::tuple<Ts...>{}, helper(0) };
	}

	template<size_t... Is, typename... Ts>
	static constexpr auto get_fields_tuple_types_impl(std::index_sequence<Is...>, const std::tuple<Ts...>&)
	{
		constexpr size_t next_size = sizeof...(Is)-1;
		static_assert(next_size > 0, "no match");
		return get_fields_tuple_types_impl(std::make_index_sequence<next_size>{}, get_tuple_types<next_size - 1>());
	}

	template<size_t I0, size_t... Is, typename... Ts>
	static constexpr auto get_fields_tuple_types_impl(std::index_sequence<I0, Is...>, const std::tuple<Ts...>& types) ->
		std::tuple<bool, size_t, decltype(PodType{ Ts{}... })>
	{
		return { is_real_types(types), sizeof...(Is), {} };
	}

	//
	template<size_t N>
	static constexpr auto get_fields_tuple_types_optimized()
	{
		constexpr size_t parts = (n_tuples_of_m_set_type::size / N) + static_cast<size_t>((n_tuples_of_m_set_type::size % N) > 0);
		constexpr size_t index = get_index_tuple_types<N>(std::make_index_sequence<parts>{});
		static_assert(index < n_tuples_of_m_set_type::size, "not found");
		return get_tuple_types<index>();
	}

	template<size_t N, size_t... Is>
	static constexpr size_t get_index_tuple_types(std::index_sequence<Is...>)
	{
		constexpr size_t not_found_index = static_cast<size_t>(-1);
		constexpr std::array<size_t, sizeof...(Is)> parts{ check_indexes_of_tuple_types<N*Is>(std::false_type{}, make_index_sequence_range<(N*Is), (N*Is + N) < n_tuples_of_m_set_type::size ? (N*Is + N) : n_tuples_of_m_set_type::size >())... };
		for (size_t i = 0; i < parts.size(); ++i)
			if (parts[i] != not_found_index)
				return parts[i];
		return not_found_index;
	}

	template<size_t MinIndex, size_t... Is>
	static constexpr size_t check_indexes_of_tuple_types(std::false_type, std::index_sequence<Is...>)
	{
		constexpr size_t next_size = sizeof...(Is) > 0 ? sizeof...(Is) - 1 : 0;
		if (is_real_types(get_tuple_types<MinIndex + next_size>()))
			return MinIndex + next_size;
		using no_match = std::integral_constant<bool, sizeof...(Is) == 0>;
		return check_indexes_of_tuple_types<MinIndex>(no_match{}, make_index_sequence_range<MinIndex, MinIndex + next_size>());
	}

	template<size_t MinIndex, size_t... Is>
	static constexpr size_t check_indexes_of_tuple_types(std::true_type, std::index_sequence<Is...>)
	{
		return static_cast<size_t>(-1);
	}
};


struct A {};
struct B {};
struct C {};

struct S
{
	int a;
	float b;
	char c;
};

using str_t = std::string;

struct S2
{
	char a1;
	int a2;
	char a3;
	int a4;
	char a5;
	int a6;
	char a7;
	int a8;
	char a9;
	int a10;
};

int main()
{
	using types_tuple_t = std::tuple<int, char, float>;
	using pod_fields_scanner_t = pod_fields_scanner<S, types_tuple_t>;

	static_assert(pod_fields_scanner_t::fields_count == 3, "fields_count");
	static_assert(pod_fields_scanner_t::n_tuples_of_m_set_type::size == 27, "n_tuples_of_m_set_type::size");

	constexpr auto s_tuple_types = pod_fields_scanner_t::get_fields_tuple_types();
	static_assert(std::is_same<decltype(s_tuple_types), const std::tuple<int, float, char>>::value, "");

	static_assert(pod_fields_scanner_t::is_real_types(std::tuple<int, float, char>{}), "not real types");

	static_assert(pod_fields_scanner_t::check_indexes_of_tuple_types<6>(std::false_type{}, make_index_sequence_range<6, 8>()) != size_t(-1), "check indexes of tuples");

	static_assert(pod_fields_scanner_t::get_index_tuple_types<5>(std::make_index_sequence<6>{}) == 7, "get_index_tuple_types");
	static_assert(std::is_same<decltype(pod_fields_scanner_t::get_fields_tuple_types_optimized<5>()), std::tuple<int, float, char>>::value, "get_fields_tuple_types_optimized");

	constexpr auto t = pod_fields_scanner_t::get_tuple_types<7>();
	static_assert(pod_fields_scanner_t::is_real_types(t), "tuple index");

	static_assert(std::is_same<decltype(make_index_sequence_range<7, 8>()), std::index_sequence<7>>::value, "index sequnce range");

	using scanner_S2 = pod_fields_scanner<S2, std::tuple<int, char>>;
	static_assert(scanner_S2::fields_count == 10, "args");
	static_assert(scanner_S2::n_tuples_of_m_set_type::size == 1024, "scanner_S2::n_tuples_of_m_set_type::size");
	static_assert(std::is_same<decltype(scanner_S2::get_fields_tuple_types_optimized<100>()), std::tuple<char,int, char, int, char, int, char, int, char, int>>::value, "types");

	return 0;
}