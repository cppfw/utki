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

#pragma once

#include "span.hpp"

namespace utki {

class deserializer
{
	utki::span<const uint8_t> data;

public:
	deserializer(utki::span<const uint8_t> data) :
		data(data)
	{}

	bool empty() const noexcept
	{
		return this->data.empty();
	}

	size_t size() const noexcept
	{
		return this->data.size();
	}

	std::string_view read_string(size_t length);

	utki::span<const uint8_t> read_span(size_t length);

	uint8_t read_uint8();

	uint16_t read_uint16_le();
	uint32_t read_uint32_le();
	uint64_t read_uint64_le();
	float read_float_le();

	uint16_t read_uint16_be();
	uint32_t read_uint32_be();
	uint64_t read_uint64_be();
	float read_float_be();

	void skip(size_t length);
};

} // namespace utki
