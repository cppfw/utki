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

#pragma once

#include <exception>
#include <string>
#include <string_view>

#include "debug.hpp"
#include "string.hpp"

namespace utki {

/**
 * @brief Convert exception object to human readable string.
 * The resulting string can be multiline.
 * @param e - exception object to convert to string.
 * @param indentation - indentation to use for resulting string.
 * @return String describing the exception object.
 */
std::string to_string(const std::exception& e, std::string_view indentation = std::string_view());

/**
 * @brief Convert currently handled exception object to human readable string.
 * The resulting string can be multiline.
 * @param indentation - indentation to use for resulting string.
 * @return String describing the exception object.
 */
std::string current_exception_to_string(std::string_view indentation = std::string_view());

/**
 * @brief Generic exception.
 */
class exception : public std::exception
{
public:
	/**
	 * @brief Convert the exception object to human readable string.
	 * The string can be multiline.
	 * @param indentation - indentation to use for conversion.
	 * @return Human readable string describing the exception object.
	 */
	virtual std::string to_string(std::string_view indentation = std::string_view()) const
	{
		return utki::to_string(static_cast<const std::exception&>(*this));
	}
};

// forward declaration
template <typename exception_type>
void throw_with_nested(exception_type exception);

template <typename exception_type>
class stacked_exception : public exception
{
	static_assert(
		std::is_base_of_v<std::exception, exception_type>,
		"nesting exception must be derived from std::exception"
	);

	std::nested_exception nested;
	exception_type nesting;

	mutable std::string description;

	stacked_exception(exception_type nesting) :
		nesting(std::move(nesting))
	{}

	template <typename same_exception_type>
	friend void utki::throw_with_nested(same_exception_type exception);

public:
	const char* what() const noexcept override
	{
		this->description = utki::cat(std::string_view("exception stack:\n"), this->to_string(std::string_view("- ")));
		return this->description.c_str();
	}

	std::string to_string(std::string_view indentation = std::string_view()) const override
	{
		std::stringstream ss;
		ss << indentation << utki::to_string(static_cast<const std::exception&>(this->nesting));

		try {
			this->nested.rethrow_nested();
		} catch (exception& e) {
			ss << "\n" << e.to_string(indentation);
		} catch (std::exception& e) {
			ss << "\n" << utki::to_string(e, indentation);
		} catch (...) {
			ss << "\n" << utki::current_exception_to_string(indentation);
		}
		return ss.str();
	}
};

/**
 * @brief Throw exception with nested current exception.
 * This is a replacement of std::throw_with_nested() which
 * prints all nested exceptions information via it's what() member function.
 * @param exception - exception to throw.
 */
template <typename exception_type>
void throw_with_nested(exception_type exception)
{
	static_assert(
		std::is_base_of_v<std::exception, exception_type>,
		"nesting exception must be derived from std::exception"
	);

	ASSERT( //
		std::current_exception() != nullptr,
		[](auto& o) {
			o << "called not from within 'catch' block";
		}
	)
	throw stacked_exception(std::move(exception));
}

} // namespace utki
