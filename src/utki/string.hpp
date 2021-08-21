/*
The MIT License (MIT)

Copyright (c) 2015-2021 Ivan Gagis <igagis@gmail.com>

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

#include <string>
#include <vector>
#include <array>
#include <charconv>

#include "config.hpp"
#include "span.hpp"

namespace utki{

/**
 * @brief Format string.
 * @param format - printf-like string format.
 * @return formatted string.
 */
#if M_COMPILER != M_COMPILER_MSVC
__attribute__((format(printf, 1, 2)))
#endif
std::string make_string(const char* format, ...);

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
inline std::string make_string(utki::span<const char> buf){
	return std::string(buf.data(), buf.size());
}

/**
 * @brief Make string_view out of char buffer.
 * @param buf - char buffer to make the string_view from.
 * @return string_view pointing to the contents of the char buffer.
 */
inline std::string_view make_string_view(utki::span<const char> buf){
	return std::string_view(buf.data(), buf.size());
}

/**
 * @brief Make string out of uint8_t buffer.
 * @param buf - uint8_t buffer to make the string from.
 * @return string representing the contents of the uint8_t buffer.
 */
inline std::string make_string(utki::span<const uint8_t> buf){
	return utki::make_string(utki::make_span(reinterpret_cast<const char*>(buf.data()), buf.size()));
}

/**
 * @brief Make string out of char buffer.
 * @param buf - char buffer to make the string from.
 * @return string representing the contents of the char buffer.
 */
template <size_t S> inline std::string make_string(const std::array<char, S>& buf){
	return utki::make_string(utki::make_span(buf));
}

/**
 * @brief Make string out of uint8_t buffer.
 * @param buf - uint8_t buffer to make the string from.
 * @return string representing the contents of the uint8_t buffer.
 */
template <size_t S> inline std::string make_string(const std::array<uint8_t, S>& buf){
	return utki::make_string(utki::make_span(buf));
}

/**
 * @brief Make string out of char buffer.
 * @param buf - char buffer to make the string from.
 * @return string representing the contents of the char buffer.
 */
inline std::string make_string(const std::vector<char>& buf){
	return utki::make_string(utki::make_span(buf));
}

/**
 * @brief Make string out of uint8_t buffer.
 * @param buf - uint8_t buffer to make the string from.
 * @return string representing the contents of the uint8_t buffer.
 */
inline std::string make_string(const std::vector<uint8_t>& buf){
	return utki::make_string(utki::make_span(buf));
}

/**
 * @brief Split string using given delimiter.
 * @param str - string to split.
 * @param delimiter - delimiter character to use as a splitter.
 * @return vector of splitted strings.
 */
template <typename T> std::vector<std::basic_string<T>> split(std::basic_string_view<T> str, T delimiter){
	std::vector<std::basic_string<T>> ret;
	size_t pos = 0;

	for(;;){
		auto dpos = str.find(delimiter, pos);

		if(dpos == std::string::npos){
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
template <typename T> std::vector<std::basic_string<T>> split(const T* str, T delimiter){
	return split(std::basic_string_view<T>(str), delimiter);
}

/**
 * @brief Split string to separate words.
 * Delimiter for splitting is any sequence of whitespaces.
 * Whitespace includes space, tab, newline characters.
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

/**
 * @brief Format for from_chars().
 * This is to be removed when std::chars_format is widely supported by compilers.
 */
enum class chars_format{
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
std::from_chars_result from_chars(
		const char *first,
		const char *last,
		float& value,
		chars_format fmt = chars_format::general
	)noexcept;

/**
 * @brief Convert chars to double.
 * This is to be removed when std::from_chars(double) is widely supported by compilers.
 * @param first - chars start.
 * @param last - chars end.
 * @param value - the output value.
 * @param fmt - conversion format.
 * @return covnersion result.
 */
std::from_chars_result from_chars(
		const char *first,
		const char *last,
		double& value,
		chars_format fmt = chars_format::general
	)noexcept;

class string_parser{
	std::string_view view;

	void throw_if_empty();

public:
	static bool is_space(char c);

	string_parser(std::string_view view) :
			view(view)
	{}

	void skip_whitespaces();
	void skip_whitespaces_and_comma();
	void skip_inclusive_until(char c);

	std::string_view read_word();
	std::string_view read_word_until(char c);

	// skips leading whitespaces
	template <class number_type>
	number_type read_number(){
		this->skip_whitespaces();

		number_type ret = 0;

		std::from_chars_result res;

		if constexpr (std::is_floating_point<number_type>::value){
			// TODO: use std::from_chars for floats when it is widely supported by C++17 compilers,
			// so if constexpr will not be needed.
			if constexpr (std::is_same<long double, number_type>::value){
				return number_type(this->read_number<double>());
			}else{
				res = utki::from_chars(this->view.data(), this->view.data() + this->view.size(), ret);
			}
		}else{
			int base = 10;

			if constexpr (std::is_unsigned_v<number_type>){
				// detect base
				if(this->peek_char() == '0'){
					this->read_char();
					if(this->empty()){
						return number_type(0);
					}
					char c = this->peek_char();
					switch(c){
						case 'x':
							if(this->size() < 2){
								return number_type(0);
							}
							c = this->peek_char(1);
							if(('0' <= c && c <= '9') || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F')){
								base = 16;
								this->read_char();
							}else{
								return number_type(0);
							}
							break;
						case 'b':
							if(this->size() < 2){
								return number_type(0);
							}
							c = this->peek_char(1);
							if(c == '0' || c == '1'){
								base = 2;
								this->read_char();
							}else{
								return number_type(0);
							}
							break;
						default:
							if('0' <= c && c <= '7'){
								base = 8;
							}else if(c < '0' || '9' < c){
								return number_type(0);
							}
							break;
					}
				}
			}

			res = std::from_chars(this->view.data(), this->view.data() + this->view.size(), ret, base);
		}

		if(res.ec == std::errc::invalid_argument){
			throw std::invalid_argument("string_parser::read_integer(): could not parse integer number");
		}

		ASSERT(this->view.data() != res.ptr)

		this->view = this->view.substr(res.ptr - this->view.data());

		return ret;
	}

	char read_char();

	char peek_char();
	char peek_char(size_t n);

	std::string_view read_chars(size_t n);
	std::string_view read_chars_until(char c);

	bool empty()const noexcept{
		return this->view.empty();
	}

	size_t size()const noexcept{
		return this->view.size();
	}
};

}
