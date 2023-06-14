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

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

// TODO: M_ prefixed macros are deprecated, remove those when all usages are migrated to new macros.

//====================================================|
//            C++ standard                            |
//                                                    |

#if __cplusplus > 201703L
#	define M_CPP 20 // NOLINT(cppcoreguidelines-macro-usage)
#	define CFG_CPP 20 // NOLINT(cppcoreguidelines-macro-usage)
#elif __cplusplus > 201402L
#	define M_CPP 17 // NOLINT(cppcoreguidelines-macro-usage)
#	define CFG_CPP 17 // NOLINT(cppcoreguidelines-macro-usage)
#elif __cplusplus > 201103L
#	define M_CPP 14 // NOLINT(cppcoreguidelines-macro-usage)
#	define CFG_CPP 14 // NOLINT(cppcoreguidelines-macro-usage)
#elif __cplusplus > 199711L
#	define M_CPP 11 // NOLINT(cppcoreguidelines-macro-usage)
#	define CFG_CPP 11 // NOLINT(cppcoreguidelines-macro-usage)
#else
#	define M_CPP 3 // NOLINT(cppcoreguidelines-macro-usage)
#	define CFG_CPP 3 // NOLINT(cppcoreguidelines-macro-usage)
#endif

//====================================================|
//            Compiler definitions                    |
//                                                    |

// NOLINTBEGIN(modernize-macro-to-enum)
#define CFG_COMPILER_UNKNOWN 0 // NOLINT(cppcoreguidelines-macro-usage)
#define CFG_COMPILER_GCC 1 // NOLINT(cppcoreguidelines-macro-usage)
#define CFG_COMPILER_MSVC 2 // NOLINT(cppcoreguidelines-macro-usage)
#define CFG_COMPILER_CLANG 3 // NOLINT(cppcoreguidelines-macro-usage)

#define M_COMPILER_UNKNOWN 0 // NOLINT(cppcoreguidelines-macro-usage)
#define M_COMPILER_GCC 1 // NOLINT(cppcoreguidelines-macro-usage)
#define M_COMPILER_MSVC 2 // NOLINT(cppcoreguidelines-macro-usage)
#define M_COMPILER_CLANG 3 // NOLINT(cppcoreguidelines-macro-usage)
// NOLINTEND(modernize-macro-to-enum)

#if defined(__GNUC__) || defined(__GNUG__)
#	define CFG_COMPILER CFG_COMPILER_GCC
#	define CFG_COMPILER_VERSION_MAJOR __GNUC__
#	define CFG_COMPILER_VERSION_MINOR __GNUC_MINOR__
#	define CFG_COMPILER_VERSION_PATCH __GNUC_PATCHLEVEL__

#	define M_COMPILER M_COMPILER_GCC
#	define M_COMPILER_VERSION_MAJOR __GNUC__
#	define M_COMPILER_VERSION_MINOR __GNUC_MINOR__
#	define M_COMPILER_VERSION_PATCH __GNUC_PATCHLEVEL__
#elif defined(_MSC_VER)
#	define CFG_COMPILER CFG_COMPILER_MSVC
#	define M_COMPILER M_COMPILER_MSVC
#	if _MSC_VER < 1916
#		define CFG_COMPILER_MSVC_TOOLS_V 140
#		define M_COMPILER_MSVC_TOOLS_V 140
#	elif _MSC_VER < 1929
#		define CFG_COMPILER_MSVC_TOOLS_V 141
#		define M_COMPILER_MSVC_TOOLS_V 141
#	elif _MSC_VER < 1930
#		define CFG_COMPILER_MSVC_TOOLS_V 142
#		define M_COMPILER_MSVC_TOOLS_V 142
#	else
#		define CFG_COMPILER_MSVC_TOOLS_V 143
#		define M_COMPILER_MSVC_TOOLS_V 143
#	endif
#elif defined(__clang__)
#	define CFG_COMPILER CFG_COMPILER_CLANG
#	define M_COMPILER M_COMPILER_CLANG
#else
#	define CFG_COMPILER CFG_COMPILER_UNKNOWN
#	define M_COMPILER M_COMPILER_UNKNOWN
#endif

//====================================================|
//            CPU architecture definitions            |
//                                                    |

// NOLINTBEGIN(modernize-macro-to-enum)
#define CFG_CPU_UNKNOWN 0
#define CFG_CPU_X86 1
#define CFG_CPU_X86_64 2
#define CFG_CPU_ARM 3

#define M_CPU_UNKNOWN 0
#define M_CPU_X86 1
#define M_CPU_X86_64 2
#define M_CPU_ARM 3
// NOLINTEND(modernize-macro-to-enum)

#if CFG_COMPILER == CFG_COMPILER_GCC
#	if defined(__i386__) // __i386__ is defined for any x86 processor
#		define CFG_CPU CFG_CPU_X86
#		define CFG_CPU_BITS 32

#		define M_CPU M_CPU_X86
#		define M_CPU_BITS 32

#		if defined(__i686__)
#			define CFG_CPU_VERSION 6
#			define M_CPU_VERSION 6
#		elif defined(__i586__)
#			define CFG_CPU_VERSION 5
#			define M_CPU_VERSION 5
#		elif defined(__i486__)
#			define CFG_CPU_VERSION 4
#			define M_CPU_VERSION 4
#		else
#			define CFG_CPU_VERSION 3
#			define M_CPU_VERSION 3
#		endif

#	elif defined(__x86_64__)
#		define CFG_CPU CFG_CPU_X86_64
#		define CFG_CPU_BITS 64
#		define CFG_CPU_VERSION 0

#		define M_CPU M_CPU_X86_64
#		define M_CPU_BITS 64
#		define M_CPU_VERSION 0

#	elif defined(__arm__)
#		define CFG_CPU CFG_CPU_ARM
#		define CFG_CPU_BITS 32

#		define M_CPU M_CPU_ARM
#		define M_CPU_BITS 32

#		if defined(__thumb2__) // this macro is defined when targeting only thumb-2
#			define CFG_CPU_ARM_THUMB_VERSION 2
#			define M_CPU_ARM_THUMB_VERSION 2
#		elif defined(__thumb__) // this macro is defined when targeting any, thumb-1 or thumb-2
#			define CFG_CPU_ARM_THUMB_VERSION 1
#			define M_CPU_ARM_THUMB_VERSION 1
#		endif

#		if defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__)
#			define CFG_CPU_VERSION 7
#			define M_CPU_VERSION 7
#		elif defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6Z__) \
			|| defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6__)

#			define CFG_CPU_VERSION 6
#			define M_CPU_VERSION 6
#		elif defined(__ARM_ARCH_5TEJ__) || defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5T__) \
			|| defined(__ARM_ARCH_5E__) || defined(__ARM_ARCH_5__)

#			define CFG_CPU_VERSION 5
#			define M_CPU_VERSION 5
#		elif defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_4__)
#			define CFG_CPU_VERSION 4
#			define M_CPU_VERSION 4
#		else
#			define CFG_CPU_VERSION 0
#			define M_CPU_VERSION 0
#		endif

#	else
#		define CFG_CPU CFG_CPU_UNKNOWN
#		define CFG_CPU_BITS 0
#		define CFG_CPU_VERSION 0

#		define M_CPU M_CPU_UNKNOWN
#		define M_CPU_BITS 0
#		define M_CPU_VERSION 0
#	endif

#elif CFG_COMPILER == CFG_COMPILER_MSVC
#	if defined(_M_IX86)
#		define CFG_CPU CFG_CPU_X86
#		define CFG_CPU_BITS 32

#		define M_CPU M_CPU_X86
#		define M_CPU_BITS 32

#		if _M_IX86 == 600
#			define CFG_CPU_VERSION 6
#			define M_CPU_VERSION 6
#		elif _M_IX86 == 500
#			define CFG_CPU_VERSION 5
#			define M_CPU_VERSION 5
#		elif _M_IX86 == 400
#			define CFG_CPU_VERSION 4
#			define M_CPU_VERSION 4
#		else
#			define CFG_CPU_VERSION 3
#			define M_CPU_VERSION 3
#		endif

#	elif defined(_M_AMD64) || defined(_M_X64)
#		define CFG_CPU CFG_CPU_X86_64
#		define CFG_CPU_BITS 64
#		define CFG_CPU_VERSION 0

#		define M_CPU M_CPU_X86_64
#		define M_CPU_BITS 64
#		define M_CPU_VERSION 0

#	else
#		define CFG_CPU CFG_CPU_UNKNOWN
#		define CFG_CPU_BITS 0
#		define CFG_CPU_VERSION 0

#		define M_CPU M_CPU_UNKNOWN
#		define M_CPU_BITS 0
#		define M_CPU_VERSION 0
#	endif
#else
#	define CFG_CPU CFG_CPU_UNKNOWN
#	define CFG_CPU_BITS 0
#	define CFG_CPU_VERSION 0

#	define M_CPU M_CPU_UNKNOWN
#	define M_CPU_BITS 0
#	define M_CPU_VERSION 0
#endif

//======================================|
//            OS definitions            |
//                                      |

// NOLINTBEGIN(modernize-macro-to-enum)

// OS family
#define CFG_OS_UNKNOWN 0
#define CFG_OS_LINUX 1
#define CFG_OS_WINDOWS 2
#define CFG_OS_MACOSX 3
#define CFG_OS_UNIX 4

#define M_OS_UNKNOWN 0
#define M_OS_LINUX 1
#define M_OS_WINDOWS 2
#define M_OS_MACOSX 3
#define M_OS_UNIX 4

// OS name
#define CFG_OS_NAME_UNKNOWN 0
#define CFG_OS_NAME_MACOSX 1
#define CFG_OS_NAME_IOS 2
#define CFG_OS_NAME_ANDROID 3
#define CFG_OS_NAME_SOLARIS 4

#define M_OS_NAME_UNKNOWN 0
#define M_OS_NAME_MACOSX 1
#define M_OS_NAME_IOS 2
#define M_OS_NAME_ANDROID 3
#define M_OS_NAME_SOLARIS 4

// NOLINTEND(modernize-macro-to-enum)

#if defined(__linux__)
#	define CFG_OS CFG_OS_LINUX
#	define M_OS M_OS_LINUX
#	if defined(__ANDROID__)
#		define CFG_OS_NAME CFG_OS_NAME_ANDROID
#		define M_OS_NAME M_OS_NAME_ANDROID
#	else
#		define CFG_OS_NAME CFG_OS_NAME_UNKNOWN
#		define M_OS_NAME M_OS_NAME_UNKNOWN
#	endif

// _WIN32 macro is defined for both win32 and win64. _WIN32 is the correct one, WIN32 is not always defined.
#elif defined(_WIN32) || defined(WIN32)
#	define CFG_OS CFG_OS_WINDOWS
#	define CFG_OS_NAME CFG_OS_NAME_UNKNOWN

#	define M_OS M_OS_WINDOWS
#	define M_OS_NAME M_OS_NAME_UNKNOWN
#elif defined(__APPLE__)
#	define CFG_OS CFG_OS_MACOSX
#	define M_OS M_OS_MACOSX
#	include <TargetConditionals.h>
#	if TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1
#		define CFG_OS_NAME CFG_OS_NAME_IOS
#		define M_OS_NAME M_OS_NAME_IOS
#	else
#		define CFG_OS_NAME CFG_OS_NAME_MACOSX
#		define M_OS_NAME M_OS_NAME_MACOSX
#	endif

// check for UNIX should go after check for Linux, because on Linux the __unix macro is also defined
#elif defined(__unix) || defined(__unix__)
#	define CFG_OS CFG_OS_UNIX
#	define M_OS M_OS_UNIX
#	if defined(sun) || defined(__sun)
#		define CFG_OS_NAME CFG_OS_NAME_SOLARIS
#		define M_OS_NAME M_OS_NAME_SOLARIS
#	else
#		define CFG_OS_NAME CFG_OS_NAME_UNKNOWN
#		define M_OS_NAME M_OS_NAME_UNKNOWN
#	endif
#else
#	define CFG_OS CFG_OS_UNKNOWN
#	define CFG_OS_NAME CFG_OS_NAME_UNKNOWN

#	define M_OS M_OS_UNKNOWN
#	define M_OS_NAME M_OS_NAME_UNKNOWN
#endif

#if CFG_OS == CFG_OS_WINDOWS
#	if CFG_COMPILER == CFG_COMPILER_MSVC
#		ifdef _USRDLL
#			define DLLEXPORT __declspec(dllexport)
#		else
#			define DLLEXPORT __declspec(dllimport)
#		endif
#		pragma warning(disable : 4251) // member variable needs to have dll-interface to be used by clients of class
#		pragma warning(disable : 4275) // non dll-interface class used as base for dll-interface class
#		pragma warning(disable : 4250) // method inherited via dominance
#	else
#		define DLLEXPORT
#	endif
#else
#	define DLLEXPORT
#endif

//====================================================|
//            CPU endianness definitions              |
//                                                    |

// NOLINTBEGIN(modernize-macro-to-enum)
#define CFG_ENDIANNESS_UNKNOWN 0
#define CFG_ENDIANNESS_LITTLE 1
#define CFG_ENDIANNESS_BIG 2
// NOLINTEND(modernize-macro-to-enum)

#if CFG_OS == CFG_OS_WINDOWS
// Windows 32/64 bit is always little endian
#	define CFG_ENDIANNESS CFG_ENDIANNESS_LITTLE
#else
#	if (defined(__ORDER_BIG_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ || __FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__)) \
		|| defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) \
		|| defined(_MIPSEB) || defined(__MIPSEB) || defined(__MIPSEB__)
#		define CFG_ENDIANNESS CFG_ENDIANNESS_BIG
#	elif ( \
		defined(__ORDER_LITTLE_ENDIAN__) \
		&& (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ || __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__) \
	) || defined(__LITTLE_ENDIAN__) \
		|| defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(_MIPSEL) \
		|| defined(__MIPSEL) || defined(__MIPSEL__)
#		define CFG_ENDIANNESS CFG_ENDIANNESS_LITTLE
#	else
#		define CFG_ENDIANNESS CFG_ENDIANNESS_UNKNOWN
#	endif
#endif

// NOLINTEND(cppcoreguidelines-macro-usage)
