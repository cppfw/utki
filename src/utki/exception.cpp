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

#include "exception.hpp"

#include <stdexcept>

#include "config.hpp"

#if CFG_COMPILER == CFG_COMPILER_GCC || CFG_COMPILER == CFG_COMPILER_CLANG
#	include <cxxabi.h> // for accessing current exception type name
#endif

#include "debug.hpp"

using namespace std::string_literals;

using namespace utki;

std::string utki::current_exception_to_string(std::string_view indentation)
{
	if (!std::current_exception()) {
		throw std::logic_error("no current exception");
	}

	std::stringstream ss;
	ss << indentation <<
#if CFG_COMPILER == CFG_COMPILER_GCC || CFG_COMPILER == CFG_COMPILER_CLANG
		demangle(abi::__cxa_current_exception_type()->name());
#else
		"unknown exception"s;
#endif

	try {
		throw;
	} catch (std::nested_exception& nested) {
		try {
			nested.rethrow_nested();
		} catch (std::exception& e) {
			ss << "\n" << to_string(e, indentation);
		} catch (...) {
			ss << "\n" << current_exception_to_string(indentation);
		}
		// NOLINTNEXTLINE(bugprone-empty-catch)
	} catch (...) {
		// do nothing, the exception is already printed
	}

	return ss.str();
}

std::string utki::to_string(const std::exception& e, std::string_view indentation)
{
	std::stringstream ss;
	ss << indentation << demangle(typeid(e).name()) << ": " << e.what();

	try {
		std::rethrow_if_nested(e);
	} catch (std::exception& nested) {
		ss << "\n" << to_string(nested, indentation);
	} catch (...) {
		ss << "\n" << current_exception_to_string(indentation);
	}
	return ss.str();
}
