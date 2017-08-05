#include "pod_fields_scanner.h"

namespace
{

struct S
{
	int a;
	float b;
	char c;
};

void test_s1()
{
	using types_tuple_t = std::tuple<int, char, float>;
	using pod_fields_scanner_t = pod_fields_scanner<S, types_tuple_t>;
	static_assert(pod_fields_scanner_t::fields_count == 3, "fields_count");
	static_assert(pod_fields_scanner_t::n_tuples_of_m_set_type::size == 27, "n_tuples_of_m_set_type::size");
	constexpr auto s_tuple_types = pod_fields_scanner_t::get_fields_tuple_types();
	static_assert(std::is_same<decltype(s_tuple_types), const std::tuple<int, float, char>>::value, "get_fields_tuple_types");
	static_assert(pod_fields_scanner_t::check_indexes_of_tuple_types<6>(std::false_type{}, make_index_sequence_range<6, 8>()) != size_t(-1), "check indexes of tuples");
	static_assert(pod_fields_scanner_t::get_index_tuple_types<5>(std::make_index_sequence<6>{}) == 7, "get_index_tuple_types");
}

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

void test_s2()
{
	//using scanner_S2 = pod_fields_scanner<S2, std::tuple<int, char>>;
	//static_assert(scanner_S2::fields_count == 10, "args");
	//static_assert(scanner_S2::n_tuples_of_m_set_type::size == 1024, "scanner_S2::n_tuples_of_m_set_type::size");
	////static_assert(std::is_same<decltype(scanner_S2::get_fields_tuple_types_optimized<100>()), std::tuple<char,int, char, int, char, int, char, int, char, int>>::value, "types");

	//static_assert(scanner_S2::getMinMaxIndexes(10, 1).first == 10, "getMinMaxIndexes");
	//static_assert(scanner_S2::getMinMaxIndexes(10, 1).second == 20, "getMinMaxIndexes");

	//constexpr pod_fields_scanner_t::n_tuples_of_m_set_type::spec_constexpr_array arr(7);
	//static_assert(arr[0] == 0, "0");
	//static_assert(arr[1] == 2, "1");
	//static_assert(arr[2] == 1, "2");

	//constexpr constexpr_array<size_t, 10, spec_functor> arr2;
	//static_assert(arr2[4] == 16, "16");
}



}