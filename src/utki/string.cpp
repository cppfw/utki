#include "string.hpp"

#include <cstdarg>

using namespace utki;


std::string utki::make_string(const char* format, ...){
	va_list args1;
	va_start(args1, format);
    int size = vsnprintf(nullptr, 0, format, args1);
	va_end(args1);

    std::string ret;
    ret.resize(size);

	va_list args2;
	va_start(args2, format);
    vsnprintf(
			&*ret.begin(),
			ret.size() + 1, // NOTE: since C++11 it is guaranteed that std::string buffer has 1 extra character for terminating zero.
			format,
			args2
		);
	va_end(args2);
	
    return ret;
}
