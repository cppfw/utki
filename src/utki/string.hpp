#pragma once

#include <string>
#include <vector>
#include <array>

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
std::vector<std::string> split(const std::string& str, char delimiter);

/**
 * @brief Split string to separate words.
 * Delimiter for splitting is any sequence of whitespaces.
 * Whitespace includes space, tab, newline characters.
 * @param str - string to split to words.
 * @return vector of words.
 */
std::vector<std::string> split(const std::string& str);

/**
 * @brief Word wrap string.
 * @param str - string to word wrap.
 * @param width - maximum number of characters per line.
 * @return list of word wrapped lines.
 */
std::vector<std::string> word_wrap(const std::string& str, unsigned width);

}
