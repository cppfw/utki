#pragma once

#include "config.hpp"

#include <functional>
#include <sstream>

#if M_OS_NAME == M_OS_NAME_ANDROID
#	include <android/log.h>

#else
#	include <iostream>

#endif

#if defined(_DEBUG) && !defined(DEBUG)
#	define DEBUG
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

#	define LOG(x) utki::log([&](auto&o){o << x;});
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
#define SL std::make_pair(__FILE__, __LINE__)

namespace utki{

void assert(
		bool condition,
		const std::function<void(std::ostream&)>& print,
		std::pair<const char*, size_t> source_location
	);

inline void assert(bool condition, std::pair<const char*, size_t> source_location){
	utki::assert(condition, nullptr, source_location);
}

}

// TODO: deprecated, remove.
#define ASSERT_INFO_ALWAYS(x, y) \
	if(!(x)){ \
		std::stringstream assert_info_always_string_stream; \
		assert_info_always_string_stream << y; \
		utki::assert(false, [&](auto&o){o << assert_info_always_string_stream.str();}, SL); \
	}

// TODO: deprecated, remove.
#define ASSERT_ALWAYS(x) ASSERT_INFO_ALWAYS((x), "no additional info")

#ifdef DEBUG

// TODO: deprecated, remove.
#	define ASSERT_INFO(x, y) \
	if(!(x)){ \
		LOG_ALWAYS(__FILE__ << ":" << __LINE__ << ": \e[1;31merror\e[0m: assertion failed:" << std::endl << "  " << y << std::endl) \
		abort(); \
	}

#	define ASSERT1(condition) utki::assert(condition, SL);
#	define ASSERT2(condition, print) utki::assert(condition, print, SL);

#else // No DEBUG macro defined

// TODO: deprecated, remove.
#	define ASSERT_INFO(x, y)

#	define ASSERT1(x)
#	define ASSERT2(x, y)

#endif // ~DEBUG

#define UTKI_GET_MACRO(_1, _2, NAME, ...) NAME
#define ASSERT(...) UTKI_GET_MACRO(__VA_ARGS__, ASSERT2, ASSERT1)(__VA_ARGS__)
