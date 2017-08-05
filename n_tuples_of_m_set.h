#pragma once

#include <tuple>

#include "common.h"

template<size_t N, size_t M>
struct n_tuples_of_m_set
{
	static constexpr size_t size = constexpr_pow(M, N);

	struct array_filler
	{
		template<typename T>
		static constexpr void fill(T* values, size_t size, size_t index) noexcept
		{
			for (size_t i = size; i-- > 0;)
			{
				values[i] = index % M;
				index /= M;
			}
		}
	};

	using tuple_type = constexpr_array<size_t, N, array_filler>;

	static constexpr tuple_type create_tuple(size_t index) noexcept
	{
		return tuple_type(index);
	}

	template<size_t Index>
	static constexpr tuple_type create_tuple() noexcept
	{
		return tuple_type(Index);
	}

	template<size_t Index, typename TypesTuple>
	static constexpr auto get_tuple_types() noexcept
	{
		static_assert(std::tuple_size<TypesTuple>::value == M, "incorrect tuple size");
		return get_tuple_types_impl<Index, TypesTuple>(std::make_index_sequence<N>{});
	}

	template<size_t Index, typename TypesTuple, size_t... Is>
	static constexpr auto get_tuple_types_impl(std::index_sequence<Is...>) noexcept
	{
		constexpr auto types_tuple_indexes = create_tuple<Index>();
		return std::make_tuple(decltype(std::get<types_tuple_indexes[Is]>(TypesTuple{})){}...);
	}

	/*struct matrix_filler
	{
		template<typename T, size_t n, size_t m>
		static constexpr void fill(T(&values)[n][m]) noexcept
		{
			for (size_t i = 0; i < n; ++i)
			{
				auto index = i;
				for (size_t j = m; j-- > 0;)
				{
					values[i][j] = index % M;
					index /= M;
				}
			}
		}
	};

	static constexpr auto create_n_tuples() noexcept
	{
		return constexpr_matrix<size_t, size, N, matrix_filler>();
	}*/
};