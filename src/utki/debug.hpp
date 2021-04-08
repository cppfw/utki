#pragma once

#include "config.hpp"

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

/**
 * @brief Source location macro.
 * Constructs an object which holds current filename and current line number.
 */
#define SL std::make_pair(__FILE__, size_t(__LINE__))

namespace utki{

void assert(
		bool condition,
		const std::function<void(std::ostream&)>& print,
		std::pair<const char*, size_t> source_location
	);

inline void assert(bool condition, std::pair<const char*, size_t> source_location){
	utki::assert(condition, nullptr, source_location);
}

// MSVC compiler gives warning about implicit conversion of pointer to bool,
// so define template overloads of the assert for that

template <class type>
void assert(
		type* p,
		const std::function<void(std::ostream&)>& print,
		std::pair<const char*, size_t> source_location
	)
{
	assert(p != nullptr, print, source_location);
}

template <class type>
void assert(
		type* p,
		std::pair<const char*, size_t> source_location
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
		std::pair<const char*, size_t> source_location
	)
{
	assert(p != nullptr, print, source_location);
}

template <class type>
void assert(
		const std::shared_ptr<type>& p,
		std::pair<const char*, size_t> source_location
	)
{
	assert(p != nullptr, nullptr, source_location);
}

template <class type>
void assert(
		const std::unique_ptr<type>& p,
		const std::function<void(std::ostream&)>& print,
		std::pair<const char*, size_t> source_location
	)
{
	assert(p != nullptr, print, source_location);
}

template <class type>
void assert(
		const std::unique_ptr<type>& p,
		std::pair<const char*, size_t> source_location
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
		std::pair<const char*, size_t> source_location
	)
{
	assert(p != nullptr, print, source_location);
}

template <class type>
void assert(
		const std::function<type>& p,
		std::pair<const char*, size_t> source_location
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

#define UTKI_GET_MACRO(_1, _2, NAME, ...) NAME
#define ASSERT(...) UTKI_GET_MACRO(__VA_ARGS__, ASSERT2, ASSERT1)(__VA_ARGS__)
