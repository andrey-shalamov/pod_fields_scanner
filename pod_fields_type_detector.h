#pragma once

#include <tuple>
#include <utility>

#include "pod_fields_count_detector.h"

template<typename PodType>
struct pod_fields_type_detector
{
	static constexpr size_t fields_count = pod_fields_count_detector<PodType>::detect();

	template<size_t I, typename T>
	struct type_detector
	{
		bool* values;
		constexpr type_detector(bool* b) noexcept : values(b) {}

		template<typename RealType>
		constexpr operator RealType() const noexcept
		{
			static_assert(I < fields_count, "incorrect index");
			values[I] = std::is_same<RealType, T>::value;
			return {};
		}
	};

	template<typename... Ts>
	static constexpr bool is_real_types(std::tuple<Ts...>) noexcept
	{
		static_assert(sizeof...(Ts) == fields_count, "incorrect tuple size");
		return is_real_types_impl(std::make_index_sequence<fields_count>{}, std::tuple<Ts...>{});
	}

	template<size_t... Is, typename... Ts>
	static constexpr bool is_real_types_impl(std::index_sequence<Is...>, std::tuple<Ts...>) noexcept
	{
		static_assert(sizeof...(Ts) == fields_count, "incorrect tuple size");
		bool indicators[fields_count]{};
		static_cast<void>(PodType{ type_detector<Is, Ts>{indicators}... });
		bool result = true;
		for (size_t i = 0; i < fields_count; ++i)
			result &= indicators[i];
		return result;
	}
};