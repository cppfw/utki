/*
The MIT License (MIT)

Copyright (c) 2015-2023 Ivan Gagis <igagis@gmail.com>

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

#include "util.hpp"

#if CFG_OS == CFG_OS_WINDOWS
#	include <io.h>
#else
#	include <unistd.h>
#endif

using namespace utki;

bool utki::is_cerr_terminal()
{
#if CFG_OS == CFG_OS_WINDOWS
	// need to compare result with 0 to avoid MSVC compiler warning
	return _isatty(_fileno(stderr)) != 0;
#else
	return isatty(fileno(stderr));
#endif
}

bool utki::is_cout_terminal()
{
#if CFG_OS == CFG_OS_WINDOWS
	// need to compare result with 0 to avoid MSVC compiler warning
	return _isatty(_fileno(stdout)) != 0;
#else
	return isatty(fileno(stdout));
#endif
}

bool utki::is_cin_terminal()
{
#if CFG_OS == CFG_OS_WINDOWS
	// need to compare result with 0 to avoid MSVC compiler warning
	return _isatty(_fileno(stdin)) != 0;
#else
	return isatty(fileno(stdin));
#endif
}
