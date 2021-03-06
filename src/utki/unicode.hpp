#pragma once

#include <string>
#include <vector>
#include <array>

namespace utki{

/**
 * @brief Iterator to iterate through utf-8 encoded unicode characters.
 */
class utf8_iterator{
	char32_t c = 0;
	const uint8_t* n = nullptr;

public:
	/**
	 * @brief Create undefined iterator.
     */
	utf8_iterator(){}

	/**
	 * @brief Create iterator pointing to the begin of the given utf-8 encoded string.
     * @param begin - pointer to the first byte of the null-terminated utf-8 encoded string.
     */
	utf8_iterator(const char* begin);

	/**
	 * @brief Get current unicode character.
     * @return unicode value of the character this interator is currently pointing to.
     */
	char32_t character()const noexcept{
		return this->c;
	}

	// no operator*() because it usually returns reference, don't want to break this contract.
	
	/**
	 * @brief Prefix increment.
	 * Move iterator to the next character in the string.
	 * If iterator points to the end of the string before this operation then the result of this operation is undefined.
     * @return reference to this iterator object.
     */
	utf8_iterator& operator++()noexcept;

	// no postfix ++ operator, there is no need in it.

	/**
	 * @brief Check if iterator points to the end of the string.
     * @return true if iterator points to the end of the string.
	 * @return false otherwise.
     */
	bool is_end()const noexcept{
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
inline std::u32string to_utf32(const char* str){
	return to_utf32(utf8_iterator(str));
}

/**
 * @brief Convert UTF-8 to UTF-32.
 * @param str - string to convert.
 * @return UTF-32 string.
 */
inline std::u32string to_utf32(const std::string& str){
	return to_utf32(str.c_str());
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
std::string to_utf8(const std::u32string& str);

}
