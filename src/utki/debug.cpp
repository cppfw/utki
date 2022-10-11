/*
The MIT License (MIT)

Copyright (c) 2015-2022 Ivan Gagis <igagis@gmail.com>

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

using namespace utki;

namespace {
std::string_view colored_error_string = "\033[1;31merror\033[0m";
std::string_view uncolored_error_string = "error";
} // namespace

void utki::assert(
	bool condition,
	const std::function<void(std::ostream&)>& print,
	utki::source_location&& source_location
)
{
	if (condition) {
		return;
	}

	std::stringstream ss;
	ss << source_location.file_name() << ":" << source_location.line() << ": ";

	if (is_cerr_terminal()) {
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
