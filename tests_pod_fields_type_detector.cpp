#include "pod_fields_type_detector.h"
#include <cstdint>

namespace
{

struct A
{
	int m1;
	char m2;
	float m3;
};

void test()
{
	using types_tuple = type_list<double, float, A, long, int, char, short>;
	constexpr auto types_indexes = trivially_constructible_pod_fields_type_detector<A, types_tuple>::detect();
	static_assert(types_indexes[0] == 4, "int");
	static_assert(types_indexes[1] == 5, "char");
	static_assert(types_indexes[2] == 1, "float");
}

void test2()
{
	using types_tuple = type_list<long, double, A>;
	constexpr auto types_indexes = not_trivially_constructible_pod_fields_type_inaccurate_detector<A, types_tuple>::detect();
	static_assert(types_indexes[0] == 1, "int");
	static_assert(types_indexes[1] == 1, "char");
	static_assert(types_indexes[2] == 1, "float");
	// I have no idea why, but the next code compiles successfully
	// A{ as_type<double>{}, as_type<double>{}, as_type<double>{} };
}

void test3()
{
	using types_tuple = type_list<int,float,char, A>;
	constexpr auto types_indexes = not_trivially_constructible_pod_fields_type_accurate_detector<A, types_tuple>::indexes_of_types_sorted_by_size;
	static_assert(types_indexes[0] == 2, "int");
	static_assert(types_indexes[1] == 0, "char");
	static_assert(types_indexes[2] == 1, "float");
	static_assert(types_indexes[3] == 3, "float");
}

void test4()
{
	using types_tuple = type_list<double, float, A, long, int, char, short>;
	constexpr auto types_indexes = not_trivially_constructible_pod_fields_type_accurate_detector<A, types_tuple>::detect();
	static_assert(types_indexes[0] == 4, "int");
	static_assert(types_indexes[1] == 5, "char");
	static_assert(types_indexes[2] == 1, "float");
}

struct S
{
	int a;
	float b;
	char c;
};

void test_s1()
{
	static_assert(sizeof(int) == sizeof(long), "!");
	using types_tuple_t = type_list<int8_t, int16_t, int32_t, int64_t, float, double>;
	using pod_fields_scanner_t = not_trivially_constructible_pod_fields_type_accurate_detector<S, types_tuple_t>;
	static_assert(pod_fields_scanner_t::fields_count == 3, "fields_count");
	constexpr auto types_indexes = pod_fields_scanner_t::detect();
	static_assert(types_indexes[0] == 2, "int");
	static_assert(types_indexes[1] == 4, "char");
	static_assert(types_indexes[2] == 0, "float");
}

}