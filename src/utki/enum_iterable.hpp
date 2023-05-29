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

#include <iterator>
#include <type_traits>

namespace utki {

// TODO: doxygen all
template <typename enum_type>
class enum_iterable
{
	static_assert(std::is_enum_v<enum_type>, "enum type expected");

	// check that enum has enum_size item. Note, that this is the only way to do it for MSVC compiler at the moment.
	static_assert(
		enum_type::enum_size == enum_type::enum_size,
		"enum_type must have enum_type::enum_size as its last item"
	);

public:
	class iterator
	{
		friend class enum_iterable;

		enum_type e;

		constexpr iterator(enum_type begin) :
			e(begin)
		{}

	public:
		using iterator_category = std::input_iterator_tag;
		using difference_type = std::make_signed_t<std::underlying_type_t<enum_type>>;
		using value_type = enum_type;
		using reference = value_type;
		using pointer = void;

		constexpr bool operator!=(const iterator& i) const noexcept
		{
			return this->e != i.e;
		}

		constexpr value_type operator*() const noexcept
		{
			return this->e;
		}

		constexpr iterator& operator++() noexcept
		{
			this->e = enum_type(std::underlying_type_t<enum_type>(this->e) + 1);
			return *this;
		}
	};

	using reverse_iterator = std::reverse_iterator<iterator>;

	constexpr iterator begin() const noexcept
	{
		return iterator(enum_type(0));
	}

	constexpr iterator end() const noexcept
	{
		return iterator(enum_type::enum_size);
	}

	constexpr iterator cbegin() const noexcept
	{
		return this->begin();
	}

	constexpr iterator cend() const noexcept
	{
		return this->end();
	}

	constexpr reverse_iterator crbegin() const
	{
		return const_reverse_iterator(this->cend());
	}

	constexpr reverse_iterator crend() const
	{
		return const_reverse_iterator(this->cbegin());
	}

	constexpr reverse_iterator rbegin() const
	{
		return reverse_iterator(this->end());
	}

	constexpr reverse_iterator rend() const
	{
		return reverse_iterator(this->begin());
	}
};

template <typename enum_type>
constexpr enum_iterable<enum_type> enum_iterable_v{};

} // namespace utki
