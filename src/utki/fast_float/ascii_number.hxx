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

#ifndef FASTFLOAT_ASCII_NUMBER_H
#	define FASTFLOAT_ASCII_NUMBER_H

#	include <cctype>
#	include <cstdint>
#	include <cstdio>
#	include <cstring>

#	include "float_common.hxx"

namespace fast_float {

// Next function can be micro-optimized, but compilers are entirely
// able to optimize it well.
fastfloat_really_inline bool is_integer(char c) noexcept
{
	return c >= '0' && c <= '9';
}

fastfloat_really_inline uint64_t byteswap(uint64_t val)
{
	// NOLINTNEXTLINE
	return (val & 0xFF00000000000000) >> 56
		// NOLINTNEXTLINE
		| (val & 0x00FF000000000000) >> 40
		// NOLINTNEXTLINE
		| (val & 0x0000FF0000000000) >> 24
		// NOLINTNEXTLINE
		| (val & 0x000000FF00000000) >> 8
		// NOLINTNEXTLINE
		| (val & 0x00000000FF000000) << 8
		// NOLINTNEXTLINE
		| (val & 0x0000000000FF0000) << 24
		// NOLINTNEXTLINE
		| (val & 0x000000000000FF00) << 40 | (val & 0x00000000000000FF) << 56;
}

fastfloat_really_inline uint64_t read_u64(const char* chars)
{
	// NOLINTNEXTLINE(cppcoreguidelines-init-variables)
	uint64_t val;
	::memcpy(&val, chars, sizeof(uint64_t));
#	if FASTFLOAT_IS_BIG_ENDIAN == 1
	// Need to read as-if the number was in little-endian order.
	val = byteswap(val);
#	endif
	return val;
}

fastfloat_really_inline void write_u64(uint8_t* chars, uint64_t val)
{
#	if FASTFLOAT_IS_BIG_ENDIAN == 1
	// Need to read as-if the number was in little-endian order.
	val = byteswap(val);
#	endif
	::memcpy(chars, &val, sizeof(uint64_t));
}

// credit  @aqrit
fastfloat_really_inline uint32_t parse_eight_digits_unrolled(uint64_t val)
{
	const uint64_t mask = 0x000000FF000000FF;
	const uint64_t mul1 = 0x000F424000000064; // 100 + (1000000ULL << 32)
	const uint64_t mul2 = 0x0000271000000001; // 1 + (10000ULL << 32)
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
	val -= 0x3030303030303030;
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
	val = (val * 10) + (val >> 8); // val = (val * 2561) >> 8;
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
	val = (((val & mask) * mul1) + (((val >> 16) & mask) * mul2)) >> 32;
	return uint32_t(val);
}

fastfloat_really_inline uint32_t parse_eight_digits_unrolled(const char* chars) noexcept
{
	return parse_eight_digits_unrolled(read_u64(chars));
}

// credit @aqrit
fastfloat_really_inline bool is_made_of_eight_digits_fast(uint64_t val) noexcept
{
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
	return !((((val + 0x4646464646464646) | (val - 0x3030303030303030)) & 0x8080808080808080));
}

fastfloat_really_inline bool is_made_of_eight_digits_fast(const char* chars) noexcept
{
	return is_made_of_eight_digits_fast(read_u64(chars));
}

struct parsed_number_string {
	int64_t exponent;
	uint64_t mantissa;
	const char* lastmatch;
	bool negative;
	bool valid;
	bool too_many_digits;
};

// Assuming that you use no more than 19 digits, this will
// parse an ASCII string.
fastfloat_really_inline parsed_number_string
parse_number_string(const char* p, const char* pend, chars_format fmt) noexcept
{
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	parsed_number_string answer;
	answer.valid = false;
	answer.too_many_digits = false;
	answer.negative = (*p == '-');
	if (*p == '-') { // C++17 20.19.3.(7.1) explicitly forbids '+' sign here
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		++p;
		if (p == pend) {
			return answer;
		}
		if (!is_integer(*p) && (*p != '.')) { // a  sign must be followed by an integer or the dot
			return answer;
		}
	}
	const char* const start_digits = p;

	uint64_t i = 0; // an unsigned int avoids signed overflows (which are bad)

	while ((p != pend) && is_integer(*p)) {
		// a multiplication by 10 is cheaper than an arbitrary integer
		// multiplication
		// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
		i = 10 * i + uint64_t(*p - '0'); // might overflow, we will handle the overflow later
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		++p;
	}
	const char* const end_of_integer_part = p;
	auto digit_count = int64_t(end_of_integer_part - start_digits);
	int64_t exponent = 0;
	if ((p != pend) && (*p == '.')) {
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		++p;
		// Fast approach only tested under little endian systems
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic, cppcoreguidelines-avoid-magic-numbers)
		if ((p + 8 <= pend) && is_made_of_eight_digits_fast(p)) {
			// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
			i = i * 100000000 + parse_eight_digits_unrolled(p); // in rare cases, this will overflow, but that's ok
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic, cppcoreguidelines-avoid-magic-numbers)
			p += 8;
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic, cppcoreguidelines-avoid-magic-numbers)
			if ((p + 8 <= pend) && is_made_of_eight_digits_fast(p)) {
				// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
				i = i * 100000000 + parse_eight_digits_unrolled(p); // in rare cases, this will overflow, but that's ok
				// NOLINTNEXTLINE
				p += 8;
			}
		}
		while ((p != pend) && is_integer(*p)) {
			auto digit = uint8_t(*p - '0');
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			++p;
			// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
			i = i * 10 + digit; // in rare cases, this will overflow, but that's ok
		}
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		exponent = end_of_integer_part + 1 - p;
		digit_count -= exponent;
	}
	// we must have encountered at least one integer!
	if (digit_count == 0) {
		return answer;
	}
	int64_t exp_number = 0; // explicit exponential part
	if ((fmt & chars_format::scientific) && (p != pend) && (('e' == *p) || ('E' == *p))) {
		const char* location_of_e = p;
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		++p;
		bool neg_exp = false;
		if ((p != pend) && ('-' == *p)) {
			neg_exp = true;
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			++p;
		} else if ((p != pend) && ('+' == *p)) { // '+' on exponent is allowed by C++17 20.19.3.(7.1)
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			++p;
		}
		if ((p == pend) || !is_integer(*p)) {
			if (!(fmt & chars_format::fixed)) {
				// We are in error.
				return answer;
			}
			// Otherwise, we will be ignoring the 'e'.
			p = location_of_e;
		} else {
			while ((p != pend) && is_integer(*p)) {
				auto digit = uint8_t(*p - '0');
				// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
				if (exp_number < 0x10000) {
					// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
					exp_number = 10 * exp_number + digit;
				}
				// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
				++p;
			}
			if (neg_exp) {
				exp_number = -exp_number;
			}
			exponent += exp_number;
		}
	} else {
		// If it scientific and not fixed, we have to bail out.
		if ((fmt & chars_format::scientific) && !(fmt & chars_format::fixed)) {
			return answer;
		}
	}
	answer.lastmatch = p;
	answer.valid = true;

	// If we frequently had to deal with long strings of digits,
	// we could extend our code by using a 128-bit integer instead
	// of a 64-bit integer. However, this is uncommon.
	//
	// We can deal with up to 19 digits.
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
	if (digit_count > 19) { // this is uncommon
		// It is possible that the integer had an overflow.
		// We have to handle the case where we have 0.0000somenumber.
		// We need to be mindful of the case where we only have zeroes...
		// E.g., 0.000000000...000.
		const char* start = start_digits;
		while ((start != pend) && (*start == '0' || *start == '.')) {
			if (*start == '0') {
				digit_count--;
			}
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			start++;
		}
		// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
		if (digit_count > 19) {
			answer.too_many_digits = true;
			// Let us start again, this time, avoiding overflows.
			i = 0;
			p = start_digits;
			const uint64_t minimal_nineteen_digit_integer{1000000000000000000};
			while ((i < minimal_nineteen_digit_integer) && (p != pend) && is_integer(*p)) {
				// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
				i = i * 10 + uint64_t(*p - '0');
				// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
				++p;
			}
			if (i >= minimal_nineteen_digit_integer) { // We have a big integers
				exponent = end_of_integer_part - p + exp_number;
			} else { // We have a value with a fractional component.
				// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
				p++; // skip the '.'
				const char* first_after_period = p;
				while ((i < minimal_nineteen_digit_integer) && (p != pend) && is_integer(*p)) {
					// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
					i = i * 10 + uint64_t(*p - '0');
					// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					++p;
				}
				exponent = first_after_period - p + exp_number;
			}
			// We have now corrected both exponent and i, to a truncated value
		}
	}
	answer.exponent = exponent;
	answer.mantissa = i;
	return answer;
}

// This should always succeed since it follows a call to parse_number_string
// This function could be optimized. In particular, we could stop after 19 digits
// and try to bail out. Furthermore, we should be able to recover the computed
// exponent from the pass in parse_number_string.
fastfloat_really_inline decimal parse_decimal(const char* p, const char* pend) noexcept
{
	decimal answer;
	answer.num_digits = 0;
	answer.decimal_point = 0;
	answer.truncated = false;
	answer.negative = (*p == '-');
	if (*p == '-') { // C++17 20.19.3.(7.1) explicitly forbids '+' sign here
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		++p;
	}
	// skip leading zeroes
	while ((p != pend) && (*p == '0')) {
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		++p;
	}
	while ((p != pend) && is_integer(*p)) {
		if (answer.num_digits < max_digits) {
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
			answer.digits[answer.num_digits] = uint8_t(*p - '0');
		}
		answer.num_digits++;
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		++p;
	}
	if ((p != pend) && (*p == '.')) {
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		++p;
		const char* first_after_period = p;
		// if we have not yet encountered a zero, we have to skip it as well
		if (answer.num_digits == 0) {
			// skip zeros
			while ((p != pend) && (*p == '0')) {
				// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
				++p;
			}
		}
		// We expect that this loop will often take the bulk of the running time
		// because when a value has lots of digits, these digits often
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic, cppcoreguidelines-avoid-magic-numbers)
		while ((p + 8 <= pend) && (answer.num_digits + 8 < max_digits)) {
			uint64_t val = read_u64(p);
			if (!is_made_of_eight_digits_fast(val)) {
				break;
			}
			// We have eight digits, process them in one go!
			// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
			val -= 0x3030303030303030;
			// NOLINTNEXTLINE
			write_u64(answer.digits + answer.num_digits, val);
			// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
			answer.num_digits += 8;
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			p += 8; // NOLINT
		}
		while ((p != pend) && is_integer(*p)) {
			if (answer.num_digits < max_digits) {
				// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
				answer.digits[answer.num_digits] = uint8_t(*p - '0');
			}
			answer.num_digits++;
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			++p;
		}
		answer.decimal_point = int32_t(first_after_period - p);
	}
	// We want num_digits to be the number of significant digits, excluding
	// leading *and* trailing zeros! Otherwise the truncated flag later is
	// going to be misleading.
	if (answer.num_digits > 0) {
		// We potentially need the answer.num_digits > 0 guard because we
		// prune leading zeros. So with answer.num_digits > 0, we know that
		// we have at least one non-zero digit.
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		const char* preverse = p - 1;
		int32_t trailing_zeros = 0;
		while ((*preverse == '0') || (*preverse == '.')) {
			if (*preverse == '0') {
				trailing_zeros++;
			};
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			--preverse;
		}
		answer.decimal_point += int32_t(answer.num_digits);
		answer.num_digits -= uint32_t(trailing_zeros);
	}
	if (answer.num_digits > max_digits) {
		answer.truncated = true;
		answer.num_digits = max_digits;
	}
	if ((p != pend) && (('e' == *p) || ('E' == *p))) {
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		++p;
		bool neg_exp = false;
		if ((p != pend) && ('-' == *p)) {
			neg_exp = true;
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			++p;
		} else if ((p != pend) && ('+' == *p)) { // '+' on exponent is allowed by C++17 20.19.3.(7.1)
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			++p;
		}
		int32_t exp_number = 0; // exponential part
		while ((p != pend) && is_integer(*p)) {
			auto digit = uint8_t(*p - '0');
			// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
			if (exp_number < 0x10000) {
				// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
				exp_number = 10 * exp_number + digit;
			}
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			++p;
		}
		answer.decimal_point += (neg_exp ? -exp_number : exp_number);
	}
	// In very rare cases, we may have fewer than 19 digits, we want to be able to reliably
	// assume that all digits up to max_digit_without_overflow have been initialized.
	for (uint32_t i = answer.num_digits; i < max_digit_without_overflow; i++) {
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
		answer.digits[i] = 0;
	}

	return answer;
}
} // namespace fast_float

#endif

// NOLINTEND
