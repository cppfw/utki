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

	// test empty string split
	{
		auto r = utki::split("", '/');

		ASSERT_INFO_ALWAYS(r.size() == 1, "r.size() = " << r.size())
	}

	// test basic word wrap
	{
		auto res = utki::word_wrap("abcd efgh i jkl mnop", 7);

		ASSERT_INFO_ALWAYS(res.size() == 4, "res.size() = " << res.size())
		ASSERT_INFO_ALWAYS(res[0] == "abcd", "res[0] = '" << res[0] << "'")
		ASSERT_INFO_ALWAYS(res[1] == "efgh i", "res[1] = '" << res[1] << "'")
		ASSERT_INFO_ALWAYS(res[2] == "jkl", "res[2] = '" << res[2] << "'")
		ASSERT_INFO_ALWAYS(res[3] == "mnop", "res[3] = '" << res[3] << "'")
	}

	// test word wrap with width 0
	{
		auto str = "abcd efgh i jkl mnop";
		auto res = utki::word_wrap(str, 0);

		ASSERT_INFO_ALWAYS(res.size() == 1, "res.size() = " << res.size())
		ASSERT_INFO_ALWAYS(res[0] == str, "res[0] = '" << res[0] << "'")
	}

	// test word wrap with width 1
	{
		auto str = "ab cd e ";
		auto res = utki::word_wrap(str, 1);

		ASSERT_INFO_ALWAYS(res.size() == 5, "res.size() = " << res.size())
		ASSERT_INFO_ALWAYS(res[0] == "a", "res[0] = '" << res[0] << "'")
		ASSERT_INFO_ALWAYS(res[1] == "b", "res[1] = '" << res[1] << "'")
		ASSERT_INFO_ALWAYS(res[2] == "c", "res[2] = '" << res[2] << "'")
		ASSERT_INFO_ALWAYS(res[3] == "d", "res[3] = '" << res[3] << "'")
		ASSERT_INFO_ALWAYS(res[4] == "e", "res[4] = '" << res[4] << "'")
	}

	// test long word wrap
	{
		auto str = "ab cdefghijklmno p";
		auto res = utki::word_wrap(str, 5);

		ASSERT_INFO_ALWAYS(res.size() == 4, "res.size() = " << res.size())
		ASSERT_INFO_ALWAYS(res[0] == "ab", "res[0] = '" << res[0] << "'")
		ASSERT_INFO_ALWAYS(res[1] == "cdefg", "res[1] = '" << res[1] << "'")
		ASSERT_INFO_ALWAYS(res[2] == "hijkl", "res[2] = '" << res[2] << "'")
		ASSERT_INFO_ALWAYS(res[3] == "mno p", "res[3] = '" << res[3] << "'")
	}

	// test word wrap with newlines
	{
		auto str = "ab cd\nef\n g\n  h\n  ijk   \nlm   no p \n\nq\n";
		auto res = utki::word_wrap(str, 5);

		ASSERT_INFO_ALWAYS(res.size() == 12, "res.size() = " << res.size())
		ASSERT_INFO_ALWAYS(res[0] == "ab cd", "res[0] = '" << res[0] << "'")
		ASSERT_INFO_ALWAYS(res[1] == "ef", "res[1] = '" << res[1] << "'")
		ASSERT_INFO_ALWAYS(res[2] == " g", "res[2] = '" << res[2] << "'")
		ASSERT_INFO_ALWAYS(res[3] == "  h", "res[3] = '" << res[3] << "'")
		ASSERT_INFO_ALWAYS(res[4] == "  ijk", "res[4] = '" << res[4] << "'")
		ASSERT_INFO_ALWAYS(res[5] == "   ", "res[5] = '" << res[5] << "'")
		ASSERT_INFO_ALWAYS(res[6] == "lm", "res[6] = '" << res[6] << "'")
		ASSERT_INFO_ALWAYS(res[7] == "  no", "res[7] = '" << res[7] << "'")
		ASSERT_INFO_ALWAYS(res[8] == "p ", "res[8] = '" << res[8] << "'")
		ASSERT_INFO_ALWAYS(res[9] == "", "res[9] = '" << res[9] << "'")
		ASSERT_INFO_ALWAYS(res[10] == "q", "res[10] = '" << res[10] << "'")
		ASSERT_INFO_ALWAYS(res[11] == "", "res[11] = '" << res[11] << "'")
	}

	// test make_string(span<char>)
	{
		std::array<char, 12> arr = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};
		std::vector<char> vec = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

		auto str_arr = utki::make_string(utki::make_span(arr));
		ASSERT_ALWAYS(str_arr == "Hello world!")

		auto str_vec = utki::make_string(utki::make_span(vec));
		ASSERT_ALWAYS(str_vec == "Hello world!")
	}

	// test make_string(std::array<char>)
	{
		std::array<char, 12> arr = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

		auto str_arr = utki::make_string(arr);
		ASSERT_ALWAYS(str_arr == "Hello world!")
	}

	// test make_string(std::array<uint8_t>)
	{
		std::array<uint8_t, 12> arr = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

		auto str_arr = utki::make_string(arr);
		ASSERT_ALWAYS(str_arr == "Hello world!")
	}

	// test make_string(std::vector<char>)
	{
		std::vector<char> vec = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

		auto str_vec = utki::make_string(vec);
		ASSERT_ALWAYS(str_vec == "Hello world!")
	}

	// test make_string(std::vector<uint8_t>)
	{
		std::vector<uint8_t> vec = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

		auto str_vec = utki::make_string(vec);
		ASSERT_ALWAYS(str_vec == "Hello world!")
	}
}
