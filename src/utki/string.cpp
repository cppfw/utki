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

#include "string.hpp"

#include <array>
#include <cctype> // for std::isspace()
#include <cstdarg> // for varargs

#include <fast_float/fast_float.h>

#include "debug.hpp"
#include "util.hpp"

#ifdef assert
#	undef assert
#endif

using namespace utki;

std::string utki::make_string_va_list(const char* format, va_list args)
{
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	std::array<char, kilobyte> buf; // first guess is that the resulting string will take less than 1k

	std::string ret;

	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
	auto buf_ptr = buf.data();
	auto buf_size = buf.size();

	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg, cppcoreguidelines-init-variables)
	va_list cur_args;
	// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
	va_copy(cur_args, args);
	utki::scope_exit cur_args_scope_exit([&cur_args]() { //
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
		va_end(cur_args);
	});

	for (
#ifdef DEBUG
		size_t i = 0;; ++i
#else
		;;
#endif
	)
	{
		int size = vsnprintf( //
			buf_ptr,
			buf_size,
			format,
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
			cur_args
		);

		if (size < 0) {
			throw std::logic_error("snprintf() failed");
		}

		if (decltype(buf_size)(size) >= buf_size) {
			// resulting string takes more than we have first guessed, allocate enough memory and try again
			ret.resize(size);

			buf_ptr = ret.data();
			// NOTE: C++11 guarantees that std::string's internal buffer has 1 extra byte for null terminator.
			buf_size = ret.size() + 1;

			// after the vsnprintf() call the cur_args remain in altered state, thus, re-initialize it
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
			va_end(cur_args);
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
			va_copy(cur_args, args);
		} else {
			ret = std::string(buf_ptr, size);
			break;
		}

		ASSERT(i < 1)
	}

	return ret;
} // NOLINT(clang-analyzer-valist.Unterminated): the cur_args is freed by cur_args_scope_exit

std::string utki::make_string(const char* format, ...)
{
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg, cppcoreguidelines-init-variables)
	va_list args;
	// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
	va_start(args, format);
	// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
	auto ret = make_string_va_list(format, args);
	// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
	va_end(args);
	return ret;
}

std::vector<std::string> utki::split(std::string_view str)
{
	std::vector<std::string> ret;

	std::vector<char> buf;

	const auto add_str = [&]() {
		if (!buf.empty()) {
			ret.emplace_back(buf.data(), buf.size());
			buf.clear();
		}
	};

	for (auto c : str) {
		if (std::isspace(c)) {
			add_str();
			continue;
		}
		buf.push_back(c);
	}

	add_str();

	return ret;
}

std::vector<std::string> utki::word_wrap(std::string_view str, unsigned width)
{
	std::vector<std::string> ret;

	if (width == 0) {
		ret.emplace_back(str);
		return ret;
	}

	auto line_begin = str.begin();
	auto span_begin = str.begin(); // span is either series of spaces or series of non-spaces
	unsigned word_ended = false; // indicates that at least one word in the current line has ended
	for (auto i = str.begin(); i != str.end(); ++i) {
		ASSERT(std::distance(line_begin, i) >= 0)
		if (*i != '\n' && unsigned(std::distance(line_begin, i)) == width) {
			if (*span_begin == ' ') { // span of spaces
				if (word_ended) {
					ret.emplace_back(str.substr( //
						std::distance(str.begin(), line_begin),
						std::distance(line_begin, span_begin)
					));
					line_begin = std::next(span_begin);
				} else {
					line_begin = i;
					span_begin = line_begin;
				}
			} else if (word_ended) { // short word span
				ASSERT(std::distance(line_begin, span_begin) >= 1)
				ret.emplace_back(str.substr( //
					std::distance(str.begin(), line_begin),
					std::distance(line_begin, std::prev(span_begin, 1))
				));
				line_begin = span_begin;
			} else { // long word span (word longer than width)
				ret.emplace_back(str.substr( //
					std::distance(str.begin(), line_begin),
					std::distance(line_begin, i)
				));
				line_begin = i;
				span_begin = line_begin;
			}
			word_ended = false;
		}
		switch (*i) {
			case ' ':
				if (*span_begin != ' ') {
					span_begin = i;
					word_ended = true;
				}
				break;
			case '\n':
				ret.emplace_back(str.substr(std::distance(str.begin(), line_begin), std::distance(line_begin, i)));
				line_begin = std::next(i);
				span_begin = line_begin;
				word_ended = false;
				break;
			default:
				if (*span_begin == ' ') {
					span_begin = i;
				}
				break;
		}
	}

	// add last string
	if (span_begin != str.end() && *span_begin == ' ') {
		if (word_ended) {
			ret.emplace_back(str.substr( //
				std::distance(str.begin(), line_begin),
				std::distance(line_begin, span_begin)
			));
		}
	} else {
		ret.emplace_back(str.substr( //
			std::distance(str.begin(), line_begin),
			std::distance(line_begin, str.end())
		));
	}

	return ret;
}

namespace {
fast_float::chars_format to_fast_float_format(utki::chars_format f)
{
	switch (f) {
		case utki::chars_format::scientific:
			return fast_float::scientific;
		case utki::chars_format::fixed:
			return fast_float::fixed;
		case utki::chars_format::hex:
			return fast_float::hex;
		default:
		case utki::chars_format::general:
			return fast_float::general;
	}
}
} // namespace

std::from_chars_result utki::from_chars( //
	const char* first,
	const char* last,
	float& value,
	chars_format fmt
) noexcept
{
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	std::from_chars_result ret;

	auto res = fast_float::from_chars(first, last, value, to_fast_float_format(fmt));

	ret.ptr = res.ptr;
	ret.ec = res.ec;

	return ret;
}

std::from_chars_result utki::from_chars( //
	const char* first,
	const char* last,
	double& value,
	chars_format fmt
) noexcept
{
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	std::from_chars_result ret;

	auto res = fast_float::from_chars(first, last, value, to_fast_float_format(fmt));

	ret.ptr = res.ptr;
	ret.ec = res.ec;

	return ret;
}

bool string_parser::is_space(char c)
{
	// space characters of the default locale
	return //
		c == ' ' || //
		c == '\n' || //
		c == '\t' || //
		c == '\r' || //
		c == '\v' || // vertical tab
		c == '\f'; // form feed
}

void string_parser::skip_whitespaces()
{
	size_t pos = 0;
	for (char c : this->view) {
		if (!string_parser::is_space(c)) {
			break;
		}
		++pos;
	}
	this->view = this->view.substr(pos);
}

void string_parser::skip_whitespaces_and_comma()
{
	size_t pos = 0;

	bool comma_skipped = false;
	for (char c : this->view) {
		if (string_parser::is_space(c)) {
			++pos;
		} else if (c == ',') {
			if (comma_skipped) {
				break;
			}
			++pos;
			comma_skipped = true;
		} else {
			break;
		}
	}
	this->view = this->view.substr(pos);
}

void string_parser::skip_inclusive_until(char c)
{
	for (; !this->view.empty(); this->view = this->view.substr(1)) {
		if (this->view.front() == c) {
			this->view = this->view.substr(1);
			break;
		}
	}
}

char string_parser::skip_inclusive_until_one_of(utki::span<const char> c)
{
	for (; !this->view.empty(); this->view = this->view.substr(1)) {
		auto i = std::find(c.begin(), c.end(), this->view.front());
		if (i != c.end()) {
			this->view = this->view.substr(1);
			return *i;
		}
	}
	return '\0';
}

std::string_view string_parser::read_word()
{
	for (auto i = this->view.begin(); i != this->view.end(); ++i) {
		if (string_parser::is_space(*i)) {
			auto dist = std::distance(this->view.begin(), i);
			auto ret = this->view.substr(0, dist);
			this->view = this->view.substr(dist);
			return ret;
		}
	}

	auto ret = this->view;

	this->view = std::string_view();

	return ret;
}

std::string_view string_parser::read_word_until(char until_char)
{
	for (auto i = this->view.begin(); i != this->view.end(); ++i) {
		if (string_parser::is_space(*i) || *i == until_char) {
			auto dist = std::distance(this->view.begin(), i);
			auto ret = this->view.substr(0, dist);
			this->view = this->view.substr(dist);
			return ret;
		}
	}

	auto ret = this->view;

	this->view = std::string_view();

	return ret;
}

void string_parser::throw_if_empty() const
{
	if (this->view.empty()) {
		throw std::invalid_argument("string_parser string is empty");
	}
}

char string_parser::read_char()
{
	this->throw_if_empty();

	char ret = this->view.front();

	this->view = this->view.substr(1);

	return ret;
}

char string_parser::peek_char() const
{
	this->throw_if_empty();

	return this->view.front();
}

char string_parser::peek_char(size_t n) const
{
	if (this->view.size() <= n) {
		std::stringstream ss;
		ss << "string_parser string is to short (" << this->view.size() << " chars), requested char index = " << n;
		throw std::invalid_argument(ss.str());
	}

	return this->view[n];
}

std::string_view string_parser::read_chars(size_t n)
{
	using std::min;
	n = min(n, this->view.size());
	// NOLINTNEXTLINE(bugprone-suspicious-stringview-data-usage, "false positive")
	auto ret = std::string_view(this->view.data(), n);

	this->view = this->view.substr(n);

	return ret;
}

std::string_view string_parser::read_chars_until(char until_char)
{
	for (auto i = this->view.begin(); i != this->view.end(); ++i) {
		if (*i == until_char) {
			auto dist = std::distance(this->view.begin(), i);
			auto ret = this->view.substr(0, dist);
			this->view = this->view.substr(dist);
			return ret;
		}
	}

	auto ret = this->view;

	this->view = std::string_view();

	return ret;
}
