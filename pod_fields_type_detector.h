#pragma once

#include <tuple>
#include <utility>

#include "pod_fields_count_detector.h"

template<typename PodType, typename TypeList, size_t FieldIndex>
struct trivially_constructible_pod_field_type_detector
{
	static constexpr size_t fields_count = pod_fields_count_detector<PodType>::detect();
	static constexpr size_t not_found_index = static_cast<size_t>(-1);

	size_t* values;
	constexpr trivially_constructible_pod_field_type_detector(size_t* v) noexcept : values(v) {}

	template<typename RealType>
	constexpr operator RealType() const noexcept
	{
		static_assert(FieldIndex < fields_count, "incorrect index");
		constexpr size_t type_index = get_type_index<RealType>(std::make_index_sequence<TypeList::size>{});
		static_assert(type_index != not_found_index, "type index not found");
		values[FieldIndex] = type_index;
		return {};
	}

	template<typename T, size_t... Is>
	static constexpr size_t get_type_index(std::index_sequence<Is...>) noexcept
	{
		size_t result = not_found_index;
		std::initializer_list<int>{((result = std::is_same<T, type_list_element_t<Is, TypeList>>::value ? Is : result), 0)...};
		return result;
	}
};

template<typename PodType, typename TypeList>
struct trivially_constructible_pod_fields_type_detector
{
	static constexpr size_t fields_count = pod_fields_count_detector<PodType>::detect();

	static constexpr auto detect() noexcept
	{
		static_assert(std::is_pod<PodType>::value && std::is_trivially_constructible<PodType>::value, "PodType must be trivially constructible.");
		return constexpr_array<size_t, fields_count, types_indexes_filler>();
	}

private:
	struct types_indexes_filler
	{
		template<typename T>
		static constexpr void fill(T* values, size_t size) noexcept
		{
			static_cast<void>(size);
			fill_impl(values, std::make_index_sequence<fields_count>{});
		}

		template<typename T, size_t... Is>
		static constexpr void fill_impl(T* values, std::index_sequence<Is...>) noexcept
		{
			static_assert(sizeof...(Is) == fields_count, "incorrect tuple size");
			static_cast<void>(PodType{ trivially_constructible_pod_field_type_detector<PodType, TypeList, Is>{values}... });
		}
	};
};

template<typename PodType, typename TypeList>
struct not_trivially_constructible_pod_fields_type_inaccurate_detector
{
	static constexpr size_t fields_count = pod_fields_count_detector<PodType>::detect();

	static constexpr auto detect() noexcept
	{
		return constexpr_array<size_t, fields_count, types_indexes_filler>();
	}

private:
	struct types_indexes_filler
	{
		template<typename T>
		static constexpr void fill(T* values, size_t size) noexcept
		{
			static_cast<void>(size);
			fill_impl(values, std::make_index_sequence<fields_count>{});
		}

		template<typename T, size_t... Is>
		static constexpr void fill_impl(T* values, std::index_sequence<Is...>) noexcept
		{
			static_assert(sizeof...(Is) == fields_count, "incorrect tuple size");
			static_cast<void>(std::initializer_list<int>{((values[Is] = get_index_of_convertible_to_real_type<Is>()), 0)...});
		}

		template<size_t FieldIndex>
		static constexpr size_t get_index_of_convertible_to_real_type()
		{
			return get_index_of_convertible_to_real_type(std::make_index_sequence<TypeList::size>{}, std::make_index_sequence<FieldIndex>{}, make_index_sequence_range<FieldIndex + 1, fields_count>());
		}

		template<size_t... Is1, size_t... Is2, size_t... Is3>
		static constexpr size_t get_index_of_convertible_to_real_type(std::index_sequence<Is1...>, std::index_sequence<Is2...> is2, std::index_sequence<Is3...> is3)
		{
			return get_index_of_convertible_to_real_type(std::make_index_sequence<sizeof...(Is1)-1>{}, is2, is3);
		}

		template<size_t I0, size_t... Is1, size_t... Is2, size_t... Is3>
		static constexpr std::enable_if_t< std::is_same_v < PodType,
			decltype(PodType{ as_any_type<Is2>{}..., as_type<type_list_element_t<sizeof...(Is1), TypeList>>{}, as_any_type<Is3>{}... }) >, size_t >
			get_index_of_convertible_to_real_type(std::index_sequence<I0, Is1...>, std::index_sequence<Is2...>, std::index_sequence<Is3...>)
		{
			return sizeof...(Is1);
		}
	};
};

template<typename PodType, typename TypeList>
struct not_trivially_constructible_pod_fields_type_accurate_detector
{
	static constexpr size_t fields_count = pod_fields_count_detector<PodType>::detect();

	static constexpr auto detect() noexcept
	{
		return constexpr_array<size_t, fields_count, types_indexes_filler>();
	}

	struct types_indexes_sorter;
	static constexpr constexpr_array<size_t, TypeList::size, types_indexes_sorter> indexes_of_types_sorted_by_size {};
//private:
	struct types_indexes_sorter
	{
		template<typename T>
		static constexpr void fill(T* values, size_t size) noexcept
		{
			fill_impl(values, size, std::make_index_sequence<TypeList::size>{});
		}

		template<typename T, size_t... Is>
		static constexpr void fill_impl(T* values, size_t size, std::index_sequence<Is...>) noexcept
		{
			std::pair<size_t, size_t> type_sizes[]{ {Is, sizeof(type_list_element_t<Is, TypeList>) }... };
			for (int i = 1; i < static_cast<int>(size); ++i)
			{
				const auto x = type_sizes[i];
				int j = i - 1;
				while (j >= 0 && x.second < type_sizes[j].second)
				{
					type_sizes[j + 1].first = type_sizes[j].first;
					type_sizes[j + 1].second = type_sizes[j].second;
					--j;
				}
				type_sizes[j + 1].first = x.first;
				type_sizes[j + 1].second = x.second;
			}
			static_cast<void>(std::initializer_list<int>{((values[Is] = type_sizes[Is].first), 0)...});
		}
	};

	struct types_indexes_filler
	{
		template<typename T>
		static constexpr void fill(T* values, size_t size) noexcept
		{
			static_cast<void>(size);
			fill_impl(values, std::make_index_sequence<fields_count>{});
		}

		template<size_t I>
		using is_inaccurate = std::is_trivially_constructible<type_list_element_t<I, TypeList>>;

		template<typename T, size_t... Is>
		static constexpr void fill_impl(T* values, std::index_sequence<Is...>) noexcept
		{
			static_assert(sizeof...(Is) == fields_count, "incorrect tuple size");
			constexpr auto inaccurate_indexes = not_trivially_constructible_pod_fields_type_inaccurate_detector<PodType, TypeList>::detect();
			static_cast<void>(std::initializer_list<int>{(( values[Is] = get_real_index<Is, inaccurate_indexes[Is]>(is_inaccurate<inaccurate_indexes[Is]>{})), 0)...});
		}

		template<size_t FieldIndex, size_t AccurateIndex>
		static constexpr size_t get_real_index(std::false_type)
		{
			return AccurateIndex;
		}

		template<size_t FieldIndex, size_t InaccurateIndex>
		static constexpr size_t get_real_index(std::true_type)
		{
			return get_real_index_impl<FieldIndex>(
				std::make_index_sequence<indexes_of_types_sorted_by_size.size()>{},
				std::make_index_sequence<FieldIndex>{},
				make_index_sequence_range<FieldIndex + 1, fields_count>()
			);
		}

		template<size_t FieldIndex, size_t... Is0, size_t... Is1, size_t... Is2>
		static constexpr size_t get_real_index_impl(std::index_sequence<Is0...>, std::index_sequence<Is1...>, std::index_sequence<Is2...>)
		{
			return get_real_index_impl<FieldIndex>(
				std::make_index_sequence<sizeof...(Is0) - 1>{},
				std::make_index_sequence<FieldIndex>{},
				make_index_sequence_range<FieldIndex + 1, fields_count>()
			);
		}

		template<size_t FieldIndex, size_t I0, size_t... Is0, size_t... Is1, size_t... Is2>
		static constexpr std::enable_if_t < std::is_same < PodType, decltype(PodType{ as_any_type<Is1>{}..., type_list_element<indexes_of_types_sorted_by_size[sizeof...(Is0)], TypeList>::get(), as_any_type<Is2>{}... }) > ::value, size_t >
			get_real_index_impl(std::index_sequence<I0, Is0...>,  std::index_sequence<Is1...>, std::index_sequence<Is2...>)
		{
			return indexes_of_types_sorted_by_size[sizeof...(Is0)];
		}
	};
};