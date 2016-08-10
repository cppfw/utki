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

#ifdef min
#	undef min
#endif

#ifdef DELETE
#	undef DELETE
#endif