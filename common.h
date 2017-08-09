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
		ConstexprFiller::template fill<T>(values, N, std::forward<Args>(args)...);
	}
	constexpr T operator[](size_t i) const noexcept { return values[i]; }
	constexpr size_t size() const noexcept { return N; }
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

//

template<typename... Ts>
struct type_list
{
	static constexpr size_t size = sizeof...(Ts);
};

template<size_t I, typename T>
struct type_list_element;

template<typename T, typename... Ts>
struct type_list_element<0, type_list<T, Ts...>>
{
	using type = T;
	static constexpr T get(); // Undefined
};

template<size_t I, typename T, typename... Ts>
struct type_list_element<I, type_list<T, Ts...>>
{
	using type = typename type_list_element<I - 1, type_list<Ts...>>::type;
	static constexpr type get(); // Undefined
};

template<size_t I, typename TypeList>
using type_list_element_t = typename type_list_element<I, TypeList>::type;

