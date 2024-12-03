/*
The MIT License (MIT)

utki - Utility Kit for C++.

Copyright (c) 2015-2024 Ivan Gagis <igagis@gmail.com>

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

// NOLINTBEGIN

//====================================================|
//            C++ standard                            |
//                                                    |

#if __cplusplus > 201703L
#	define CFG_CPP 20 // NOLINT(cppcoreguidelines-macro-usage)
#elif __cplusplus > 201402L
#	define CFG_CPP 17 // NOLINT(cppcoreguidelines-macro-usage)
#elif __cplusplus > 201103L
#	define CFG_CPP 14 // NOLINT(cppcoreguidelines-macro-usage)
#elif __cplusplus > 199711L
#	define CFG_CPP 11 // NOLINT(cppcoreguidelines-macro-usage)
#else
#	define CFG_CPP 3 // NOLINT(cppcoreguidelines-macro-usage)
#endif

//======================================|
//            OS definitions            |
//                                      |

// NOLINTBEGIN(modernize-macro-to-enum, cppcoreguidelines-macro-to-enum)

// OS family
#define CFG_OS_UNKNOWN 0 // NOLINT(cppcoreguidelines-macro-usage)
#define CFG_OS_LINUX 1 // NOLINT(cppcoreguidelines-macro-usage)
#define CFG_OS_WINDOWS 2 // NOLINT(cppcoreguidelines-macro-usage)
#define CFG_OS_MACOSX 3 // NOLINT(cppcoreguidelines-macro-usage)
#define CFG_OS_UNIX 4 // NOLINT(cppcoreguidelines-macro-usage)

// OS name
#define CFG_OS_NAME_UNKNOWN 0 // NOLINT(cppcoreguidelines-macro-usage)

// when CFG_OS == CFG_OS_MACOSX
#define CFG_OS_NAME_MACOSX 1 // NOLINT(cppcoreguidelines-macro-usage)

// when CFG_OS == CFG_OS_MACOSX
#define CFG_OS_NAME_IOS 2 // NOLINT(cppcoreguidelines-macro-usage)

// when CFG_OS == CFG_OS_LINUX
#define CFG_OS_NAME_ANDROID 3 // NOLINT(cppcoreguidelines-macro-usage)

// when CFG_OS == CFG_OS_UNIX
#define CFG_OS_NAME_SOLARIS 4 // NOLINT(cppcoreguidelines-macro-usage)

// when CFG_OS == CFG_OS_UNIX
#define CFG_OS_NAME_FREEBSD 5 // NOLINT(cppcoreguidelines-macro-usage)

// when CFG_OS == CFG_OS_UNIX
#define CFG_OS_NAME_EMSCRIPTEN 6 // NOLINT(cppcoreguidelines-macro-usage)

// NOLINTEND(modernize-macro-to-enum, cppcoreguidelines-macro-to-enum)

#if defined(__linux__)
#	define CFG_OS CFG_OS_LINUX // NOLINT(cppcoreguidelines-macro-usage)
#	if defined(__ANDROID__)
#		define CFG_OS_NAME CFG_OS_NAME_ANDROID // NOLINT(cppcoreguidelines-macro-usage)
#	else
#		define CFG_OS_NAME CFG_OS_NAME_UNKNOWN // NOLINT(cppcoreguidelines-macro-usage)
#	endif

// _WIN32 macro is defined for both win32 and win64. _WIN32 is the correct one, WIN32 is not always defined.
#elif defined(_WIN32) || defined(WIN32)
#	define CFG_OS CFG_OS_WINDOWS // NOLINT(cppcoreguidelines-macro-usage)
#	define CFG_OS_NAME CFG_OS_NAME_UNKNOWN // NOLINT(cppcoreguidelines-macro-usage)
#elif defined(__APPLE__)
#	define CFG_OS CFG_OS_MACOSX // NOLINT(cppcoreguidelines-macro-usage)
#	include <TargetConditionals.h>
#	include <Availability.h>
#	if TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1
#		define CFG_OS_NAME CFG_OS_NAME_IOS // NOLINT(cppcoreguidelines-macro-usage)

/**
 * @brief IOS deployment target.
 * Example values:
 * - iOS 6.1 = 60100
 * - iOS 12.3 = 120300
 * - iOS 13.0 = 130000
 */
#		define CFG_IOS_DEPLOYMENT_TARGET __IPHONE_OS_VERSION_MIN_REQUIRED
#	else
#		define CFG_OS_NAME CFG_OS_NAME_MACOSX // NOLINT(cppcoreguidelines-macro-usage)

// macosx deployment target, e.g. value of 1090 means MacOS X 10.9
#		define CFG_MACOSX_DEPLOYMENT_TARGET __MAC_OS_X_VERSION_MIN_REQUIRED
#	endif

// check for UNIX should go after check for Linux, because on Linux the __unix macro is also defined
#elif defined(__unix) || defined(__unix__)
#	define CFG_OS CFG_OS_UNIX // NOLINT(cppcoreguidelines-macro-usage)
#	if defined(sun) || defined(__sun)
#		define CFG_OS_NAME CFG_OS_NAME_SOLARIS // NOLINT(cppcoreguidelines-macro-usage)
#	elif defined(__FreeBSD__)
#		define CFG_OS_NAME CFG_OS_NAME_FREEBSD // NOLINT(cppcoreguidelines-macro-usage)
#	elif defined(__EMSCRIPTEN__)
#		define CFG_OS_NAME CFG_OS_NAME_EMSCRIPTEN // NOLINT(cppcoreguidelines-macro-usage)
#	else
#		define CFG_OS_NAME CFG_OS_NAME_UNKNOWN // NOLINT(cppcoreguidelines-macro-usage)
#	endif
#else
#	define CFG_OS CFG_OS_UNKNOWN // NOLINT(cppcoreguidelines-macro-usage)
#	define CFG_OS_NAME CFG_OS_NAME_UNKNOWN // NOLINT(cppcoreguidelines-macro-usage)
#endif

//====================================================|
//            Compiler definitions                    |
//                                                    |

// NOLINTBEGIN(modernize-macro-to-enum, cppcoreguidelines-macro-to-enum)
#define CFG_COMPILER_UNKNOWN 0 // NOLINT(cppcoreguidelines-macro-usage)
#define CFG_COMPILER_GCC 1 // NOLINT(cppcoreguidelines-macro-usage)
#define CFG_COMPILER_MSVC 2 // NOLINT(cppcoreguidelines-macro-usage)
#define CFG_COMPILER_CLANG 3 // NOLINT(cppcoreguidelines-macro-usage)
// NOLINTEND(modernize-macro-to-enum, cppcoreguidelines-macro-to-enum)

#if defined(_MSC_VER)
#	define CFG_COMPILER CFG_COMPILER_MSVC // NOLINT(cppcoreguidelines-macro-usage)
#	if _MSC_VER < 1916
#		define CFG_COMPILER_MSVC_TOOLS_V 140 // NOLINT(cppcoreguidelines-macro-usage)
#	elif _MSC_VER < 1929
#		define CFG_COMPILER_MSVC_TOOLS_V 141 // NOLINT(cppcoreguidelines-macro-usage)
#	elif _MSC_VER < 1930
#		define CFG_COMPILER_MSVC_TOOLS_V 142 // NOLINT(cppcoreguidelines-macro-usage)
#	else
#		define CFG_COMPILER_MSVC_TOOLS_V 143 // NOLINT(cppcoreguidelines-macro-usage)
#	endif

// NOTE: clang check should go before GCC check because some clang-based copmilers
// (e.g. emscripten) define both macros due to compatibility with GCC.
#elif defined(__clang__)
#	define CFG_COMPILER CFG_COMPILER_CLANG // NOLINT(cppcoreguidelines-macro-usage)

#elif defined(__GNUC__) || defined(__GNUG__)
#	define CFG_COMPILER CFG_COMPILER_GCC // NOLINT(cppcoreguidelines-macro-usage)
#	define CFG_COMPILER_VERSION_MAJOR __GNUC__ // NOLINT(cppcoreguidelines-macro-usage)
#	define CFG_COMPILER_VERSION_MINOR __GNUC_MINOR__ // NOLINT(cppcoreguidelines-macro-usage)
#	define CFG_COMPILER_VERSION_PATCH __GNUC_PATCHLEVEL__ // NOLINT(cppcoreguidelines-macro-usage)

#else
#	define CFG_COMPILER CFG_COMPILER_UNKNOWN // NOLINT(cppcoreguidelines-macro-usage)
#endif

//====================================================|
//            CPU architecture definitions            |
//                                                    |

// NOLINTBEGIN(modernize-macro-to-enum, cppcoreguidelines-macro-to-enum)
#define CFG_CPU_UNKNOWN 0 // NOLINT(cppcoreguidelines-macro-usage)
#define CFG_CPU_X86 1 // NOLINT(cppcoreguidelines-macro-usage)
#define CFG_CPU_X86_64 2 // NOLINT(cppcoreguidelines-macro-usage)
#define CFG_CPU_ARM 3 // NOLINT(cppcoreguidelines-macro-usage)
#define CFG_CPU_WASM 4 // NOLINT(cppcoreguidelines-macro-usage)
// NOLINTEND(modernize-macro-to-enum, cppcoreguidelines-macro-to-enum)

#if CFG_OS_NAME == CFG_OS_NAME_EMSCRIPTEN
#	define CFG_CPU CFG_CPU_WASM
#	define CFG_CPU_BITS 32

#elif CFG_COMPILER == CFG_COMPILER_GCC
#	if defined(__i386__) // __i386__ is defined for any x86 processor
#		define CFG_CPU CFG_CPU_X86 // NOLINT(cppcoreguidelines-macro-usage)
#		define CFG_CPU_BITS 32 // NOLINT(cppcoreguidelines-macro-usage)

#		if defined(__i686__)
#			define CFG_CPU_VERSION 6 // NOLINT(cppcoreguidelines-macro-usage)
#		elif defined(__i586__)
#			define CFG_CPU_VERSION 5 // NOLINT(cppcoreguidelines-macro-usage)
#		elif defined(__i486__)
#			define CFG_CPU_VERSION 4 // NOLINT(cppcoreguidelines-macro-usage)
#		else
#			define CFG_CPU_VERSION 3 // NOLINT(cppcoreguidelines-macro-usage)
#		endif

#	elif defined(__x86_64__)
#		define CFG_CPU CFG_CPU_X86_64 // NOLINT(cppcoreguidelines-macro-usage)
#		define CFG_CPU_BITS 64 // NOLINT(cppcoreguidelines-macro-usage)
#		define CFG_CPU_VERSION 0 // NOLINT(cppcoreguidelines-macro-usage)

#	elif defined(__arm__)
#		define CFG_CPU CFG_CPU_ARM // NOLINT(cppcoreguidelines-macro-usage)
#		define CFG_CPU_BITS 32 // NOLINT(cppcoreguidelines-macro-usage)

#		if defined(__thumb2__) // this macro is defined when targeting only thumb-2
#			define CFG_CPU_ARM_THUMB_VERSION 2 // NOLINT(cppcoreguidelines-macro-usage)
#		elif defined(__thumb__) // this macro is defined when targeting any, thumb-1 or thumb-2
#			define CFG_CPU_ARM_THUMB_VERSION 1 // NOLINT(cppcoreguidelines-macro-usage)
#		endif

#		if defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__)
#			define CFG_CPU_VERSION 7 // NOLINT(cppcoreguidelines-macro-usage)
#		elif defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6Z__) || \
			defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6__)

#			define CFG_CPU_VERSION 6 // NOLINT(cppcoreguidelines-macro-usage)
#		elif defined(__ARM_ARCH_5TEJ__) || defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5T__) || \
			defined(__ARM_ARCH_5E__) || defined(__ARM_ARCH_5__)

#			define CFG_CPU_VERSION 5 // NOLINT(cppcoreguidelines-macro-usage)
#		elif defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_4__)
#			define CFG_CPU_VERSION 4 // NOLINT(cppcoreguidelines-macro-usage)
#		else
#			define CFG_CPU_VERSION 0 // NOLINT(cppcoreguidelines-macro-usage)
#		endif

#	elif defined(__aarch64__)
#		define CFG_CPU CFG_CPU_ARM // NOLINT(cppcoreguidelines-macro-usage)
#		define CFG_CPU_BITS 64 // NOLINT(cppcoreguidelines-macro-usage)
#		define CFG_CPU_VERSION 8 // NOLINT(cppcoreguidelines-macro-usage)

#	else
#		define CFG_CPU CFG_CPU_UNKNOWN // NOLINT(cppcoreguidelines-macro-usage)
#		define CFG_CPU_BITS 0 // NOLINT(cppcoreguidelines-macro-usage)
#		define CFG_CPU_VERSION 0 // NOLINT(cppcoreguidelines-macro-usage)
#	endif

#elif CFG_COMPILER == CFG_COMPILER_MSVC
#	if defined(_M_IX86)
#		define CFG_CPU CFG_CPU_X86 // NOLINT(cppcoreguidelines-macro-usage)
#		define CFG_CPU_BITS 32 // NOLINT(cppcoreguidelines-macro-usage)

#		if _M_IX86 == 600
#			define CFG_CPU_VERSION 6 // NOLINT(cppcoreguidelines-macro-usage)
#		elif _M_IX86 == 500
#			define CFG_CPU_VERSION 5 // NOLINT(cppcoreguidelines-macro-usage)
#		elif _M_IX86 == 400
#			define CFG_CPU_VERSION 4 // NOLINT(cppcoreguidelines-macro-usage)
#		else
#			define CFG_CPU_VERSION 3 // NOLINT(cppcoreguidelines-macro-usage)
#		endif

#	elif defined(_M_AMD64) || defined(_M_X64)
#		define CFG_CPU CFG_CPU_X86_64 // NOLINT(cppcoreguidelines-macro-usage)
#		define CFG_CPU_BITS 64 // NOLINT(cppcoreguidelines-macro-usage)
#		define CFG_CPU_VERSION 0 // NOLINT(cppcoreguidelines-macro-usage)

#	else
#		define CFG_CPU CFG_CPU_UNKNOWN // NOLINT(cppcoreguidelines-macro-usage)
#		define CFG_CPU_BITS 0 // NOLINT(cppcoreguidelines-macro-usage)
#		define CFG_CPU_VERSION 0 // NOLINT(cppcoreguidelines-macro-usage)
#	endif
#else
#	define CFG_CPU CFG_CPU_UNKNOWN // NOLINT(cppcoreguidelines-macro-usage)
#	define CFG_CPU_BITS 0 // NOLINT(cppcoreguidelines-macro-usage)
#	define CFG_CPU_VERSION 0 // NOLINT(cppcoreguidelines-macro-usage)
#endif

//====================================================|
//                DLL Export definitions              |
//                                                    |

#if CFG_OS == CFG_OS_WINDOWS
#	if CFG_COMPILER == CFG_COMPILER_MSVC
#		ifdef _USRDLL
#			define DLLEXPORT __declspec(dllexport) // NOLINT(cppcoreguidelines-macro-usage)
#		else
#			define DLLEXPORT __declspec(dllimport) // NOLINT(cppcoreguidelines-macro-usage)
#		endif
#		pragma warning(disable : 4251) // member variable needs to have dll-interface to be used by clients of class
#		pragma warning(disable : 4275) // non dll-interface class used as base for dll-interface class
#		pragma warning(disable : 4250) // method inherited via dominance
#	else
#		define DLLEXPORT // NOLINT(cppcoreguidelines-macro-usage)
#	endif
#else
#	define DLLEXPORT // NOLINT(cppcoreguidelines-macro-usage)
#endif

//====================================================|
//            CPU endianness definitions              |
//                                                    |

// NOLINTBEGIN(modernize-macro-to-enum, cppcoreguidelines-macro-to-enum)
#define CFG_ENDIANNESS_UNKNOWN 0 // NOLINT(cppcoreguidelines-macro-usage)
#define CFG_ENDIANNESS_LITTLE 1 // NOLINT(cppcoreguidelines-macro-usage)
#define CFG_ENDIANNESS_BIG 2 // NOLINT(cppcoreguidelines-macro-usage)
// NOLINTEND(modernize-macro-to-enum, cppcoreguidelines-macro-to-enum)

#if CFG_OS_NAME == CFG_OS_NAME_EMSCRIPTEN
#	define CFG_ENDIANNESS CFG_ENDIANNESS_LITTLE

#elif CFG_OS == CFG_OS_WINDOWS
// Windows 32/64 bit is always little endian
#	define CFG_ENDIANNESS CFG_ENDIANNESS_LITTLE // NOLINT(cppcoreguidelines-macro-usage)

#else
// clang-format off
#	if (defined(__ORDER_BIG_ENDIAN__) && \
		 (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ || __FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__)) || \
		defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || \
		defined(_MIPSEB) || defined(__MIPSEB) || defined(__MIPSEB__)
#		define CFG_ENDIANNESS CFG_ENDIANNESS_BIG // NOLINT(cppcoreguidelines-macro-usage)
#	elif ( \
		defined(__ORDER_LITTLE_ENDIAN__) && \
		(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ || __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__) \
	) || defined(__LITTLE_ENDIAN__) || \
		defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(_MIPSEL) || \
		defined(__MIPSEL) || defined(__MIPSEL__)
#		define CFG_ENDIANNESS CFG_ENDIANNESS_LITTLE // NOLINT(cppcoreguidelines-macro-usage)
#	else
#		define CFG_ENDIANNESS CFG_ENDIANNESS_UNKNOWN // NOLINT(cppcoreguidelines-macro-usage)
#	endif
// clang-format on
#endif

// NOLINTEND
