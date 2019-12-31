#include "../../src/utki/string.hpp"
#include "../../src/utki/debug.hpp"

int main(int argc, char** argv){
	// test string without format specifiers
	{
		auto str = utki::make_string(" ");

		ASSERT_INFO_ALWAYS(str.length() == 1, "str.length = " << str.length())
		ASSERT_INFO_ALWAYS(str == " ", "str = " << str)
	}

	// test short string
	{
		auto str = utki::make_string("Hello %d world!", 13);

		ASSERT_INFO_ALWAYS(str.length() == 15, "str.length = " << str.length())
		ASSERT_INFO_ALWAYS(str == "Hello 13 world!", "str = " << str)
	}

	// test long string over 1k
	{
		auto str = utki::make_string(
				"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
				"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
				"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
				"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef" 

				"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
				"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
				"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
				"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef" 

				"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
				"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
				"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
				"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef" 

				"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
				"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
				"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
				"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef" 

				"Hello %d world!", 13
			);

		ASSERT_INFO_ALWAYS(str.length() == 1024 + 15, "str.length = " << str.length())
		ASSERT_INFO_ALWAYS(
				str == 
						"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
						"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
						"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
						"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef" 

						"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
						"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
						"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
						"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef" 

						"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
						"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
						"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
						"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef" 

						"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
						"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
						"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef"
						"1234567890abcdef" "1234567890abcdef" "1234567890abcdef" "1234567890abcdef" 

						"Hello 13 world!",
				"str = " << str
			)
	}

	// test string split
	{
		auto str = "qwe/rtyu/io/p//[]";

		auto r = utki::split(str, '/');

		ASSERT_INFO_ALWAYS(r.size() == 6, "r.size() = " << r.size())
		ASSERT_ALWAYS(r[0] == "qwe")
		ASSERT_ALWAYS(r[1] == "rtyu")
		ASSERT_ALWAYS(r[2] == "io")
		ASSERT_ALWAYS(r[3] == "p")
		ASSERT_ALWAYS(r[4] == "")
		ASSERT_ALWAYS(r[5] == "[]")
	}

	// test emnpty string split
	{
		auto r = utki::split("", '/');

		ASSERT_INFO_ALWAYS(r.size() == 1, "r.size() = " << r.size())
	}
}
