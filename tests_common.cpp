#include "common.h"

namespace
{
struct array_filler
{
	template<typename T>
	static constexpr void fill(T* values, size_t size, size_t mult)
	{
		for (size_t i = 0; i < size; ++i)
		{
			values[i] = i * mult;
		}
	}
};

struct A
{
	char a;
	int b;
	float c;
};

void test()
{
	static_assert(constexpr_pow(2, 10) == 1024, "constexpr_pow");
	constexpr constexpr_array<int, 10, array_filler> arr(2);
	static_assert(arr[5] == 10, "constexpr_array");
	static_assert(std::is_same<decltype(make_index_sequence_range<7, 8>()), std::index_sequence<7>>::value, "make_index_sequence_range");
	using type_list_t = type_list<double, char, int, float>;
	static_assert(std::is_same<int, typename type_list_element<2, type_list_t>::type >::value, "type_list_element");
	static_assert(std::is_same < A, decltype(A{ type_list_element<1, type_list_t>::get(), type_list_element<2, type_list_t>::get(), type_list_element<3, type_list_t>::get() }) > ::value, "type_list_element::get");
}
}