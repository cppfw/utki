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

#include "debug.hpp"

#include "config.hpp"
#include "util.hpp"

#if CFG_OS_NAME == CFG_OS_NAME_ANDROID
#	include <android/log.h>
#else
#	include <iostream>
#endif

#if CFG_COMPILER == CFG_COMPILER_GCC || CFG_COMPILER == CFG_COMPILER_CLANG
#	include <cxxabi.h> // for demangling type names
#endif

// undefine possibly defined assert macro to avoid macro's name collision
#ifdef assert
#	undef assert
#endif

using namespace utki;

namespace {
const std::string_view colored_error_string = "\033[1;31merror\033[0m";
const std::string_view uncolored_error_string = "error";
} // namespace

void utki::assert(
	bool condition,
	const std::function<void(std::ostream&)>& print,
	const utki::source_location& source_location
)
{
	if (condition) {
		return;
	}

	std::stringstream ss;
	ss << source_location.file_name() << ":" << source_location.line() << ": ";

	if (is_terminal_cerr()) {
#if CFG_COMPILER == CFG_COMPILER_MSVC && CFG_COMPILER_MSVC_TOOLS_V <= 141
		ss << std::string(colored_error_string);
	} else {
		ss << std::string(uncolored_error_string);
#else
		ss << colored_error_string;
	} else {
		ss << uncolored_error_string;
#endif
	}

	ss << ": assertion failed";

	if (print) {
		ss << ":" << std::endl << "  ";
		print(ss);
	}

#if CFG_OS_NAME == CFG_OS_NAME_ANDROID
	// TODO: remove commented code after __android_log_assert() is tested to be working
	// __android_log_write(ANDROID_LOG_INFO, "utki", ss.str().c_str());

	__android_log_assert(ss.str().c_str(), "utki", nullptr);
#else
	std::cerr << ss.str() << std::endl;
	std::cerr.flush();
#endif

	abort();
}

void utki::log(const std::function<void(std::ostream&)>& print)
{
	if (!print) {
		return;
	}

#if CFG_OS_NAME == CFG_OS_NAME_ANDROID
	std::stringstream ss;
	print(ss);
	__android_log_write(ANDROID_LOG_INFO, "utki", ss.str().c_str());
#else
	print(std::cout);
	std::cout.flush();
#endif
}

std::string utki::demangle(const char* name)
{
#if CFG_COMPILER == CFG_COMPILER_GCC || CFG_COMPILER == CFG_COMPILER_CLANG
	// NOLINTNEXTLINE(cppcoreguidelines-init-variables)
	int status;
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
	auto demangled_name = abi::__cxa_demangle(
		name,
		nullptr, // let __cxa_demangle() allocate memory buffer for us
		nullptr, // not interested in allocated memory buffer size
		&status
	);

	switch (status) {
		case 0: // demangling succeeded
			{
				utki::scope_exit scope_exit([demangled_name] {
					// NOLINTNEXTLINE(cppcoreguidelines-no-malloc, cppcoreguidelines-owning-memory)
					free(demangled_name); // abi::__cxa_demangle requires freeing allocated memory
				});
				return {demangled_name};
			}
			break;
		// NOLINTNEXTLINE(bugprone-branch-clone)
		case -1: // memory allocation failed
			[[fallthrough]];
		case -2: // given mangled name is not a valid name under the C++ ABI mangling rules
			[[fallthrough]];
		case -3: // one of the arguments is invalid
			[[fallthrough]];
		default:
			return {name};
	}
#else
	return {name};
#endif
}
