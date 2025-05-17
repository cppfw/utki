#include <utki/string.hpp>

int main(int argc, const char** argv){
    auto s = utki::split("Hello world!");

    std::cout << "num strings = " << s.size() << std::endl;

    return 0;
}
