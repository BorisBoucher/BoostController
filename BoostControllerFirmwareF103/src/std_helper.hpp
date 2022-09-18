#pragma once

#include <algorithm>
#include <array>

#define sizeof_array(arr) (sizeof(arr) / sizeof(arr[0]))

// Not available in C++17, std::to_array
namespace std
{
namespace detail {
 
template <class T, std::size_t N, std::size_t... I>
constexpr std::array<std::remove_cv_t<T>, N>
    to_array_impl(T (&&a)[N], std::index_sequence<I...>)
{
    return { {std::move(a[I])...} };
}
 
} // namespace detail
 
template <class T, std::size_t N>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&&a)[N])
{
    return detail::to_array_impl(std::move(a), std::make_index_sequence<N>{});
}
} // namespace std

template <typename T, typename U, typename V>
T constrain(const T value, const U min, const V max)
{
	T ret;
	if (value < min)
	{
		ret = min;
	}
	else if (value > max)
	{
		ret = max;
	}
	else
	{
		ret = value;
	}

	return ret;
}
