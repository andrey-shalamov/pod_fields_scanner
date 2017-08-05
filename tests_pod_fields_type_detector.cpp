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
	static_assert(pod_fields_type_detector<A>::is_real_types(std::tuple<int, char, float>{}), "is_real_types");
	static_assert(!pod_fields_type_detector<A>::is_real_types(std::tuple<char, int, double>{}), "is_real_types");
}
}