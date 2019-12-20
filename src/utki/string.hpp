#pragma once

#include <string>

namespace utki{


__attribute__((format(printf, 1, 2))) std::string make_string(const char* format, ...);


}
