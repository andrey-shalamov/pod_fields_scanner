#pragma once

#include <array>
#include <initializer_list>

#include "common.h"
#include "n_tuples_of_m_set.h"
#include "pod_fields_count_detector.h"
#include "pod_fields_type_detector.h"

template<typename PodType, typename TypesTuple>
struct pod_fields_scanner
{
	static constexpr auto fields_count = pod_fields_count_detector<PodType>::detect();

	using n_tuples_of_m_set_type = n_tuples_of_m_set<fields_count, std::tuple_size<TypesTuple>::value>;

	template<size_t TupleIndex>
	static constexpr auto get_tuple_types()
	{
		return n_tuples_of_m_set_type::template  get_tuple_types<TupleIndex, TypesTuple>();
	}

	static constexpr std::pair<size_t, size_t> getMinMaxIndexes(size_t n, size_t part)
	{
		const size_t min = n*part;
		const size_t max = (min + n) < n_tuples_of_m_set_type::size ? (min + n) : n_tuples_of_m_set_type::size;
		return { min, max };
	}

	template<size_t N>
	static constexpr size_t get_parts()
	{
		return (n_tuples_of_m_set_type::size / N) + static_cast<size_t>((n_tuples_of_m_set_type::size % N) > 0);
	}

	template<size_t N>
	struct min_index_functor
	{
		constexpr size_t operator()(size_t i) const noexcept { return N*i; }
	};

	template<size_t N>
	struct max_index_functor
	{
		static constexpr size_t max = n_tuples_of_m_set_type::size;
		constexpr size_t operator()(size_t i) const noexcept { return (N*i + N) < max ? (N*i + N) : max; }
	};

	struct min_indexes_array_filler
	{
		template<typename T>
		static constexpr void fill(T* values, size_t size, size_t n, size_t max) noexcept
		{
			for (size_t i = 0; i < size; ++i)
			{
				values[i] = i*n;
			}
			values[size - 1] = max;
		}
	};

	struct indexes_array_filler
	{
		template<typename T>
		static constexpr void fill(T* values, size_t size, size_t min) noexcept
		{
			for (size_t i = 0; i < size; ++i)
			{
				values[i] = min + i;
			}
		}
	};

	static constexpr auto get_fields_tuple_types()
	{
		constexpr size_t N = 4;
		constexpr size_t parts = (n_tuples_of_m_set_type::size / N) + static_cast<size_t>((n_tuples_of_m_set_type::size % N) > 0);
		constexpr size_t index = get_index_tuple_types<N>(std::make_index_sequence<parts>{});
		static_assert(index < n_tuples_of_m_set_type::size, "not found");
		return get_tuple_types<index>();
	}

	template<size_t N, size_t... Is>
	static constexpr size_t get_index_tuple_types(std::index_sequence<Is...>)
	{
		constexpr size_t not_found_index = static_cast<size_t>(-1);
		constexpr std::array<size_t, sizeof...(Is)> parts{ check_indexes_of_tuple_types<N*Is>(std::false_type{}, make_index_sequence_range<(N*Is), (N*Is + N) < n_tuples_of_m_set_type::size ? (N*Is + N) : n_tuples_of_m_set_type::size >())... };
		for (size_t i = 0; i < parts.size(); ++i)
			if (parts[i] != not_found_index)
				return parts[i];
		return not_found_index;
	}

	template<size_t MinIndex, size_t... Is>
	static constexpr size_t check_indexes_of_tuple_types(std::false_type, std::index_sequence<Is...>)
	{
		constexpr size_t next_size = sizeof...(Is) > 0 ? sizeof...(Is)-1 : 0;
		if (pod_fields_type_detector<PodType>::is_real_types(get_tuple_types<MinIndex + next_size>()))
			return MinIndex + next_size;
		using no_match = std::integral_constant<bool, sizeof...(Is) == 0>;
		return check_indexes_of_tuple_types<MinIndex>(no_match{}, make_index_sequence_range<MinIndex, MinIndex + next_size>());
	}

	template<size_t MinIndex, size_t... Is>
	static constexpr size_t check_indexes_of_tuple_types(std::true_type, std::index_sequence<Is...>)
	{
		return static_cast<size_t>(-1);
	}

	
};