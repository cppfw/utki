/*
The MIT License (MIT)

Copyright (c) 2015-2024 Ivan Gagis <igagis@gmail.com>

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

// NOLINTBEGIN

#ifndef FASTFLOAT_PARSE_NUMBER_H
#	define FASTFLOAT_PARSE_NUMBER_H
#	include <cmath>
#	include <cstring>
#	include <limits>
#	include <system_error>

#	include "ascii_number.hxx"
#	include "decimal_to_binary.hxx"
#	include "simple_decimal_conversion.hxx"

namespace fast_float {

namespace detail {
/**
 * Special case +inf, -inf, nan, infinity, -infinity.
 * The case comparisons could be made much faster given that we know that the
 * strings a null-free and fixed.
 **/

// NOLINTNEXTLINE(readability-identifier-naming)
template <typename T>
from_chars_result parse_infnan(const char* first, const char* last, T& value) noexcept
{
	from_chars_result answer; // NOLINT
	answer.ptr = first;
	answer.ec = std::errc(); // be optimistic
	bool minus_sign = false;
	if (*first == '-')
	{ // assume first < last, so dereference without checks; C++17 20.19.3.(7.1) explicitly forbids '+' here
		minus_sign = true;
		++first; // NOLINT
	}
	if (last - first >= 3) {
		if (fastfloat_strncasecmp(first, "nan", 3)) {
			answer.ptr = (first += 3); // NOLINT
			value = minus_sign ? -std::numeric_limits<T>::quiet_NaN() : std::numeric_limits<T>::quiet_NaN();
			// Check for possible nan(n-char-seq-opt), C++17 20.19.3.7, C11 7.20.1.3.3. At least MSVC produces nan(ind)
			// and nan(snan).
			if (first != last && *first == '(') {
				for (const char* ptr = first + 1; ptr != last; ++ptr) { // NOLINT
					if (*ptr == ')') {
						// NOLINTNEXTLINE
						answer.ptr = ptr + 1; // valid nan(n-char-seq-opt)
						break;
					} else if (!(('a' <= *ptr && *ptr <= 'z') || ('A' <= *ptr && *ptr <= 'Z') ||
								 ('0' <= *ptr && *ptr <= '9') || *ptr == '_'))
						break; // forbidden char, not nan(n-char-seq-opt)
				}
			}
			return answer;
		}
		if (fastfloat_strncasecmp(first, "inf", 3)) {
			if ((last - first >= 8) && fastfloat_strncasecmp(first + 3, "inity", 5)) { // NOLINT
				answer.ptr = first + 8; // NOLINT
			} else {
				answer.ptr = first + 3; // NOLINT
			}
			value = minus_sign ? -std::numeric_limits<T>::infinity() : std::numeric_limits<T>::infinity();
			return answer;
		}
	}
	answer.ec = std::errc::invalid_argument;
	return answer;
}

// NOLINTNEXTLINE(readability-identifier-naming)
template <typename T>
fastfloat_really_inline void to_float(bool negative, adjusted_mantissa am, T& value)
{
	uint64_t word = am.mantissa;
	word |= uint64_t(am.power2) << binary_format<T>::mantissa_explicit_bits();
	word = negative ? word | (uint64_t(1) << binary_format<T>::sign_index()) : word;
#	if FASTFLOAT_IS_BIG_ENDIAN == 1
	if (std::is_same<T, float>::value) {
		::memcpy(&value, (char*)&word + 4, sizeof(T)); // extract value at offset 4-7 if float on big-endian
	} else {
		::memcpy(&value, &word, sizeof(T));
	}
#	else
	// For little-endian systems:
	::memcpy(&value, &word, sizeof(T));
#	endif
}

} // namespace detail

// NOLINTNEXTLINE(readability-identifier-naming)
template <typename T>
from_chars_result
from_chars(const char* first, const char* last, T& value, chars_format fmt /*= chars_format::general*/) noexcept
{
	static_assert(std::is_same_v<T, double> || std::is_same_v<T, float>, "only float and double are supported");

	from_chars_result answer; // NOLINT
	if (first == last) {
		answer.ec = std::errc::invalid_argument;
		answer.ptr = first;
		return answer;
	}
	parsed_number_string pns = parse_number_string(first, last, fmt);
	if (!pns.valid) {
		return detail::parse_infnan(first, last, value);
	}
	answer.ec = std::errc(); // be optimistic
	answer.ptr = pns.lastmatch;
	// Next is Clinger's fast path.
	if (binary_format<T>::min_exponent_fast_path() <= pns.exponent &&
		pns.exponent <= binary_format<T>::max_exponent_fast_path() &&
		pns.mantissa <= binary_format<T>::max_mantissa_fast_path() && !pns.too_many_digits)
	{
		value = T(pns.mantissa);
		if (pns.exponent < 0) {
			value = value / binary_format<T>::exact_power_of_ten(-pns.exponent);
		} else {
			value = value * binary_format<T>::exact_power_of_ten(pns.exponent);
		}
		if (pns.negative) {
			value = -value;
		}
		return answer;
	}
	adjusted_mantissa am = compute_float<binary_format<T>>(pns.exponent, pns.mantissa);
	if (pns.too_many_digits) {
		if (am != compute_float<binary_format<T>>(pns.exponent, pns.mantissa + 1)) {
			am.power2 = -1; // value is invalid.
		}
	}
	// If we called compute_float<binary_format<T>>(pns.exponent, pns.mantissa) and we have an invalid power (am.power2
	// < 0), then we need to go the long way around again. This is very uncommon.
	if (am.power2 < 0) {
		am = parse_long_mantissa<binary_format<T>>(first, last);
	}
	detail::to_float(pns.negative, am, value);
	return answer;
}

} // namespace fast_float

#endif

// NOLINTEND
