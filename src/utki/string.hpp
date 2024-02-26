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

#include <array>
#include <charconv>
#include <string>
#include <vector>

#include "config.hpp"
#include "span.hpp"

namespace utki {

/**
 * @brief Format string.
 * @param format - printf-like string format.
 * @return formatted string.
 */
#if CFG_COMPILER != CFG_COMPILER_MSVC
__attribute__((format(printf, 1, 2)))
#endif
std::string
make_string(const char* format, ...);

/**
 * @brief Format string.
 * va_list version of make_string().
 * @param format - printf-like string format.
 * @param args - va_list of string arguments.
 * @return formatted string.
 */
std::string make_string_va_list(const char* format, va_list args);

/**
 * @brief Make string out of char buffer.
 * @param buf - char buffer to make the string from.
 * @return string representing the contents of the char buffer.
 */
inline std::string make_string(utki::span<const char> buf)
{
	return std::string{buf.data(), buf.size()};
}

/**
 * @brief Make string_view out of char buffer.
 * @param buf - char buffer to make the string_view from.
 * @return string_view pointing to the contents of the char buffer.
 */
inline std::string_view make_string_view(utki::span<const char> buf)
{
	return std::string_view{buf.data(), buf.size()};
}

/**
 * @brief Make string_view out of char buffer.
 * @param buf - char buffer to make the string_view from.
 * @return string_view pointing to the contents of the char buffer.
 */
inline std::string_view make_string_view(utki::span<const uint8_t> buf)
{
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	return std::string_view{reinterpret_cast<const char*>(buf.data()), buf.size()};
}

/**
 * @brief Make string out of uint8_t buffer.
 * @param buf - uint8_t buffer to make the string from.
 * @return string representing the contents of the uint8_t buffer.
 */
inline std::string make_string(utki::span<const uint8_t> buf)
{
	return std::string(utki::make_string_view(buf));
}

/**
 * @brief Make string out of char buffer.
 * @param buf - char buffer to make the string from.
 * @return string representing the contents of the char buffer.
 */
template <size_t array_size>
inline std::string make_string(const std::array<char, array_size>& buf)
{
	return utki::make_string(utki::make_span(buf));
}

/**
 * @brief Make string out of uint8_t buffer.
 * @param buf - uint8_t buffer to make the string from.
 * @return string representing the contents of the uint8_t buffer.
 */
template <size_t array_size>
inline std::string make_string(const std::array<uint8_t, array_size>& buf)
{
	return utki::make_string(utki::make_span(buf));
}

/**
 * @brief Make string out of char buffer.
 * @param buf - char buffer to make the string from.
 * @return string representing the contents of the char buffer.
 */
inline std::string make_string(const std::vector<char>& buf)
{
	return utki::make_string(utki::make_span(buf));
}

/**
 * @brief Make string out of uint8_t buffer.
 * @param buf - uint8_t buffer to make the string from.
 * @return string representing the contents of the uint8_t buffer.
 */
inline std::string make_string(const std::vector<uint8_t>& buf)
{
	return utki::make_string(utki::make_span(buf));
}

/**
 * @brief Split string view using given delimiter.
 * @param str - string view to split.
 * @param delimiter - delimiter character to use as a splitter.
 * @return vector of splitted strings.
 */
template <typename element_type>
std::vector<std::basic_string<element_type>> split(std::basic_string_view<element_type> str, element_type delimiter)
{
	std::vector<std::basic_string<element_type>> ret;
	size_t pos = 0;

	for (;;) {
		auto dpos = str.find(delimiter, pos);

		if (dpos == std::string::npos) {
			ret.emplace_back(str.substr(pos, str.size() - pos));
			break;
		}

		ret.emplace_back(str.substr(pos, dpos - pos));

		pos = dpos + 1;
	}

	return ret;
}

/**
 * @brief Split string using given delimiter.
 * @param str - string to split.
 * @param delimiter - delimiter character to use as a splitter.
 * @return vector of splitted strings.
 */
template <typename char_type>
std::vector<std::basic_string<char_type>> split( //
	const std::basic_string<char_type>& str,
	char_type delimiter
)
{
	return split(std::basic_string_view<char_type>(str), delimiter);
}

/**
 * @brief Split string using given delimiter.
 * @param str - string to split.
 * @param delimiter - delimiter character to use as a splitter.
 * @return vector of splitted strings.
 */
template <typename element_type>
std::vector<std::basic_string<element_type>> split(const element_type* str, element_type delimiter)
{
	return split(std::basic_string_view<element_type>(str), delimiter);
}

/**
 * @brief Split string to separate words.
 * Delimiter for splitting is any sequence of whitespaces.
 * Whitespace includes space, tab, new line characters.
 * @param str - string to split to words.
 * @return vector of words.
 */
std::vector<std::string> split(std::string_view str);

/**
 * @brief Word wrap string.
 * @param str - string to word wrap.
 * @param width - maximum number of characters per line.
 * @return list of word wrapped lines.
 */
std::vector<std::string> word_wrap(std::string_view str, unsigned width);

// TODO: doxygen
enum class integer_base {
	// WARNING: add new items to the bottom of the enum to preserve existing item's values
	bin,
	oct,
	dec,
	hex
};

// TODO: doxygen
inline constexpr int to_int(integer_base conversion_base)
{
	const int bin_base = 2;
	const int oct_base = 8;
	const int dec_base = 10;
	const int hex_base = 16;

	switch (conversion_base) {
		case integer_base::bin:
			return bin_base;
		case integer_base::oct:
			return oct_base;
		default:
			[[fallthrough]];
		case integer_base::dec:
			return dec_base;
		case integer_base::hex:
			return hex_base;
	}
}

/**
 * @brief Format for from_chars().
 * This is to be removed when std::chars_format is widely supported by compilers.
 */
enum class chars_format {
	scientific,
	fixed,
	hex,
	general
};

/**
 * @brief Convert chars to float.
 * This is to be removed when std::from_chars(float) is widely supported by compilers.
 * @param first - chars start.
 * @param last - chars end.
 * @param value - the output value.
 * @param fmt - conversion format.
 * @return covnersion result.
 */
std::from_chars_result from_chars( //
	const char* first,
	const char* last,
	float& value,
	chars_format fmt = chars_format::general
) noexcept;

/**
 * @brief Convert chars to double.
 * This is to be removed when std::from_chars(double) is widely supported by compilers.
 * @param first - chars start.
 * @param last - chars end.
 * @param value - the output value.
 * @param fmt - conversion format.
 * @return covnersion result.
 */
std::from_chars_result from_chars( //
	const char* first,
	const char* last,
	double& value,
	chars_format fmt = chars_format::general
) noexcept;

class string_parser
{
	std::string_view view;

	void throw_if_empty() const;

public:
	static bool is_space(char c);

	string_parser(std::string_view view) :
		view(view)
	{}

	std::string_view get_view() const noexcept
	{
		return this->view;
	}

	void skip_whitespaces();
	void skip_whitespaces_and_comma();
	void skip_inclusive_until(char c);

	/**
	 * @brief Skip until one of given chars inclusively.
	 * @param c - chars to skip until.
	 * @return the char which stopped the skipping. Can be '\0' in case of end of string reached.
	 */
	char skip_inclusive_until_one_of(utki::span<const char> c);

	std::string_view read_word();
	std::string_view read_word_until(char c);

	// skips leading whitespaces
	template <class number_type>
	number_type read_number()
	{
		this->skip_whitespaces();

		number_type ret = 0;

		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
		std::from_chars_result res;

		if constexpr (std::is_floating_point<number_type>::value) {
			// TODO: use std::from_chars for floats when it is widely supported by C++17 compilers,
			// so if constexpr will not be needed.
			if constexpr (std::is_same<long double, number_type>::value) {
				return number_type(this->read_number<double>());
			} else {
				res = utki::from_chars(this->view.data(), this->view.data() + this->view.size(), ret);
			}
		} else {
			int base = to_int(integer_base::dec);

			if constexpr (std::is_unsigned_v<number_type>) {
				// detect base
				if (this->peek_char() == '0') {
					this->read_char();
					if (this->empty()) {
						return number_type(0);
					}
					char c = this->peek_char();
					switch (c) {
						case 'x':
							if (this->size() < 2) {
								return number_type(0);
							}
							c = this->peek_char(1);
							if (('0' <= c && c <= '9') || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F')) {
								base = to_int(integer_base::hex);
								this->read_char();
							} else {
								return number_type(0);
							}
							break;
						case 'b':
							if (this->size() < 2) {
								return number_type(0);
							}
							c = this->peek_char(1);
							if (c == '0' || c == '1') {
								base = to_int(integer_base::bin);
								this->read_char();
							} else {
								return number_type(0);
							}
							break;
						default:
							if ('0' <= c && c <= '7') {
								base = to_int(integer_base::oct);
							} else if (c < '0' || '9' < c) {
								return number_type(0);
							}
							break;
					}
				}
			}

			res = std::from_chars(this->view.data(), this->view.data() + this->view.size(), ret, base);
		}

		if (res.ec == std::errc::invalid_argument) {
			throw std::invalid_argument("string_parser::read_integer(): could not parse number");
		}

		if (res.ec == std::errc::result_out_of_range) {
			throw std::invalid_argument("string_parser::read_integer(): parsed number does not fit into requested type"
			);
		}

		ASSERT(this->view.data() != res.ptr)

		this->view = this->view.substr(res.ptr - this->view.data());

		return ret;
	}

	/**
	 * @brief Read character at current parser position.
	 * The parser position is avanced one character further.
	 * @return Character at current parser position.
	 * @throw std::invalid_argument in case end of string is reached.
	 */
	char read_char();

	/**
	 * @brief Get current character without advancing parser position.
	 * @return Character at the current parser position.
	 * @throw std::invalid_argument in case end of string is reached.
	 */
	char peek_char() const;

	/**
	 * @brief Peek n-th character.
	 * This method doesn't move the parser position.
	 * @param n - position of the character to peek.
	 * @return Character at n-th position from the current parser's position.
	 */
	char peek_char(size_t n) const;

	/**
	 * @brief Read N characters.
	 * @param n - number of characters to read.
	 * @return Read characters. It can be less than requested in case string has ended before
	 *     reading requested number of characters.
	 */
	std::string_view read_chars(size_t n);

	/**
	 * @brief Read until specified character.
	 * Read until specified character, not including the character,
	 * or till the end of the string, whatever is encountered first.
	 * @param c - character to read until.
	 * @return Read characters.
	 */
	std::string_view read_chars_until(char c);

	/**
	 * @brief Check if parser has reached end of string.
	 * @return true in case end of string is reached.
	 * @return false otherwise.
	 */
	bool empty() const noexcept
	{
		return this->view.empty();
	}

	/**
	 * @brief Number of characters left to parse.
	 * @return Number of characters left to parse.
	 */
	size_t size() const noexcept
	{
		return this->view.size();
	}
};

/**
 * @brief Locale-independent version of std::to_string().
 * @param value - Number value to convert to string.
 * @param conversion_base - Integer base to use for the conversion.
 * @return String representing the converted integer value.
 */
template <typename number_type>
std::string to_string(number_type value, integer_base conversion_base = integer_base::dec)
{
	// 128 chars is large enough to hold any built-in integral or floating point type
	constexpr size_t buf_size = 128;
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init, cppcoreguidelines-pro-type-vararg)
	std::array<char, buf_size> buf;
	// under MSVC compiler the &*buf.end() is not a pointer to the past-the-end of the buffer,
	// so we wrap the buffer to span and use the span's iterators
	auto span = utki::make_span(buf);
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
	auto begin = span.begin();
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
	auto end = span.end();

	if constexpr (std::is_unsigned_v<number_type>) {
		switch (conversion_base) {
			case integer_base::bin:
				buf[0] = '0';
				buf[1] = 'b';
				begin = std::next(begin, 2);
				break;
			case integer_base::oct:
				buf[0] = '0';
				begin = std::next(begin);
				break;
			case integer_base::hex:
				buf[0] = '0';
				buf[1] = 'x';
				begin = std::next(begin, 2);
				break;
			default:
				break;
		}
	}

	auto res = std::to_chars(&*begin, &*end, value, to_int(conversion_base));

	if (res.ec != std::errc()) {
		// std::to_chars() returned error
		return {};
	}

	ASSERT(res.ptr <= &*end)

	return std::string(buf.data(), res.ptr - buf.data());
}

/**
 * @brief Concatenate strings.
 * Concatenates values which can be streamed to an std::ostream using operator<<().
 * @tparam streamable_type - parameter pack of types of values to concatenate.
 * @param s - parameter pack of values to concatenate.
 * @return std::string of concatenated values.
 */
template <typename... streamable_type>
std::string cat(const streamable_type&... s)
{
	std::stringstream ss;

	(ss << ... << s);

	return ss.str();
}

/**
 * @brief Trim whitespaces from front of the string.
 * @tparam element_type - string element type.
 * @param s - string view to trim whitespaces from.
 * @return string view with trimmed front whitespaces.
 */
template <typename element_type>
std::basic_string_view<element_type> trim_front(std::basic_string_view<element_type> s)
{
	return s.substr( //
		std::distance( //
			s.begin(),
			std::find_if( //
				s.begin(),
				s.end(),
				[](auto c) {
					return !std::isspace(c);
				}
			)
		)
	);
}

std::string_view trim_front(const char* s)
{
	return trim_front(make_string_view(utki::make_span(s)));
}

template <typename element_type>
std::basic_string_view<element_type> trim_front(const std::basic_string<element_type>& s)
{
	return trim_front(std::basic_string_view<element_type>(s));
}

/**
 * @brief Trim whitespaces from back of the string.
 * @tparam element_type - string element type.
 * @param s - string view to trim whitespaces from.
 * @return string view with trimmed back whitespaces.
 */
template <typename element_type>
std::basic_string_view<element_type> trim_back(std::basic_string_view<element_type> s)
{
	return s.substr( //
		0,
		std::distance( //
			s.begin(),
			std::find_if( //
				s.rbegin(),
				s.rend(),
				[](auto c) {
					return !std::isspace(c);
				}
			).base()
		)
	);
}

std::string_view trim_back(const char* s)
{
	return trim_back(make_string_view(utki::make_span(s)));
}

template <typename element_type>
std::basic_string_view<element_type> trim_back(const std::basic_string<element_type>& s)
{
	return trim_back(std::basic_string_view<element_type>(s));
}

/**
 * @brief Trim whitespaces from the string.
 * @tparam element_type - string element type.
 * @param s - string view to trim whitespaces from.
 * @return string view with trimmed front and back whitespaces.
 */
template <typename element_type>
std::basic_string_view<element_type> trim(std::basic_string_view<element_type> s)
{
	return trim_front(trim_back(s));
}

std::string_view trim(const char* s)
{
	return trim(make_string_view(utki::make_span(s)));
}

template <typename element_type>
std::basic_string_view<element_type> trim(const std::basic_string<element_type>& s)
{
	return trim(std::basic_string_view<element_type>(s));
}

} // namespace utki
