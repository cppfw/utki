/*
The MIT License (MIT)

utki - Utility Kit for C++.

Copyright (c) 2015-2025 Ivan Gagis <igagis@gmail.com>

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

#pragma once

#include "utility.hpp"

namespace utki {
/**
 * @brief Not to be used directly.
 * Use skip_front<num_to_skip>(<collection>) function.
 */
template <size_t num_to_skip, typename collection_type>
struct skip_front_collection_wrapper {
	collection_type& collection;

	skip_front_collection_wrapper(collection_type& collection) :
		collection(collection)
	{}

	auto begin()
	{
		ASSERT(this->collection.size() >= num_to_skip)
		using std::begin;
		return utki::next(begin(this->collection), num_to_skip);
	}

	auto end()
	{
		using std::end;
		return end(this->collection);
	}
};

/**
 * @brief Create collection wrapper which effectively skips first elements.
 * @tparam num_to_skip - number of front elements to skip.
 * @tparam collection_type - collection type.
 * @param collection - collection to skip first elements of.
 * @return A special wrapper class which provides begin() and end() methods
 *         returning collection iterators.
 */
// TODO: rename to utki::views::drop as in std::
template <size_t num_to_skip, typename collection_type>
auto skip_front(collection_type& collection)
{
	return skip_front_collection_wrapper<num_to_skip, collection_type>(collection);
}
} // namespace utki

namespace utki::views {

/**
 * @brief Not to be used directly.
 * Use utki::views::reverse(<collection>) function.
 */
template <typename collection_type>
struct reverse_view {
	collection_type& collection;

	reverse_view(collection_type& collection) :
		collection(collection)
	{}

	auto begin()
	{
		using std::rbegin;
		return rbegin(this->collection);
	}

	auto end()
	{
		using std::rend;
		return rend(this->collection);
	}
};

/**
 * @brief Drop-in replacement for std::views::reverse.
 * The reverse_view returns reverse iterators for begin() and end().
 * @tparam collection_type - collection type.
 * @param collection - collection with reversed elements order.
 * @return A special wrapper class which provides begin() and end() methods
 *         returning collection iterators.
 */
template <typename collection_type>
auto reverse(collection_type& collection)
{
	return reverse_view<collection_type>(collection);
}

/**
 * @brief Drop-in replacement for std::ranges::zip_view from C++23.
 * C++17 compatible.
 */
template <typename... collection_type>
struct zip_view {
	std::tuple<collection_type&...> collection;

	size_t min_size;

	zip_view(collection_type&... collection) :
		collection(std::tuple<collection_type&...>(collection...)),
		min_size(std::min({collection.size()...}))
	{}

	class iterator
	{
		std::tuple< //
			std::conditional_t<
				std::is_const_v<collection_type>,
				typename collection_type::const_iterator,
				typename collection_type::iterator>...>
			iters;

	public:
		iterator(decltype(iters) iters) :
			iters(iters)
		{}

		bool operator!=(const iterator& i) const noexcept
		{
			return std::get<0>(iters) != std::get<0>(i.iters);
		}

		iterator& operator++() noexcept
		{
			this->iters = std::apply(
				[](auto&... i) {
					return std::make_tuple(std::next(i)...);
				},
				this->iters
			);

			return *this;
		}

		auto operator*() noexcept
		{
			return std::apply(
				[](auto&... i) {
					return std::tuple<decltype(*i)...>((*i)...);
				},
				this->iters
			);
		}
	};

	auto begin()
	{
		return iterator(std::apply(
			[](auto&... c) {
				return std::make_tuple([&]() {
					if constexpr (std::is_const_v<std::remove_reference_t<decltype(c)>>) {
						return c.cbegin();
					} else {
						return c.begin();
					}
				}()...);
			},
			this->collection
		));
	}

	auto end()
	{
		return iterator(std::apply(
			[this](auto&... c) {
				return std::make_tuple(utki::next(c.begin(), this->min_size)...);
			},
			this->collection
		));
	}
};

/**
 * @brief Drop-in replacement for std::views::zip from C++23.
 * C++17 compatible.
 */
template <typename... collection_type>
zip_view<collection_type...> zip(collection_type&... collection)
{
	return zip_view<collection_type...>(collection...);
}

} // namespace utki::views
