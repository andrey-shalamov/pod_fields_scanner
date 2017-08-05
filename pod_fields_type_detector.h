#pragma once

#include <tuple>
#include <utility>

#include "pod_fields_count_detector.h"

template<typename PodType, typename TypesTuple>
struct pod_fields_type_detector
{
	static constexpr size_t fields_count = pod_fields_count_detector<PodType>::detect();

	static constexpr auto detect() noexcept
	{
		return constexpr_array<size_t, fields_count, types_indexes_filler>();
	}

private:
	template<size_t FieldIndex>
	struct type_detector
	{
		static constexpr size_t not_found_index = static_cast<size_t>(-1);

		size_t* values;
		constexpr type_detector(size_t* v) noexcept : values(v) {}

		template<typename RealType>
		constexpr operator RealType() const noexcept
		{
			static_assert(FieldIndex < fields_count, "incorrect index");
			constexpr size_t type_index = get_type_index<RealType>(std::make_index_sequence<std::tuple_size<TypesTuple>::value>{});
			static_assert(type_index != not_found_index, "type index not found");
			values[FieldIndex] = type_index;
			return {};
		}

		template<typename T, size_t... Is>
		static constexpr size_t get_type_index(std::index_sequence<Is...>) noexcept
		{
			size_t result = not_found_index;
			std::initializer_list<int>{((result = std::is_same<T, std::tuple_element_t<Is, TypesTuple>>::value ? Is : result), 0)...};
			// TODO: need to process a case with duplicated types in tuple
			return result;
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

		template<typename T, size_t... Is>
		static constexpr void fill_impl(T* values, std::index_sequence<Is...>) noexcept
		{
			static_assert(sizeof...(Is) <= fields_count, "incorrect tuple size");
			static_cast<void>(PodType{ type_detector<Is>{values}... });
		}
	};
};