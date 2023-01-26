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

#pragma once

#include <functional>
#include <memory>
#include <sstream>

#include "config.hpp"
#include "macros.hpp"

#if CFG_CPP >= 20
#	if __has_include(<source_location>)
#		include <source_location>

namespace utki {
typedef std::source_location std_source_location;
} // namespace utki
#	else
#		include <experimental/source_location>

namespace utki {
typedef std::experimental::source_location std_source_location;
} // namespace utki
#	endif
#endif

#if CFG_OS_NAME == CFG_OS_NAME_ANDROID
#	include <android/log.h>

#else
#	include <iostream>

#endif

#if defined(_DEBUG) && !defined(DEBUG)
#	define DEBUG
#endif

#ifdef assert
#	undef assert
#endif

namespace utki {

void log(const std::function<void(std::ostream&)>& print);

} // namespace utki

#ifdef DEBUG
#	define LOG(print) utki::log(print);
#else
#	define LOG(x)
#endif

//
//
//  Assertion definitions
//
//

namespace utki {

// backport of std::source_location
struct source_location {
	constexpr source_location(
		uint_least32_t l = 0,
		uint_least32_t c = 0,
		char const* fn = nullptr,
		char const* fnn = nullptr
	) noexcept :
		line_{l},
		column_{c},
		file_name_{fn},
		function_name_{fnn}
	{}

#if CFG_CPP >= 20
	constexpr source_location(const std_source_location& sl) :
		source_location(sl.line(), sl.column(), sl.file_name(), sl.function_name())
	{}
#endif

	constexpr auto line() const noexcept -> uint_least32_t
	{
		return this->line_;
	}

	constexpr auto column() const noexcept -> uint_least32_t
	{
		return this->column_;
	}

	constexpr auto file_name() const noexcept -> char const*
	{
		return this->file_name_;
	}

	constexpr auto function_name() const noexcept -> char const*
	{
		return this->function_name_;
	}

private:
	uint_least32_t line_;
	uint_least32_t column_;
	char const* file_name_;
	char const* function_name_;
};
} // namespace utki

/**
 * @brief Source location macro.
 * Constructs an object which holds current filename and current line number.
 */
#if CFG_CPP >= 20
#	define SL utki::std_source_location::current()
#else
#	define SL utki::source_location(__LINE__, 0, __FILE__, __func__)
#endif

namespace utki {

void assert(
	bool condition,
	const std::function<void(std::ostream&)>& print,
	utki::source_location&& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
);

inline void assert(
	bool condition,
	utki::source_location&& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	utki::assert(condition, nullptr, std::move(source_location));
}

// MSVC compiler gives warning about implicit conversion of pointer to bool,
// so define template overloads of the assert for that

template <class object_type>
void assert(
	object_type* p,
	const std::function<void(std::ostream&)>& print,
	utki::source_location&& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	assert(p != nullptr, print, std::move(source_location));
}

template <class object_type>
void assert(
	object_type* p,
	utki::source_location&& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	assert(p != nullptr, nullptr, std::move(source_location));
}

// smart pointers do not have implicit conversion to bool, so we need to define
// template overloads of the assert function for those

template <class object_type>
void assert(
	const std::shared_ptr<object_type>& p,
	const std::function<void(std::ostream&)>& print,
	utki::source_location&& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	assert(p != nullptr, print, std::move(source_location));
}

template <class object_type>
void assert(
	const std::shared_ptr<object_type>& p,
	utki::source_location&& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	assert(p != nullptr, nullptr, std::move(source_location));
}

template <class object_type>
void assert(
	const std::unique_ptr<object_type>& p,
	const std::function<void(std::ostream&)>& print,
	utki::source_location&& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	assert(p != nullptr, print, std::move(source_location));
}

template <class object_type>
void assert(
	const std::unique_ptr<object_type>& p,
	utki::source_location&& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	assert(p != nullptr, nullptr, std::move(source_location));
}

// std::function does not have implicit conversion to bool, so we need to define
// template overloads of the assert function for it

template <class func_type>
void assert(
	const std::function<func_type>& p,
	const std::function<void(std::ostream&)>& print,
	utki::source_location&& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	assert(p != nullptr, print, std::move(source_location));
}

template <class func_type>
void assert(
	const std::function<func_type>& p,
	utki::source_location&& source_location
#if CFG_CPP >= 20
	= std_source_location::current()
#endif
)
{
	assert(p != nullptr, nullptr, std::move(source_location));
}

} // namespace utki

#ifdef DEBUG

#	define ASSERT1(condition) utki::assert(condition, SL);
#	define ASSERT2(condition, print) utki::assert(condition, print, SL);

#else // no DEBUG macro defined

#	define ASSERT1(x)
#	define ASSERT2(x, y)

#endif // ~DEBUG

#define ASSERT(...) UTKI_GET_MACRO(__VA_ARGS__, _10, _9, _8, _7, _6, _5, _4, _3, ASSERT2, ASSERT1)(__VA_ARGS__)
