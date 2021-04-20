#pragma once

#include "config.hpp"
#include "macros.hpp"

#include <functional>
#include <sstream>
#include <memory>

#if M_OS_NAME == M_OS_NAME_ANDROID
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

namespace utki{

void log(const std::function<void(std::ostream&)>& print);

}

#if M_OS_NAME == M_OS_NAME_ANDROID

// TODO: deprecated, remove.
#	define TRACE_ALWAYS(x) \
		{ \
			std::stringstream ss; \
			ss x; \
			__android_log_write(ANDROID_LOG_INFO, "utki", ss.str().c_str()); \
		}

// TODO: deprecated, remove.
#	define LOG_ALWAYS(x) \
		{ \
			std::stringstream log_string_stream; \
			log_string_stream << x; \
			__android_log_write(ANDROID_LOG_INFO, "utki", log_string_stream.str().c_str()); \
		}

#else

// TODO: deprecated, remove.
#	define TRACE_ALWAYS(x) std::cout x; std::cout.flush();

// TODO: deprecated, remove.
#	define LOG_ALWAYS(x) std::cout << x; std::cout.flush();

#endif

#ifdef DEBUG

// TODO: deprecated, remove.
#	define TRACE(x) TRACE_ALWAYS(x)

#	define LOG(print) utki::log(print);
#else

// TODO: deprecated, remove.
#	define TRACE(x)

#	define LOG(x)
#endif

//
//
//  Assertion definitions
//
//

namespace utki{
// backport of std::source_location
struct source_location {
    constexpr source_location(
			uint_least32_t l,
			uint_least32_t c,
			char const* fn,
			char const* fnn
		)noexcept :
			line_{l},
			column_{c},
			file_name_{fn},
			function_name_{fnn} 
    {}

    constexpr auto line() const noexcept            -> uint_least32_t   { return line_; }
    constexpr auto column() const noexcept          -> uint_least32_t   { return column_; }
    constexpr auto file_name() const noexcept       -> char const*      { return file_name_; }
    constexpr auto function_name() const noexcept   -> char const*      { return function_name_; }

    private:
        uint_least32_t line_;
        uint_least32_t column_;
        char const* file_name_;
        char const* function_name_;
};
}

/**
 * @brief Source location macro.
 * Constructs an object which holds current filename and current line number.
 */
#define SL utki::source_location(__LINE__, 0, __FILE__, __func__)

namespace utki{

void assert(
		bool condition,
		const std::function<void(std::ostream&)>& print,
		utki::source_location source_location
	);

inline void assert(bool condition, utki::source_location source_location){
	utki::assert(condition, nullptr, source_location);
}

// MSVC compiler gives warning about implicit conversion of pointer to bool,
// so define template overloads of the assert for that

template <class type>
void assert(
		type* p,
		const std::function<void(std::ostream&)>& print,
		utki::source_location source_location
	)
{
	assert(p != nullptr, print, source_location);
}

template <class type>
void assert(
		type* p,
		utki::source_location source_location
	)
{
	assert(p != nullptr, nullptr, source_location);
}

// smart pointers do not have implicit conversion to bool, so we need to define
// template overloads of the assert function for those

template <class type>
void assert(
		const std::shared_ptr<type>& p,
		const std::function<void(std::ostream&)>& print,
		utki::source_location source_location
	)
{
	assert(p != nullptr, print, source_location);
}

template <class type>
void assert(
		const std::shared_ptr<type>& p,
		utki::source_location source_location
	)
{
	assert(p != nullptr, nullptr, source_location);
}

template <class type>
void assert(
		const std::unique_ptr<type>& p,
		const std::function<void(std::ostream&)>& print,
		utki::source_location source_location
	)
{
	assert(p != nullptr, print, source_location);
}

template <class type>
void assert(
		const std::unique_ptr<type>& p,
		utki::source_location source_location
	)
{
	assert(p != nullptr, nullptr, source_location);
}

// std::function does not have implicit conversion to bool, so we need to define
// template overloads of the assert function for it

template <class type>
void assert(
		const std::function<type>& p,
		const std::function<void(std::ostream&)>& print,
		utki::source_location source_location
	)
{
	assert(p != nullptr, print, source_location);
}

template <class type>
void assert(
		const std::function<type>& p,
		utki::source_location source_location
	)
{
	assert(p != nullptr, nullptr, source_location);
}

}

// TODO: deprecated, remove.
#define ASSERT_INFO_ALWAYS(x, y) utki::assert(x, [&](auto& assert_output_stream){assert_output_stream << y;}, SL);

// TODO: deprecated, remove.
#define ASSERT_ALWAYS(x) ASSERT_INFO_ALWAYS(x, "no additional info")

#ifdef DEBUG

// TODO: deprecated, remove.
#	define ASSERT_INFO(x, y) utki::assert(x, [&](auto& assert_output_stream){assert_output_stream << y;}, SL);

#	define ASSERT1(condition) utki::assert(condition, SL);
#	define ASSERT2(condition, print) utki::assert(condition, print, SL);

#else // no DEBUG macro defined

// TODO: deprecated, remove.
#	define ASSERT_INFO(x, y)

#	define ASSERT1(x)
#	define ASSERT2(x, y)

#endif // ~DEBUG

#define ASSERT(...) UTKI_GET_MACRO(__VA_ARGS__, _10, _9, _8, _7, _6, _5, _4, _3, ASSERT2, ASSERT1)(__VA_ARGS__)
