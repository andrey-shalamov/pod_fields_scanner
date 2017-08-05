#include "pod_fields_count_detector.h"

namespace
{
struct A
{
	char m1;
	int m2;
	long m3;
};

struct B
{
	char m1;
	int m2;
	long m3;
	A m4;
	A m5;
};

void test()
{
	static_assert(pod_fields_count_detector<A>::detect() == 3, "pod_fields_count_detector A");
	static_assert(pod_fields_count_detector<B>::detect() == 5, "pod_fields_count_detector B");
}
}