/* The MIT License:

Copyright (c) 2014 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */

// Home page: http://ting.googlecode.com


/**
 * @author Ivan Gagis <igagis@gmail.com>
 * @brief Workaround for including windows.h file.
 * This is a workaround for including a windows.h file to make it in a clean way
 * without breaking anything.
 */

#pragma once

#include "config.hpp"

#if M_OS != M_OS_WINDOWS
#	error "including windows.hpp from non-windows specific code"
#endif

//if _WINSOCKAPI_ macro is not defined then it means that the winsock header file
//has not been included. Here we temporarily define the macro in order to prevent
//inclusion of winsock.h from within the windows.h. Because it may later conflict with
//winsock2.h if it is included later.
#ifndef _WINSOCKAPI_
#	define _WINSOCKAPI_ 1234567890
#endif

#ifndef _WINSOCK_H
#	define _WINSOCK_H 1234567890
#endif

#include <windows.h>


#if (_WINSOCKAPI_ + 0) //if defined and is not empty
#	if _WINSOCKAPI_ == 1234567890
#		undef _WINSOCKAPI_
#	endif
#endif

#if (_WINSOCK_H + 0) //if defined and is not empty
#	if _WINSOCK_H == 1234567890
#		undef _WINSOCK_H
#	endif
#endif

#ifdef max
#	undef max
#endif