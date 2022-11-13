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

#pragma once

#include <array>
#include <string>
#include <vector>

#include "span.hpp"

namespace utki {

/**
 * @brief Iterator to iterate through utf-8 encoded unicode characters.
 */
class utf8_iterator
{
	char32_t c = 0;
	const uint8_t* p = nullptr;
	const uint8_t* end = nullptr;

public:
	/**
	 * @brief Create undefined iterator.
	 */
	utf8_iterator() = default;

	utf8_iterator(utki::span<const uint8_t> str);

	/**
	 * @brief Create iterator pointing to the begin of the given utf-8 encoded string.
	 * @param str - pointer to the null-terminated utf-8 encoded string.
	 */
	utf8_iterator(const char* str);

	/**
	 * @brief Get current unicode character.
	 * @return unicode value of the character this interator is currently pointing to.
	 */
	char32_t character() const noexcept
	{
		return this->c;
	}

	// no operator*() because it usually returns reference, don't want to break this contract.

	/**
	 * @brief Prefix increment.
	 * Move iterator to the next character in the string.
	 * If iterator points to the end of the string before this operation then the result of this operation is undefined.
	 * @return reference to this iterator object.
	 */
	utf8_iterator& operator++() noexcept;

	// no postfix ++ operator, there is no need in it.

	/**
	 * @brief Check if iterator points to the end of the string.
	 * @return true if iterator points to the end of the string.
	 * @return false otherwise.
	 */
	bool is_end() const noexcept
	{
		return this->c == 0;
	}
};

/**
 * @brief Convert UTF-8 to UTF-32.
 * @param str - string to convert.
 * @return UTF-32 string.
 */
std::u32string to_utf32(utf8_iterator str);

/**
 * @brief Convert UTF-8 to UTF-32.
 * @param str - string to convert.
 * @return UTF-32 string.
 */
inline std::u32string to_utf32(const char* str)
{
	return to_utf32(utf8_iterator(str));
}

/**
 * @brief Convert UTF-8 to UTF-32.
 * @param str - string to convert.
 * @return UTF-32 string.
 */
inline std::u32string to_utf32(const std::string& str)
{
	return to_utf32(str.c_str());
}

/**
 * @brief Convert UTF-8 to UTF-32.
 * @param str - string to convert.
 * @return UTF-32 string.
 */
std::u32string to_utf32(utki::span<const uint8_t> str);

/**
 * @brief Convert UTF-8 to UTF-32.
 * @param str - string to convert.
 * @return UTF-32 string.
 */
inline std::u32string to_utf32(utki::span<const char> str)
{
	static_assert(sizeof(char) == sizeof(uint8_t), "unexpected char size");
	return to_utf32(utki::make_span( //
		reinterpret_cast<const uint8_t*>(str.data()),
		str.size()
	));
}

/**
 * @brief Convert UTF-8 to UTF-32.
 * @param str - string to convert.
 * @return UTF-32 string.
 */
inline std::u32string to_utf32(std::string_view str)
{
	return to_utf32(utki::make_span(str));
}

const unsigned max_size_of_utf8_encoded_character = 6;

/**
 * @brief Convert UTF-32 character to UTF-8.
 * @param c - character to convert.
 * @return Zero-terminated array of bytes representing a UTF-8 character.
 */
std::array<char, max_size_of_utf8_encoded_character + 1> to_utf8(char32_t c);

/**
 * @brief Convert UTF-32 string to UTF-8 string.
 * @param str - UTF-32 string to convert.
 * @return UTF-8 string.
 */
std::string to_utf8(utki::span<const char32_t> str);

/**
 * @brief Convert UTF-32 string to UTF-8 string.
 * @param str - UTF-32 string to convert.
 * @return UTF-8 string.
 */
inline std::string to_utf8(const std::u32string& str)
{
	return to_utf8(utki::make_span(str));
}

} // namespace utki
