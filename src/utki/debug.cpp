#include "debug.hpp"

#include <unistd.h>
#include <cstdio>

#include "config.hpp"

#if M_OS_NAME == M_OS_NAME_ANDROID
#	include <android/log.h>

#else
#	include <iostream>

#endif

using namespace utki;

namespace{
const std::string colored_error_string = "\033[1;31merror\033[0m";
const std::string uncolored_error_string = "error";
}

void utki::assert(
		bool condition,
		const std::function<void(std::ostream&)>& print,
		std::pair<const char*, size_t> source_location
	)
{
	if(condition){
		return;
	}

	std::stringstream ss;
	ss << source_location.first << ":" << source_location.second << ": ";

	if(isatty(fileno(stderr))){ // if we have terminal
		ss << colored_error_string;
	}else{
		ss << uncolored_error_string;
	}
	 
	ss << ": assertion failed";

	if(print){
		ss << ":" << std::endl << "  ";
		print(ss);
	}

#if M_OS_NAME == M_OS_NAME_ANDROID
	// TODO: remove commented code after __android_log_assert() is tested to be working
	// __android_log_write(ANDROID_LOG_INFO, "utki", ss.str().c_str());

	__android_log_assert(ss.str().c_str(), "utki", nullptr);
#else
	std::cerr << ss.str() << std::endl;
	std::cerr.flush();
#endif

	abort();
}

void utki::log(const std::function<void(std::ostream&)>& print){
	if(!print){
		return;
	}

#if M_OS_NAME == M_OS_NAME_ANDROID
	std::stringstream ss;
	print(ss);
	__android_log_write(ANDROID_LOG_INFO, "utki", ss.str().c_str());
#else
	print(std::cout);
	std::cout.flush();
#endif
}
