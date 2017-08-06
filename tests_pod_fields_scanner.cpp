#include "pod_fields_scanner.h"
#include <array>
#include <string>
#include <vector>
#include <set>
#include <map>

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
	using types_tuple_t = std::tuple<char*, char, int*, const int*, float, float*, volatile int*, int, A, std::array<char, 3>>;
	using pod_fields_scanner_t = pod_fields_scanner<S3, types_tuple_t>;
	static_assert(pod_fields_scanner_t::fields_count == 5, "fields_count");
	constexpr auto types = pod_fields_scanner_t::detect_fields_types();
	static_assert(std::is_same<decltype(types), const std::tuple<char*, const int*, volatile int*, A, std::array<char, 3>>>::value, "detect_fields_types");
}

struct S4
{
	int m1[2]; // TODO: problem
};

void test_s4()
{
	using types_tuple_t = std::tuple<int, int[2]>;
	using pod_fields_scanner_t = pod_fields_scanner<S4, types_tuple_t>;
	static_assert(pod_fields_scanner_t::fields_count == 2, "fields_count");
	constexpr auto types = pod_fields_scanner_t::detect_fields_types();
	static_assert(std::is_same<decltype(types), const std::tuple<int,int>>::value, "detect_fields_types");
}

struct S5
{
	std::string s;
};

void test_s5()
{
	//static_assert(std::is_trivially_default_constructible_v<std::string>, "!");
	//static_assert(std::is_constructible_v<S2, std::string>, "!");
	/*using types_tuple_t = std::tuple<int, std::string>;
	using pod_fields_scanner_t = pod_fields_scanner<S5, types_tuple_t>;
	static_assert(pod_fields_scanner_t::fields_count == 1, "fields_count");
	constexpr auto types = pod_fields_scanner_t::detect_fields_types();*/
	//static_assert(std::is_same<decltype(types), const std::tuple<std::string>>::value, "detect_fields_types");
}

}