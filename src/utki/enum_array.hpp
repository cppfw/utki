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

#include <array>

#include "enum_iterable.hpp"
#include "type_traits.hpp"
#include "views.hpp"

namespace utki {

/**
 * @brief Array indexed by enumeration.
 * @tparam element_type - element type of the array.
 * @tparam enum_type - enumeration type which will index the array.
 *                     Must be scoped enum.
 *                     Must define 'enum_size' item as its last item.
 */
// TODO: doxygen all
template <typename element_type, typename enum_type>
class enum_array : public std::array<element_type, size_t(enum_type::enum_size)>
{
	static_assert(utki::is_scoped_enum_v<enum_type>, "enum_type must be a scoped enumeration type");

	// check that enum has enum_size item. Note, that this is the only way to do it for MSVC compiler at the moment.
	static_assert(
		enum_type::enum_size == enum_type::enum_size,
		"enum_type must have enum_type::enum_size as its last item"
	);

public:
	using base_type = std::array<element_type, size_t(enum_type::enum_size)>;

	auto zip_with_enum()
	{
		return utki::views::zip(
			*this, //
			utki::enum_iterable_v<enum_type>
		);
	}

	auto zip_with_enum() const
	{
		return utki::views::zip(
			*this, //
			utki::enum_iterable_v<enum_type>
		);
	}

	using base_type::operator[];

	element_type& operator[](enum_type i) noexcept
	{
		ASSERT(size_t(i) < this->size())
		return this->operator[](size_t(i));
	}

	const element_type& operator[](enum_type i) const noexcept
	{
		ASSERT(size_t(i) < this->size())
		return this->operator[](size_t(i));
	}
};

}; // namespace utki
