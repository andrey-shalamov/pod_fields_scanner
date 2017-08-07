#include "pod_fields_type_detector.h"

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
	using types_tuple = std::tuple<double, float, A, long, int, char, short>;
	constexpr auto types_indexes = trivially_constructible_pod_fields_type_detector<A, types_tuple>::detect();
	static_assert(types_indexes[0] == 4, "int");
	static_assert(types_indexes[1] == 5, "char");
	static_assert(types_indexes[2] == 1, "float");
}

void test2()
{
	using types_tuple = std::tuple<long, double, A>;
	constexpr auto types_indexes = not_trivially_constructible_pod_fields_type_inaccurate_detector<A, types_tuple>::detect();
	static_assert(types_indexes[0] == 1, "int");
	static_assert(types_indexes[1] == 1, "char");
	static_assert(types_indexes[2] == 1, "float");
	// I have no idea why, but the next code compiles successfully
	// A{ as_type<double>{}, as_type<double>{}, as_type<double>{} };
}

void test3()
{
	using types_tuple = std::tuple<long, char, short, int, float, double, A>;
	constexpr auto types_indexes = not_trivially_constructible_pod_fields_type_accurate_detector<A, types_tuple>::detect();
	static_assert(types_indexes[0] == 3, "int");
	static_assert(types_indexes[1] == 1, "char");
	static_assert(types_indexes[2] == 4, "float");
	// I have no idea why, but the next code compiles successfully
	// A{ as_type<double>{}, as_type<double>{}, as_type<double>{} };
}

}