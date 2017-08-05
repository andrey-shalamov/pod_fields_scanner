#pragma once

#include <utility>

constexpr size_t constexpr_pow(size_t base, size_t exp) noexcept
{
	size_t r = 1;
	for (size_t i = 0; i < exp; ++i) r *= base;
	return r;
}

//

template<typename T, size_t N, typename ConstexprFiller>
struct constexpr_array
{
	template<typename... Args>
	constexpr constexpr_array(Args&&... args) noexcept : values()
	{
		ConstexprFiller::fill<T>(values, N, std::forward<Args>(args)...);
	}
	constexpr T operator[](size_t i) const noexcept { return values[i]; }
	T values[N];
};

//template<typename T, size_t N, size_t M, typename ConstexprFiller>
//struct constexpr_matrix
//{
//	template<typename... Args>
//	constexpr constexpr_matrix(Args&&... args) noexcept : values()
//	{
//		ConstexprFiller::fill<T>(values, std::forward<Args>(args)...);
//	}
//	constexpr T (&operator[](size_t n))[M] const noexcept { return values[n]; }
//	T values[N][M];
//};

//

template<size_t MinIndex, size_t... Is>
constexpr auto make_index_sequence_range(std::index_sequence<Is...>) noexcept
{
	return std::index_sequence<(MinIndex + Is)...>{};
}

template<size_t MinIndex, size_t MaxIndex>
constexpr auto make_index_sequence_range() noexcept
{
	return make_index_sequence_range<MinIndex>(std::make_index_sequence<MaxIndex - MinIndex>{});
}

//

template<size_t>
struct as_any_type
{
	template<typename T>
	constexpr operator T() const noexcept;
};

template<typename T>
struct as_type
{
	constexpr operator T() const noexcept;
};

template<typename... Ts>
struct as_any_type_of : as_type<Ts...> {};