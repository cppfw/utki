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

#include "log.hpp"

#include <sstream>

#include "config.hpp"

#if CFG_OS_NAME == CFG_OS_NAME_ANDROID
#	include <android/log.h>

#else
#	include <iostream>

#endif

using namespace utki;

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
	std::cout.flush(); // TODO: no flush by default?
#endif
}
