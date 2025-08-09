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

#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <sstream>

#include "config.hpp"
#include "log.hpp"
#include "macros.hpp"

#if CFG_CPP >= 20
#	if __has_include(<source_location>)
#		include <source_location>

namespace utki {
using std_source_location = std::source_location;
} // namespace utki
#	else
#		include <experimental/source_location>

namespace utki {
using std_source_location = std::experimental::source_location;
} // namespace utki
#	endif
#endif

#if defined(_DEBUG) && !defined(DEBUG)
#	define DEBUG
#endif

#ifdef assert
#	undef assert
#endif

//
//
// Logging
//
//

#ifdef DEBUG
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#	define LOG(print) utki::log(print);

#else
#	define LOG(x)

#endif

namespace utki {

/**
 * @brief Log only in debug build.
 * In debug build, logs a string provided by the passed in function.
 * Does nothing on release build.
 * @param print - function which will be invoked to construct the log string.
 */
#ifdef DEBUG
inline void log_debug(const std::function<void(std::ostream&)>& print)
{
	utki::log(print);
}

#else
inline constexpr void log_debug(const std::function<void(std::ostream&)>& print) {}

#endif
} // namespace utki

//
//
//  Assertions
//
//

namespace utki {

// backport of std::source_location
struct source_location {
	constexpr source_location(
		uint_least32_t line = 0,
		uint_least32_t column = 0,
		const char* file_name = nullptr
	) noexcept :
		line_v(line),
		column_v(column),
		file_name_v(file_name)
	{}

#if CFG_CPP >= 20
	constexpr source_location(const utki::std_source_location& sl) :
		source_location(
			sl.line(), //
			sl.column(),
			sl.file_name()
		)
	{}
#endif

	constexpr auto line() const noexcept -> uint_least32_t
	{
		return this->line_v;
	}

	constexpr auto column() const noexcept -> uint_least32_t
	{
		return this->column_v;
	}

	constexpr auto file_name() const noexcept -> char const*
	{
		return this->file_name_v;
	}

	constexpr auto function_name() const noexcept -> char const*
	{
		return this->function_name_v;
	}

private:
	uint_least32_t line_v;
	uint_least32_t column_v;
	const char* file_name_v;
	const char* function_name_v = nullptr;
};
} // namespace utki

/**
 * @brief Source location macro.
 * Constructs an object which holds current filename and current line number.
 */
#if CFG_CPP >= 20
#	define SL utki::std_source_location::current() // NOLINT(cppcoreguidelines-macro-usage)
#else
#	define SL utki::source_location(__LINE__, 0, __FILE__) // NOLINT(cppcoreguidelines-macro-usage)
#endif

namespace utki {

/**
 * @brief Assert that condion is true.
 * If the condition is true, then nothing happens.
 * If the condition is false, it will print the assertion failure message along with the information string
 * provided by the passed in function and then abort the program.
 * @param condition - condition to check for being true.
 * @param print - fucntion providing information string to print in case the assertion fails.
 * @param source_location - location of the assert() invocation in the source code.
 */
void assert_always(
	bool condition,
	const std::function<void(std::ostream&)>& print,
	const utki::source_location& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
);

/**
 * @brief Assert that condion is true.
 * In debug build this function invokes assert_always().
 * In non-debug build this fuction does nothing.
 * @param condition - condition to check for being true.
 * @param print - fucntion providing information string to print in case the assertion fails.
 * @param source_location - location of the assert() invocation in the source code.
 */
#ifdef DEBUG
inline void assert(
	bool condition,
	const std::function<void(std::ostream&)>& print,
	const utki::source_location& source_location
#	if CFG_CPP >= 20
	= std_source_location::current()
#	endif
)
{
	assert_always(
		condition, //
		print,
		source_location
	);
}
#else
inline void assert(
	bool condition,
	const std::function<void(std::ostream&)>& print,
	const utki::source_location& source_location
#	if CFG_CPP >= 20
	= std_source_location::current()
#	endif
)
{}
#endif

/**
 * @brief Assert that condion is true.
 * In debug build this function invokes assert_always().
 * In non-debug build this fuction does nothing.
 * @param condition - condition to check for being true.
 * @param source_location - location of the assert() invocation in the source code.
 */
inline void assert(
	bool condition,
	const utki::source_location& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	utki::assert(
		condition, //
		nullptr,
		source_location
	);
}

// MSVC compiler gives warning about implicit conversion of pointer to bool,
// so define template overloads of the assert for that

template <class object_type>
void assert(
	object_type* p,
	const std::function<void(std::ostream&)>& print,
	const utki::source_location& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	assert(
		p != nullptr, //
		print,
		source_location
	);
}

template <class object_type>
void assert(
	object_type* p,
	const utki::source_location& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	assert(
		p != nullptr, //
		nullptr,
		source_location
	);
}

// smart pointers do not have implicit conversion to bool, so we need to define
// template overloads of the assert function for those

template <class object_type>
void assert(
	const std::shared_ptr<object_type>& p,
	const std::function<void(std::ostream&)>& print,
	const utki::source_location& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	assert(
		p != nullptr, //
		print,
		source_location
	);
}

template <class object_type>
void assert(
	const std::shared_ptr<object_type>& p,
	const utki::source_location& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	assert(
		p != nullptr, //
		nullptr,
		source_location
	);
}

template <class object_type>
void assert(
	const std::unique_ptr<object_type>& p,
	const std::function<void(std::ostream&)>& print,
	const utki::source_location& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	assert(
		p != nullptr, //
		print,
		source_location
	);
}

template <class object_type>
void assert(
	const std::unique_ptr<object_type>& p,
	const utki::source_location& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	assert(
		p != nullptr, //
		nullptr,
		source_location
	);
}

// std::function does not have implicit conversion to bool, so we need to define
// template overloads of the assert function for it

template <class func_type>
void assert(
	const std::function<func_type>& p,
	const std::function<void(std::ostream&)>& print,
	const utki::source_location& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	assert(
		p != nullptr, //
		print,
		source_location
	);
}

template <class func_type>
void assert(
	const std::function<func_type>& p,
	const utki::source_location& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	assert(
		p != nullptr, //
		nullptr,
		source_location
	);
}

} // namespace utki

#ifdef DEBUG

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#	define ASSERT1(condition) utki::assert(condition, SL);
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#	define ASSERT2(condition, print) utki::assert(condition, print, SL);

#else // no DEBUG macro defined

#	define ASSERT1(x)
#	define ASSERT2(x, y)

#endif // ~DEBUG

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASSERT(...) UTKI_GET_MACRO(__VA_ARGS__, _10, _9, _8, _7, _6, _5, _4, _3, ASSERT2, ASSERT1)(__VA_ARGS__)

namespace utki {

/**
 * @brief Demangle C++ type name.
 * Tries to demangle C++ type name.
 * Implementation uses compiler specific API for demangling.
 * For unknown compilers it just returns the name as is.
 *
 * Example:
 * @code{.cpp}
 * some_class a;
 * auto name = utki::demangle(typeid(a));
 * @endcode
 *
 * @param type_info - C++ type info of the type to demangle.
 * @return Demangled name.
 */
std::string demangle(const std::type_info& type_info);

} // namespace utki
