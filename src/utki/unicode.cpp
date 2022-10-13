/*
The MIT License (MIT)

Copyright (c) 2015-2022 Ivan Gagis <igagis@gmail.com>

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
	end(str.end_pointer())
{
	this->operator++();
}

static_assert(sizeof(char) == sizeof(uint8_t), "unexpected char size");

utf8_iterator::utf8_iterator(const char* str) :
	p(reinterpret_cast<const uint8_t*>(str))
{
	this->operator++();
}

utf8_iterator& utf8_iterator::operator++() noexcept
{
	if (this->p == this->end) {
		this->c = 0;
		return *this;
	}

	uint8_t b = *this->p;
	++this->p;

	if ((b & 0x80) == 0) {
		this->c = char32_t(b);
		return *this;
	}

	if (this->p == this->end) {
		this->c = 0;
		return *this;
	}

	this->c = (*this->p) & 0x3f;
	++this->p;

	unsigned i = 2;
	for (; //
		 (uint8_t(b << i) >> 7);
		 ++i, ++this->p)
	{
		if (this->p == this->end) {
			this->c = 0;
			return *this;
		}

		this->c <<= 6;
		this->c |= (*this->p) & 0x3f;
	}
	this->c |= (char32_t(uint8_t(b << (i + 1)) >> (i + 1)) << (6 * (i - 1)));

	return *this;
}

std::array<char, max_size_of_utf8_encoded_character + 1> //
utki::to_utf8(char32_t c)
{
	std::array<char, 7> ret;

	if (c <= 0x7f) {
		ret[0] = char(c);
		ret[1] = 0;
		return ret;
	}

	unsigned num_bytes;

	if (c <= 0x7ff) {
		num_bytes = 2;
	} else if (c <= 0xffff) {
		num_bytes = 3;
	} else if (c <= 0x1fffff) {
		num_bytes = 4;
	} else if (c <= 0x3ffffff) {
		num_bytes = 5;
	} else if (c <= 0x7fffffff) {
		num_bytes = 6;
	} else {
		ret[0] = 0;
		return ret;
	}

	switch (num_bytes) {
		case 1:
			ASSERT(false)
			break;
		case 2:
			ret[0] = 0xc0 | ((c >> 6) & 0x1f); // 0xc0 = 0b11000000
			ret[1] = 0x80 | (c & 0x3f);
			ret[2] = 0;
			break;
		case 3:
			ret[0] = 0xe0 | ((c >> 12) & 0xf); // 0xe0 = 0b11100000
			ret[1] = 0x80 | ((c >> 6) & 0x3f);
			ret[2] = 0x80 | (c & 0x3f);
			ret[3] = 0;
			break;
		case 4:
			ret[0] = 0xf0 | ((c >> 18) & 0x7); // 0xf0 = 0b11110000
			ret[1] = 0x80 | ((c >> 12) & 0x3f);
			ret[2] = 0x80 | ((c >> 6) & 0x3f);
			ret[3] = 0x80 | (c & 0x3f);
			ret[4] = 0;
			break;
		case 5:
			ret[0] = 0xf8 | ((c >> 24) & 0x3); // 0xf8 = 0b11111000
			ret[1] = 0x80 | ((c >> 18) & 0x3f);
			ret[2] = 0x80 | ((c >> 12) & 0x3f);
			ret[3] = 0x80 | ((c >> 6) & 0x3f);
			ret[4] = 0x80 | (c & 0x3f);
			ret[5] = 0;
			break;
		case 6:
			ret[0] = 0xfc | ((c >> 30) & 0x1); // 0xfc = 0b11111100
			ret[1] = 0x80 | ((c >> 24) & 0x3f);
			ret[2] = 0x80 | ((c >> 18) & 0x3f);
			ret[3] = 0x80 | ((c >> 12) & 0x3f);
			ret[4] = 0x80 | ((c >> 6) & 0x3f);
			ret[5] = 0x80 | (c & 0x3f);
			ret[6] = 0;
			break;
	}

	return ret;
}

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
	return std::u32string(buf.begin(), buf.end());
}

std::u32string utki::to_utf32(utki::span<const uint8_t> str)
{
	utf8_iterator iter(str);

	std::vector<char32_t> buf;

	for (; !iter.is_end(); ++iter) {
		buf.push_back(iter.character());
	}

	return std::u32string(buf.begin(), buf.end());
}
