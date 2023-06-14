/*
The MIT License (MIT)

Copyright (c) 2015-2023 Ivan Gagis <igagis@gmail.com>

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

#include "config.hpp"

#if CFG_COMPILER != CFG_COMPILER_MSVC || CFG_COMPILER_MSVC_TOOLS_V >= 142

#	include <functional>
#	include <vector>
#	include <map>
#	include <algorithm>

#	include "debug.hpp"
#	include "types.hpp"
#	include "config.hpp"

namespace utki {

template <typename collection_type>
class linq_collection_aggregator
{
	template <typename other_collection_type>
	friend class linq_collection_aggregator;

	template <typename other_collection_type>
	friend linq_collection_aggregator<const other_collection_type&> linq(const other_collection_type&);

	template <typename other_collection_type>
	friend linq_collection_aggregator<const other_collection_type&> linq(other_collection_type&);

	template <typename other_collection_type>
	friend linq_collection_aggregator<other_collection_type&&> linq(other_collection_type&&);

	typename std::conditional_t< //
		std::is_rvalue_reference_v<collection_type>,
		typename std::remove_reference_t<collection_type>,
		collection_type>
		// NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
		collection;

	using value_type = typename std::remove_reference<decltype(collection)>::type::value_type;

	struct noncopyable_value_type : public value_type {
		noncopyable_value_type(const noncopyable_value_type&) = delete;
		noncopyable_value_type& operator=(const noncopyable_value_type&) = delete;

		noncopyable_value_type(noncopyable_value_type&&) = delete;
		noncopyable_value_type& operator=(noncopyable_value_type&&) = delete;

		~noncopyable_value_type() = default;
	};

	linq_collection_aggregator(collection_type collection) :
		collection(std::move(collection))
	{}

public:
	decltype(collection) get()
	{
		return std::move(this->collection);
	}

	template <typename func_type>
	auto select(func_type func)
	{
		using func_arg_type = typename std::add_rvalue_reference_t<value_type>;

		static constexpr bool func_one_arg = std::is_invocable_v<func_type, func_arg_type>;

		static_assert(
			func_one_arg || std::is_invocable_v<func_type, func_arg_type, size_t>,
			"passed in function must have one argument or two arguments with the second argument of type size_t"
		);

		using func_return_type = typename std::conditional_t<
			func_one_arg,
			type_or_void_t<std::invoke_result<func_type, func_arg_type>>,
			type_or_void_t<std::invoke_result<func_type, func_arg_type, size_t>>>;

		std::vector<func_return_type> ret;
		ret.reserve(this->collection.size());

		if constexpr (func_one_arg) {
			for (auto&& e : this->collection) {
				ret.push_back(func(std::move(e)));
			}
		} else { // two arguments
			size_t i = 0;
			for (auto&& e : this->collection) {
				ret.push_back(func(std::move(e), i));
				++i;
			}
		}

		return linq_collection_aggregator<decltype(ret)&&>(std::move(ret));
	}

	template <typename func_type>
	auto group_by(func_type func)
	{
		static_assert(
			// non-const lvalue reference cannot bind to temproray objects,
			// thus func_type(auto&) will fail the assert, but func_type(const auto&) succeeds
			std::is_invocable_v<func_type, noncopyable_value_type>,
			"functor must have const reference argument"
		);

		static_assert(
			!std::is_same_v<void, std::invoke_result_t<func_type, const value_type&>>,
			"functor must return non-void"
		);

		using func_arg_type = typename std::add_lvalue_reference_t<value_type>;
		using func_return_type = typename std::remove_reference_t< //
			typename std::invoke_result_t<func_type, func_arg_type>>;

		std::map<func_return_type, std::vector<value_type>> ret;

		for (auto&& v : this->collection) {
			ret[func(v)].push_back(std::move(v));
		}

		return linq_collection_aggregator<decltype(ret)&&>(std::move(ret));
	}

	template <typename func_type>
	auto where(func_type func)
	{
		static_assert(
			// non-const lvalue reference cannot bind to temproray objects,
			// thus func_type(auto&) will fail the assert, but func_type(const auto&) succeeds
			std::is_invocable_v<func_type, noncopyable_value_type>,
			"functor must have const reference argument"
		);

		static_assert(
			std::is_same<bool, std::invoke_result_t<func_type, const value_type&>>::value,
			"functor must return bool"
		);

		typename utki::remove_const_reference<decltype(this->collection)>::type ret;

		std::copy_if(
			std::make_move_iterator(this->collection.begin()),
			std::make_move_iterator(this->collection.end()),
			std::inserter(ret, ret.end()),
			func
		);

		return linq_collection_aggregator<decltype(ret)&&>(std::move(ret));
	}

	template <typename func_type>
	auto order_by(func_type func)
	{
		static_assert(
			// non-const lvalue reference cannot bind to temproray objects,
			// thus func_type(auto&) will fail the assert, but func_type(const auto&) succeeds
			std::is_invocable_v<func_type, noncopyable_value_type>,
			"functor must have const reference argument"
		);

		using invoke_result = std::invoke_result_t<func_type, const value_type&>;

		static_assert(
			std::is_reference_v<invoke_result> && std::is_const_v<typename std::remove_reference_t<invoke_result>>,
			"functor must return const reference"
		);

		auto comparer = [&func](const value_type& a, const value_type& b) {
			return func(a) < func(b);
		};

		if constexpr (std::is_rvalue_reference_v<collection_type>) {
			std::sort(this->collection.begin(), this->collection.end(), comparer);
			return linq_collection_aggregator<decltype(this->collection)&&>(std::move(this->collection));
		} else {
			std::vector<value_type> ret;
			std::copy(this->collection.begin(), this->collection.end(), std::back_inserter(ret));

			std::sort(ret.begin(), ret.end(), comparer);
			return linq_collection_aggregator<decltype(ret)&&>(std::move(ret));
		}
	}
};

template <typename collection_type>
linq_collection_aggregator<const collection_type&> linq(const collection_type& collection)
{
	return linq_collection_aggregator<const collection_type&>(collection);
}

template <typename collection_type>
linq_collection_aggregator<const collection_type&> linq(collection_type& collection)
{
	return linq_collection_aggregator<const collection_type&>(collection);
}

template <typename collection_type>
linq_collection_aggregator<collection_type&&> linq(collection_type&& collection)
{
	return linq_collection_aggregator<collection_type&&>(std::forward<collection_type>(collection));
}

} // namespace utki

#endif // ~ non-MSVC compiler or MSVC compiler tools >= v142
