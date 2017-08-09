#pragma once

#include "common.h"
#include "pod_fields_count_detector.h"
#include "pod_fields_type_detector.h"

template<typename PodType, typename TypeList>
struct pod_fields_scanner
{
	static constexpr auto fields_count = pod_fields_count_detector<PodType>::detect();
	static constexpr auto fields_type_indexes = not_trivially_constructible_pod_fields_type_accurate_detector<PodType, TypeList>::detect();

#ifdef _MSC_VER // workaround for MS VC++
	static constexpr auto detect_fields_types()
	{
		return detect_fields_types<fields_count>(std::integral_constant<bool, fields_count == 0>{});
	}

	template<size_t N, size_t... Is>
	static constexpr auto detect_fields_types(std::false_type)
	{
		return detect_fields_types<N - 1, fields_type_indexes[N - 1], Is...>(std::integral_constant<bool, N == 1>{});
	}

	template<size_t N, size_t... Is>
	static constexpr auto detect_fields_types(std::true_type)
	{
		return type_list<type_list_element_t<Is, TypeList>...>{};
	}
#else
	static constexpr auto detect_fields_types()
	{
		return detect_fields_types(std::make_index_sequence<fields_count>{});
	}

	template<size_t... Is>
	static constexpr auto detect_fields_types(std::index_sequence<Is...>)
	{
		return type_list<type_list_element_t<fields_type_indexes[Is], TypeList>...>{};
	}
#endif
};