/*
The MIT License (MIT)

utki - Utility Kit for C++.

Copyright (c) 2015-2024 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* ================ LICENSE END ================ */

/**
 * NOTE: this implementation is taken from this blog post:
 * https://tristanbrindle.com/posts/a-more-useful-compile-time-quicksort
 */

#pragma once

#include <iterator>

#include "config.hpp"

namespace utki {

#if CFG_CPP < 20
namespace sort_internal {

template <typename random_iter_type>
constexpr random_iter_type next(
	random_iter_type it,
	typename std::iterator_traits<random_iter_type>::difference_type n = 1
)
{
	return it + n;
}

template <typename random_iter_type>
constexpr auto distance(random_iter_type first, random_iter_type last)
{
	return last - first;
}

template <class forward_iter_1_type, class forward_iter_2_type>
constexpr void iter_swap(forward_iter_1_type a, forward_iter_2_type b)
{
	auto temp = std::move(*a);
	*a = std::move(*b);
	*b = std::move(temp);
}

template <class input_iter_type, class unary_predicate_type>
constexpr input_iter_type find_if_not(input_iter_type first, input_iter_type last, unary_predicate_type q)
{
	for (; first != last; ++first) {
		if (!q(*first)) {
			return first;
		}
	}
	return last;
}

template <class forward_iter_type, class unary_predicate_type>
constexpr forward_iter_type partition(forward_iter_type first, forward_iter_type last, unary_predicate_type p)
{
	first = utki::sort_internal::find_if_not(first, last, p);
	if (first == last) {
		return first;
	}

	for (forward_iter_type i = utki::sort_internal::next(first); i != last; ++i) {
		if (p(*i)) {
			utki::sort_internal::iter_swap(i, first);
			++first;
		}
	}
	return first;
}

template <class random_iter_type, class comparator_type = std::less<>>
constexpr void quick_sort(random_iter_type first, random_iter_type last, comparator_type cmp = comparator_type{})
{
	auto const n = utki::sort_internal::distance(first, last);
	if (n <= 1) {
		return;
	}
	auto const pivot = *utki::sort_internal::next(first, n / 2);
	auto const middle1 = utki::sort_internal::partition(first, last, [=](auto const& elem) {
		return cmp(elem, pivot);
	});
	auto const middle2 = utki::sort_internal::partition(middle1, last, [=](auto const& elem) {
		return !cmp(pivot, elem);
	});
	quick_sort(first, middle1, cmp); // assert(std::is_sorted(first, middle1, cmp));
	quick_sort(middle2, last, cmp); // assert(std::is_sorted(middle2, last, cmp));
}

} // namespace sort_internal
#endif // CFG_CPP < 20

/**
 * @brief sort algorithm.
 * This is a drop-in replacement for std::sort() to support sorting in compile time for C++17.
 * In C++20 the std::sort() is marked constexpr, so this function is not needed for C++20 and later.
 * This function is to be used only for sorting in compile time with C++17.
 * In all other cases, one should use std::sort().
 * @param first - range begin iterator.
 * @param last - range end iterator.
 * @param comp - comparator functor.
 */
template <class random_iter_type, class comparator_type = std::less<>>
constexpr void sort(random_iter_type first, random_iter_type last, comparator_type comp = comparator_type{})
{
#if CFG_CPP >= 20
	std::sort(first, last, comp);
#else
	utki::sort_internal::quick_sort(first, last, comp);
#endif
}

} // namespace utki
