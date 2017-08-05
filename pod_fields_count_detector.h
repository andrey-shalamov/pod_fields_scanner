#pragma once

#include <utility>
#include <tuple>
#include "common.h"

template<typename PodType>
struct pod_fields_count_detector
{
	static constexpr size_t detect() noexcept
	{
		return std::get<size_t>(detect_impl(std::make_index_sequence<sizeof(PodType)>{}));
	}

private:
	template<size_t... Is>
	static constexpr auto detect_impl(std::index_sequence<Is...>) noexcept
	{
		return detect_impl(std::make_index_sequence<sizeof...(Is)-1>{});
	}

	template<size_t I0, size_t... Is>
	static constexpr auto detect_impl(std::index_sequence<I0, Is...>) noexcept ->
		std::tuple < size_t, decltype(PodType{ as_any_type<I0>{}, as_any_type<Is>{}... }) >
	{
		return { 1 + sizeof...(Is), {} };
	}
};
