/*
The MIT License (MIT)

utki - Utility Kit for C++.

Copyright (c) 2015-2026 Ivan Gagis <igagis@gmail.com>

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

#include <functional>
#include <ostream>

namespace utki {

/**
 * @brief Print string to standard output.
 * After printing performs flush of the standard output.
 * @param print - function which is called with a standard output stream parameter to output the string to the stream.
 */
void log(const std::function<void(std::ostream&)>& print);

/**
 * @brief Concatenate and print.
 * Concatenates all it's arguments and then prints to standard output.
 * Then performs flush on the standard output.
 * @param s - parameters pack to concatenate and print to standard output.
 */
template <typename... streamable_type>
void logcat(const streamable_type&... s)
{
	log([&](std::ostream& o) {
		// cppcoreguidelines-pro-bounds-array-to-pointer-decay false positive, exact diagnostic:
		// "do not implicitly decay an array into a pointer; consider using gsl::array_view or an explicit
		// cast instead", located at the fold expression `o << ... << s`. logcat() is a variadic template, so for a call like
		// logcat("text") each `s` is deduced as a `const char (&)[N]` reference, and `o << s` decays it
		// to the `const char*` overload of `operator<<`. The check cannot see through the template
		// parameter and warns about the potential decay. The decay is the intended and only way to
		// stream a C-string literal (std::basic_ostream has no std::string_view overload), and the
		// literal's static storage duration outlives the use, so there is no dangling pointer risk.
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay, "streaming a string literal requires decay to the const char* iostream overload; static lifetime rules out dangling")
		(o << ... << s);
		o.flush();
	});
}

} // namespace utki
