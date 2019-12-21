#include "string.hpp"

#include <cstdarg>
#include <array>

#include "debug.hpp"

using namespace utki;

std::string utki::make_string_va_list(const char* format, va_list args){
	std::array<char, 0x400> buf; // first guess is that the resulting string will take less than 1k

	std::string ret;

	auto buf_ptr = &*buf.begin();
	auto buf_size = buf.size();

	va_list cur_args;
	va_copy(cur_args, args);

	for(unsigned i = 0;; ++i){
		int size = vsnprintf(
				buf_ptr,
				buf_size,
				format,
				cur_args
			);

		if(size < 0){
			throw std::logic_error("snprintf() failed");
		}

		if(decltype(buf_size)(size) >= buf_size){
			// resulting string takes more than we have first guessed, allocate enough memory and try again
			ret.resize(size);

			buf_ptr = &*ret.begin();
			buf_size = ret.size() + 1; // NOTE: C++11 guarantees that std::string's internal buffeer has 1 extra byte for null nerminator.

			va_copy(cur_args, args);
		}else{
			ret = std::string(buf_ptr, size);
			break;
		}

		ASSERT(i < 1)
	}
    
    return ret;
}

std::string utki::make_string(const char* format, ...){
	va_list args;
	va_start(args, format);
	auto ret = make_string_va_list(format, args);
	va_end(args);
	return ret;
}
