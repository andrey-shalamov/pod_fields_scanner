#pragma once

#include "common.h"
#include "pod_fields_count_detector.h"
#include "pod_fields_type_detector.h"

template<typename PodType, typename TypesTuple>
struct pod_fields_scanner
{
	static constexpr auto fields_count = pod_fields_count_detector<PodType>::detect();
	static constexpr auto fields_type_indexes = pod_fields_type_detector<PodType, TypesTuple>::detect();

	static constexpr auto detect_fields_types()
	{
		return detect_fields_types(std::make_index_sequence<fields_count>{});
	}

	template<size_t... Is>
	static constexpr auto detect_fields_types(std::index_sequence<Is...>)
	{
		return std::make_tuple(std::tuple_element_t<fields_type_indexes[Is], TypesTuple>{}...);
	}
};