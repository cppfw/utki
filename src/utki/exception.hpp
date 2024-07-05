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

namespace utki {

/**
 * @brief Generic exception.
 *
 * This class allows throwing an std::exception-derived exception with text message.
 * This class is for the cases when none of the std::logic_error nor the std::runtime_error suits.
 */
class exception : public std::exception
{
	std::string message;

public:
	/**
	 * @brief Constructor.
	 *
	 * @param message - text message to be stored in the exception object.
	 */
	exception(std::string message) :
		message(std::move(message))
	{}

	const char* what() const noexcept override
	{
		return this->message.c_str();
	}
};

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

} // namespace utki
