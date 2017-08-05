#include "n_tuples_of_m_set.h"

namespace
{
void test()
{
	constexpr auto binary_7 = n_tuples_of_m_set<4, 2>::create_tuple(7);
	static_assert(binary_7[0] == 0, "");
	static_assert(binary_7[1] == 1, "");
	static_assert(binary_7[2] == 1, "");
	static_assert(binary_7[3] == 1, "");

	constexpr auto tuple_types_8 = n_tuples_of_m_set<4, 2>::get_tuple_types<8, std::tuple<int, char>>();
	static_assert(std::is_same<decltype(tuple_types_8), const std::tuple<char,int,int,int>>::value, "get_tuple_types 8");

	/*constexpr auto n_tuples = n_tuples_of_m_set<4, 2>::create_n_tuples();
	constexpr auto binary_10 = n_tuples[10];*/
}
}