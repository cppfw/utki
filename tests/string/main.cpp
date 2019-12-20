#include "../../src/utki/string.hpp"
#include "../../src/utki/debug.hpp"

int main(int argc, char** argv){
	{
		auto str = utki::make_string("Hello %d world!", 13);

		ASSERT_INFO_ALWAYS(str.length() == 15, "str.length = " << str.length())
		ASSERT_INFO_ALWAYS(str == "Hello 13 world!", "str = " << str)
	}

}
