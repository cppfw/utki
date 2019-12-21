#pragma once

#include <string>

#include "config.hpp"

namespace utki{

#if M_COMPILER != M_COMPILER_MSVC
__attribute__((format(printf, 1, 2)))
#endif
std::string make_string(const char* format, ...);

std::string make_string_va_list(const char* format, va_list args);

}
