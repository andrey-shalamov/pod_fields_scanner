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
	constexpr auto types_indexes = pod_fields_type_detector<A, types_tuple>::detect();
	static_assert(types_indexes[0] == 4, "int");
	static_assert(types_indexes[1] == 5, "char");
	static_assert(types_indexes[2] == 1, "float");
}
}