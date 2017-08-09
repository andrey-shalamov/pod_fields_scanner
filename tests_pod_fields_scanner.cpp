#include "pod_fields_scanner.h"
#include <array>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cstdint>

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
	using types_tuple_t = type_list<char, int16_t, int32_t, int64_t, float, double>;
	using pod_fields_scanner_t = pod_fields_scanner<S, types_tuple_t>;
	static_assert(pod_fields_scanner_t::fields_count == 3, "fields_count");
	constexpr auto types = pod_fields_scanner_t::detect_fields_types();
	static_assert(std::is_same<decltype(types), const type_list<int,float,char>>::value, "detect_fields_types");
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
	using types_tuple_t = type_list<char, int16_t, int32_t, int64_t, float, double>;
	using pod_fields_scanner_t = pod_fields_scanner<S2, types_tuple_t>;
	static_assert(pod_fields_scanner_t::fields_count == 10, "fields_count");
	constexpr auto types = pod_fields_scanner_t::detect_fields_types();
	static_assert(std::is_same<decltype(types), const type_list<char, int, char, int, char, int, char, int, char, int>>::value, "detect_fields_types");
}

struct A
{
	int a1;
	float a2;
};

struct S3
{
	char* m1;
	const int* m2;
	volatile int* m3;
	A m4;
	std::array<char, 3> m5;
};

void test_s3()
{
	using types_tuple_t = type_list<char*, char, int*, const int*, float, float*, volatile int*, int, A, std::array<char, 3>>;
	using pod_fields_scanner_t = pod_fields_scanner<S3, types_tuple_t>;
	static_assert(pod_fields_scanner_t::fields_count == 5, "fields_count");
	constexpr auto types = pod_fields_scanner_t::detect_fields_types();
	static_assert(std::is_same<decltype(types), const type_list<char*, const int*, volatile int*, A, std::array<char, 3>>>::value, "detect_fields_types");
}

struct S4
{
	int m1[2]; // TODO: problem
};

void test_s4()
{
	using types_tuple_t = type_list<int/*, int[2]*/>;
	using pod_fields_scanner_t = pod_fields_scanner<S4, types_tuple_t>;
	static_assert(pod_fields_scanner_t::fields_count == 2, "fields_count");
	constexpr auto types = pod_fields_scanner_t::detect_fields_types();
	static_assert(std::is_same<decltype(types), const type_list<int,int>>::value, "detect_fields_types");
}

struct S5
{
	std::string s;
	std::vector<int> v;
	std::set<float> set;
	std::map<std::string, int> map;
};

void test_s5()
{
	using types_tuple_t = type_list<std::vector<char>, std::map<std::string, int>, std::map<int, int>, std::set<float>, std::set<double>, int, std::string, std::vector<int>>;
	using pod_fields_scanner_t = pod_fields_scanner<S5, types_tuple_t>;
	static_assert(pod_fields_scanner_t::fields_count == 4, "fields_count");
	constexpr auto types = pod_fields_scanner_t::detect_fields_types();
	static_assert(std::is_same<decltype(types), const type_list<std::string, std::vector<int>, std::set<float>, std::map<std::string, int>>>::value, "detect_fields_types");
}

void test_6()
{
	using types_tuple_t = type_list<char, int, short>;
	using pod_fields_scanner_t = pod_fields_scanner<std::array<int, 3>, types_tuple_t>;
	static_assert(pod_fields_scanner_t::fields_count == 3, "fields_count");
	constexpr auto types = pod_fields_scanner_t::detect_fields_types();
	static_assert(std::is_same<decltype(types), const type_list<int, int, int>>::value, "detect_fields_types");
}

void test_7()
{
	using types_tuple_t = type_list<char, int, short>;
	using pod_fields_scanner_t = pod_fields_scanner<short[2], types_tuple_t>;
	static_assert(pod_fields_scanner_t::fields_count == 2, "fields_count");
	constexpr auto types = pod_fields_scanner_t::detect_fields_types();
	static_assert(std::is_same<decltype(types), const type_list<short,short>>::value, "detect_fields_types");
}

}