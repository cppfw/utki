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

#pragma once

#include <cstdint>
#include <ostream>

#include "type_traits.hpp"

namespace utki {
/**
 * @brief Version duplet.
 */
struct version_duplet {
	/**
	 * @brief Major version.
	 */
	uint16_t major;

	/**
	 * @brief Minor version.
	 */
	uint16_t minor;

	/**
	 * @brief Combine major and minor versions into one number.
	 */
	constexpr uint32_t to_uint32_t() const noexcept
	{
		return uint32_t(uint32_t(this->major) << sizeof(uint16_t) * utki::byte_bits) | uint32_t(this->minor);
	}

	friend std::ostream& operator<<(std::ostream& o, const version_duplet& vd)
	{
		return o << vd.major << '.' << vd.minor;
	}

	friend bool operator>=(const version_duplet& lhs, const version_duplet& rhs) noexcept
	{
		if (lhs.major > rhs.major) {
			return true;
		}
		if (lhs.major == rhs.major && lhs.minor >= rhs.minor) {
			return true;
		}
		return false;
	}

	friend bool operator<(const version_duplet& lhs, const version_duplet& rhs) noexcept
	{
		return !(lhs >= rhs);
	}
};

/**
 * @brief Version triplet.
 * Version of the form "MAJOR.MINOR.REVISON".
 */
struct version_triplet {
	/**
	 * @brief Version duplet.
	 * Major and minor versions.
	 */
	version_duplet duplet;

	/**
	 * @brief Revision.
	 */
	uint32_t revision;

	/**
	 * @brief Parse version triplet from string.
	 * The input string is supposed to contain version triplet at its beginning.
	 * Parsing will stop on the first character right after the version triplet, or at the end of the string.
	 * @param str - string to parse.
	 * @return Parsed version triplet.
	 * @throw std::invalid_argument in case input string does not represent a version triplet.
	 */
	version_triplet from(std::string_view str);
	// TODO: implement

	// TODO: add stream output operator

	// TODO: add unit tests

	// TODO: move out to version.hpp
};

} // namespace utki
