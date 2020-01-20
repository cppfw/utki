#pragma once

#include <string>
#include <vector>

#include "config.hpp"

namespace utki{

/**
 * @brief Format string.
 * @param format - printf-like string format.
 * @param ... - printf-like string arguments.
 * @return formatted string.
 */
#if M_COMPILER != M_COMPILER_MSVC
__attribute__((format(printf, 1, 2)))
#endif
std::string make_string(const char* format, ...);

/**
 * @brief Format string.
 * va_list version of make_string().
 * @param format - printf like string format.
 * @param args - va_list of string arguments.
 * @return formatted string.
 */
std::string make_string_va_list(const char* format, va_list args);

/**
 * @brief Split string using give delimiter.
 * @param str - string to split.
 * @param delimiter - delimiter character to use as a splitter.
 * @return vector of splitted strings.
 */
std::vector<std::string> split(const std::string& str, char delimiter);

/**
 * @brief Word wrap string.
 * @param str - string to word wrap.
 * @param width - maximum number of characters per line.
 * @return list of word wrapped lines.
 */
std::vector<std::string> word_wrap(const std::string& str, unsigned width);

}
