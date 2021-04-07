#include "util.hpp"

#if M_OS == M_OS_WINDOWS
#	include <io.h>
#else
#	include <unistd.h>
#endif

using namespace utki;

bool utki::is_cerr_terminal(){
#if M_OS == M_OS_WINDOWS
	// need to compare result with 0 to avoid MSVC compiler warning
	return _isatty(_fileno(stderr)) != 0;
#else
	return isatty(fileno(stderr));
#endif
}

bool utki::is_cout_terminal(){
#if M_OS == M_OS_WINDOWS
	// need to compare result with 0 to avoid MSVC compiler warning
	return _isatty(_fileno(stdout)) != 0;
#else
	return isatty(fileno(stdout));
#endif
}
