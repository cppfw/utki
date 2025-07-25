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

#include "base64.hpp"

#include <array>
#include <stdexcept>

#include "debug.hpp"
#include "type_traits.hpp"

using namespace utki;

namespace {
constexpr const std::array<uint8_t, size_t(std::numeric_limits<uint8_t>::max()) + 1> decode_table{
	0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	0,    0,    0,    0,    0,    0x3E, 0,    0,    0,    0x3F, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C,
	0x3D, 0,    0,    0,    0,    0,    0,    0,    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
	0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0,    0,    0,    0,
	0,    0,    0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A,
	0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	0,    0,    0,    0,    0,    0,    0,    0,    0
};

std::array<uint8_t, 3> base64_decode_quad(char a, char b, char c, char d)
{
	constexpr auto bits_per_char = 6;
	uint32_t quad = //
		(decode_table[a] << bits_per_char * 3) | //
		(decode_table[b] << bits_per_char * 2) | //
		(decode_table[c] << bits_per_char) | //
		decode_table[d];

	return {
		uint8_t((quad >> utki::byte_bits * 2) & utki::byte_mask), //
		uint8_t((quad >> utki::byte_bits) & utki::byte_mask),
		uint8_t(quad & utki::byte_mask)
	};
}
} // namespace

std::vector<uint8_t> utki::base64_decode(std::string_view str)
{
	if (str.empty()) {
		return {};
	}

	size_t num_quads = str.size() / 4;

	// strip out padding
	if (str.back() == '=') {
		if (num_quads == 0) {
			throw std::invalid_argument("base64_decode(): malformed base64 string: padding is not complete");
		}
		--num_quads;

		str = str.substr(0, str.size() - 1);

		if (str.back() == '=') {
			str = str.substr(0, str.size() - 1);
		}
	}

	std::vector<uint8_t> ret;

	auto i = str.begin();
	for (size_t counter = 0; counter != num_quads; ++counter) {
		ASSERT(i < str.end())
		ASSERT(std::next(i, 4) <= str.end())
		auto a = *i;
		i = std::next(i);
		auto b = *i;
		i = std::next(i);
		auto c = *i;
		i = std::next(i);
		auto d = *i;
		i = std::next(i);
		auto bytes = base64_decode_quad(a, b, c, d);

		ret.insert(ret.end(), bytes.begin(), bytes.end());
	}

	ASSERT(i <= str.end())

	if (i == str.end()) {
		return ret;
	}

	auto a = *i;
	i = std::next(i);
	ASSERT(i != str.end())
	auto b = *i;
	i = std::next(i);

	if (i == str.end()) {
		auto bytes = base64_decode_quad(a, b, 'A', 'A');
		ret.push_back(bytes[0]);
		return ret;
	}

	auto c = *i;
	auto bytes = base64_decode_quad(a, b, c, 'A');
	ret.push_back(bytes[0]);
	ret.push_back(bytes[1]);

	return ret;
}
