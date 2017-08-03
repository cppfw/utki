#pragma once

#include "config.hpp"

#if M_OS == M_OS_SYMBIAN
#	include <e32std.h>

#elif M_OS_NAME == M_OS_NAME_ANDROID
#	undef NDEBUG // we want assertions to work, if we don't undef NDEBUG the assertions will be translated to nothing
#	include <cassert>

#	include <sstream>
#	include <android/log.h>

#else //assume more or less standard system
#	include <sstream>
#	include <iostream>
#	include <fstream>
#	include <typeinfo>
#	include <cassert>

#endif

#if defined(_DEBUG) && !defined(DEBUG)
#	define DEBUG
#endif



#if M_OS == M_OS_SYMBIAN
#	define TRACE_ALWAYS(x)

#elif M_OS_NAME == M_OS_NAME_ANDROID
#	define TRACE_ALWAYS(x) \
		{ \
			std::stringstream ss; \
			ss x; \
			__android_log_write(ANDROID_LOG_INFO, "utki", ss.str().c_str()); \
		}

#else
#	define TRACE_ALWAYS(x) std::cout x; std::cout.flush();

#endif



#ifdef DEBUG
#	define TRACE(x) TRACE_ALWAYS(x)
#else
#	define TRACE(x)
#endif



//
//
//  Assertion definitions
//
//
namespace utki{

inline void logAssert(const char* msg, const char* file, int line){
	TRACE_ALWAYS(<< "[!!!fatal] Assertion failed at:\n\t"<< file << ":" << line << "| " << msg << std::endl)
}

}
#if M_OS == M_OS_SYMBIAN
#	define ASSERT_INFO_ALWAYS(x, y) __ASSERT_ALWAYS((x), User::Panic(_L("ASSERTION FAILED!"),3));

#else //Assume system supporting standard assert() (including Android)

#	define ASSERT_INFO_ALWAYS(x, y) if(!(x)){ \
						std::stringstream ss; \
						ss << y; \
						utki::logAssert(ss.str().c_str(), __FILE__, __LINE__); \
						assert(false); \
					}

#endif

#define ASSERT_ALWAYS(x) ASSERT_INFO_ALWAYS((x), "no additional info")


#ifdef DEBUG
#	define ASSERT_INFO(x, y) ASSERT_INFO_ALWAYS((x), y)
#	define ASSERT(x) ASSERT_ALWAYS(x)

#else //No DEBUG macro defined
#	define ASSERT_INFO(x, y)
#	define ASSERT(x)

#endif//~#ifdef DEBUG
