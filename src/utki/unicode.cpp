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

#include "unicode.hpp"

#include "debug.hpp"

using namespace utki;

utf8_iterator::utf8_iterator(utki::span<const uint8_t> str) :
	p(str.data()),
	end(&*str.end())
{
	this->operator++();
}

static_assert(sizeof(char) == sizeof(uint8_t), "unexpected char size");

utf8_iterator::utf8_iterator(const char* str) :
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	p(reinterpret_cast<const uint8_t*>(str))
{
	this->operator++();
}

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

utf8_iterator& utf8_iterator::operator++() noexcept
{
	if (this->p == this->end) {
		this->c = 0;
		return *this;
	}

	uint8_t b = *this->p;
	// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	++this->p;

	if ((b & 0x80) == 0) { // NOLINT(cppcoreguidelines-avoid-magic-numbers)
		this->c = char32_t(b);
		return *this;
	}

	if (this->p == this->end) {
		this->c = 0;
		return *this;
	}

	this->c = (*this->p) & 0x3f; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
	// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	++this->p;

	unsigned i = 2;
	for (; //
		 (uint8_t(b << i) >> 7); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
		 ++i,
		 // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		 ++this->p)
	{
		if (this->p == this->end) {
			this->c = 0;
			return *this;
		}

		this->c <<= 6; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
		this->c |= (*this->p) & 0x3f; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
	}
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
	this->c |= (char32_t(uint8_t(b << (i + 1)) >> (i + 1)) << (6 * (i - 1)));

	return *this;
}

std::array<char, max_size_of_utf8_encoded_character + 1> //
utki::to_utf8(char32_t c)
{
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	std::array<char, 7> ret; // NOLINT(cppcoreguidelines-avoid-magic-numbers)

	if (c <= 0x7f) { // NOLINT(cppcoreguidelines-avoid-magic-numbers)
		ret[0] = char(c); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
		ret[1] = 0; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
		return ret;
	}

	// NOLINTNEXTLINE(cppcoreguidelines-init-variables)
	unsigned num_bytes;

	if (c <= 0x7ff) { // NOLINT(cppcoreguidelines-avoid-magic-numbers)
		num_bytes = 2; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
	} else if (c <= 0xffff) { // NOLINT(cppcoreguidelines-avoid-magic-numbers)
		num_bytes = 3; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
	} else if (c <= 0x1fffff) { // NOLINT(cppcoreguidelines-avoid-magic-numbers)
		num_bytes = 4; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
	} else if (c <= 0x3ffffff) { // NOLINT(cppcoreguidelines-avoid-magic-numbers)
		num_bytes = 5; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
	} else if (c <= 0x7fffffff) { // NOLINT(cppcoreguidelines-avoid-magic-numbers)
		num_bytes = 6; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
	} else {
		ret[0] = 0; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
		return ret;
	}

	switch (num_bytes) {
		default:
		case 1: // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ASSERT(false)
			break;
		case 2: // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			// 0xc0 = 0b11000000
			ret[0] = char(0xc0 | ((c >> 6) & 0x1f)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[1] = char(0x80 | (c & 0x3f)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[2] = 0; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			break;
		case 3: // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			// 0xe0 = 0b11100000
			ret[0] = char(0xe0 | ((c >> 12) & 0xf)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[1] = char(0x80 | ((c >> 6) & 0x3f)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[2] = char(0x80 | (c & 0x3f)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[3] = 0; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			break;
		case 4: // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			// 0xf0 = 0b11110000
			ret[0] = char(0xf0 | ((c >> 18) & 0x7)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[1] = char(0x80 | ((c >> 12) & 0x3f)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[2] = char(0x80 | ((c >> 6) & 0x3f)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[3] = char(0x80 | (c & 0x3f)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[4] = 0; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			break;
		case 5: // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			// 0xf8 = 0b11111000
			ret[0] = char(0xf8 | ((c >> 24) & 0x3)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[1] = char(0x80 | ((c >> 18) & 0x3f)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[2] = char(0x80 | ((c >> 12) & 0x3f)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[3] = char(0x80 | ((c >> 6) & 0x3f)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[4] = char(0x80 | (c & 0x3f)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[5] = 0; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			break;
		case 6: // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			// 0xfc = 0b11111100
			ret[0] = char(0xfc | ((c >> 30) & 0x1)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[1] = char(0x80 | ((c >> 24) & 0x3f)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[2] = char(0x80 | ((c >> 18) & 0x3f)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[3] = char(0x80 | ((c >> 12) & 0x3f)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[4] = char(0x80 | ((c >> 6) & 0x3f)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[5] = char(0x80 | (c & 0x3f)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			ret[6] = 0; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
			break;
	}

	return ret;
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

std::string utki::to_utf8(utki::span<const char32_t> str)
{
	std::stringstream ss;

	for (auto c : str) {
		auto buf = to_utf8(c);
		ss << buf.data();
	}

	return ss.str();
}

std::u32string utki::to_utf32(utf8_iterator str)
{
	std::vector<char32_t> buf;
	for (; !str.is_end(); ++str) {
		buf.push_back(str.character());
	}
	return std::u32string{buf.begin(), buf.end()};
}

std::u32string utki::to_utf32(utki::span<const uint8_t> str)
{
	utf8_iterator iter(str);

	std::vector<char32_t> buf;

	for (; !iter.is_end(); ++iter) {
		buf.push_back(iter.character());
	}

	return std::u32string{buf.begin(), buf.end()};
}
