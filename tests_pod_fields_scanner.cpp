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
	using types_tuple_t = std::tuple<char, short, int, long, float, double>;
	using pod_fields_scanner_t = pod_fields_scanner<S, types_tuple_t>;
	static_assert(pod_fields_scanner_t::fields_count == 3, "fields_count");
	constexpr auto types = pod_fields_scanner_t::detect_fields_types();
	static_assert(std::is_same<decltype(types), const std::tuple<int,float,char>>::value, "detect_fields_types");
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
	using types_tuple_t = std::tuple<char, short, int, long, float, double>;
	using pod_fields_scanner_t = pod_fields_scanner<S2, types_tuple_t>;
	static_assert(pod_fields_scanner_t::fields_count == 10, "fields_count");
	constexpr auto types = pod_fields_scanner_t::detect_fields_types();
	static_assert(std::is_same<decltype(types), const std::tuple<char, int, char, int, char, int, char, int, char, int>>::value, "detect_fields_types");
}



}