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

constexpr void test_arr()
{
	constexpr size_t N = 1024;
	constexpr constexpr_array<int, N, array_filler> arr1(2);
	for (size_t i = 0; i < N; ++i)
	{
		
	}
}

void test()
{
	static_assert(constexpr_pow(2, 10) == 1024, "constexpr_pow");
	constexpr constexpr_array<int, 10, array_filler> arr(2);
	static_assert(arr[5] == 10, "constexpr_array");

	static_assert(std::is_same<decltype(make_index_sequence_range<7, 8>()), std::index_sequence<7>>::value, "make_index_sequence_range");
}
}