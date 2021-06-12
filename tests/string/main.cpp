#include "../../src/utki/string.hpp"
#include "../../src/utki/debug.hpp"

int main(int argc, char** argv){
	// test string without format specifiers
	{
		auto str = utki::make_string(" ");

		utki::assert(str.length() == 1, [&](auto&o){o << "str.length = " << str.length();}, SL);
		utki::assert(str == " ", [&](auto&o){o << "str = " << str;}, SL);
	}

	// test short string
	{
		auto str = utki::make_string("Hello %d world!", 13);

		utki::assert(str.length() == 15, [&](auto&o){o << "str.length = " << str.length();}, SL);
		utki::assert(str == "Hello 13 world!", [&](auto&o){o << "str = " << str;}, SL);
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

		utki::assert(str.length() == 1024 + 15, [&](auto&o){o << "str.length = " << str.length();}, SL);
		utki::assert(
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
				[&](auto&o){o << "str = " << str;},
				SL
			);
	}

	// test string split
	{
		auto str = "qwe/rtyu/io/p//[]";

		auto r = utki::split(str, '/');

		utki::assert(r.size() == 6, [&](auto&o){o << "r.size() = " << r.size();}, SL);
		utki::assert(r[0] == "qwe", SL);
		utki::assert(r[1] == "rtyu", SL);
		utki::assert(r[2] == "io", SL);
		utki::assert(r[3] == "p", SL);
		utki::assert(r[4] == "", SL);
		utki::assert(r[5] == "[]", SL);
	}

	// test split with trailing delimiter character
	{
		auto str = "qwe/rtyu/io/p//[]/";

		auto r = utki::split(str, '/');

		utki::assert(r.size() == 7, [&](auto&o){o << "r.size() = " << r.size();}, SL);
		utki::assert(r[0] == "qwe", SL);
		utki::assert(r[1] == "rtyu", SL);
		utki::assert(r[2] == "io", SL);
		utki::assert(r[3] == "p", SL);
		utki::assert(r[4] == "", SL);
		utki::assert(r[5] == "[]", SL);
		utki::assert(r[6] == "", SL);
	}

	// test empty string split
	{
		auto r = utki::split("", '/');

		utki::assert(r.size() == 1, [&](auto&o){o << "r.size() = " << r.size();}, SL);
	}

	// test string split into words
	{
		auto str = " hello world    bla\tblah\n!";

		auto r = utki::split(str);
		utki::assert(r.size() == 5, [&](auto&o){o << "r.size() = " << r.size();}, SL);
		utki::assert(r[0] == "hello", SL);
		utki::assert(r[1] == "world", SL);
		utki::assert(r[2] == "bla", SL);
		utki::assert(r[3] == "blah", SL);
		utki::assert(r[4] == "!", SL);
	}

	// test basic word wrap
	{
		auto res = utki::word_wrap("abcd efgh i jkl mnop", 7);

		utki::assert(res.size() == 4, [&](auto&o){o << "res.size() = " << res.size();}, SL);
		utki::assert(res[0] == "abcd", [&](auto&o){o << "res[0] = '" << res[0] << "'";}, SL);
		utki::assert(res[1] == "efgh i", [&](auto&o){o << "res[1] = '" << res[1] << "'";}, SL);
		utki::assert(res[2] == "jkl", [&](auto&o){o << "res[2] = '" << res[2] << "'";}, SL);
		utki::assert(res[3] == "mnop", [&](auto&o){o << "res[3] = '" << res[3] << "'";}, SL);
	}

	// test word wrap with width 0
	{
		auto str = "abcd efgh i jkl mnop";
		auto res = utki::word_wrap(str, 0);

		utki::assert(res.size() == 1, [&](auto&o){o << "res.size() = " << res.size();}, SL);
		utki::assert(res[0] == str, [&](auto&o){o << "res[0] = '" << res[0] << "'";}, SL);
	}

	// test word wrap with width 1
	{
		auto str = "ab cd e ";
		auto res = utki::word_wrap(str, 1);

		utki::assert(res.size() == 5, [&](auto&o){o << "res.size() = " << res.size();}, SL);
		utki::assert(res[0] == "a", [&](auto&o){o << "res[0] = '" << res[0] << "'";}, SL);
		utki::assert(res[1] == "b", [&](auto&o){o << "res[1] = '" << res[1] << "'";}, SL);
		utki::assert(res[2] == "c", [&](auto&o){o << "res[2] = '" << res[2] << "'";}, SL);
		utki::assert(res[3] == "d", [&](auto&o){o << "res[3] = '" << res[3] << "'";}, SL);
		utki::assert(res[4] == "e", [&](auto&o){o << "res[4] = '" << res[4] << "'";}, SL);
	}

	// test long word wrap
	{
		auto str = "ab cdefghijklmno p";
		auto res = utki::word_wrap(str, 5);

		utki::assert(res.size() == 4, [&](auto&o){o << "res.size() = " << res.size();}, SL);
		utki::assert(res[0] == "ab", [&](auto&o){o << "res[0] = '" << res[0] << "'";}, SL);
		utki::assert(res[1] == "cdefg", [&](auto&o){o << "res[1] = '" << res[1] << "'";}, SL);
		utki::assert(res[2] == "hijkl", [&](auto&o){o << "res[2] = '" << res[2] << "'";}, SL);
		utki::assert(res[3] == "mno p", [&](auto&o){o << "res[3] = '" << res[3] << "'";}, SL);
	}

	// test word wrap with newlines
	{
		auto str = "ab cd\nef\n g\n  h\n  ijk   \nlm   no p \n\nq\n";
		auto res = utki::word_wrap(str, 5);

		utki::assert(res.size() == 12, [&](auto&o){o << "res.size() = " << res.size();}, SL);
		utki::assert(res[0] == "ab cd", [&](auto&o){o << "res[0] = '" << res[0] << "'";}, SL);
		utki::assert(res[1] == "ef", [&](auto&o){o << "res[1] = '" << res[1] << "'";}, SL);
		utki::assert(res[2] == " g", [&](auto&o){o << "res[2] = '" << res[2] << "'";}, SL);
		utki::assert(res[3] == "  h", [&](auto&o){o << "res[3] = '" << res[3] << "'";}, SL);
		utki::assert(res[4] == "  ijk", [&](auto&o){o << "res[4] = '" << res[4] << "'";}, SL);
		utki::assert(res[5] == "   ", [&](auto&o){o << "res[5] = '" << res[5] << "'";}, SL);
		utki::assert(res[6] == "lm", [&](auto&o){o << "res[6] = '" << res[6] << "'";}, SL);
		utki::assert(res[7] == "  no", [&](auto&o){o << "res[7] = '" << res[7] << "'";}, SL);
		utki::assert(res[8] == "p ", [&](auto&o){o << "res[8] = '" << res[8] << "'";}, SL);
		utki::assert(res[9] == "", [&](auto&o){o << "res[9] = '" << res[9] << "'";}, SL);
		utki::assert(res[10] == "q", [&](auto&o){o << "res[10] = '" << res[10] << "'";}, SL);
		utki::assert(res[11] == "", [&](auto&o){o << "res[11] = '" << res[11] << "'";}, SL);
	}

	// test make_string(span<char>)
	{
		std::array<char, 12> arr = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};
		std::vector<char> vec = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

		auto str_arr = utki::make_string(utki::make_span(arr));
		utki::assert(str_arr == "Hello world!", SL);

		auto str_vec = utki::make_string(utki::make_span(vec));
		utki::assert(str_vec == "Hello world!", SL);
	}

	// test make_string_view(span<char>)
	{
		std::array<char, 12> arr = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};
		std::vector<char> vec = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

		{ // explicit conversion to span
			auto str_arr = utki::make_string_view(utki::make_span(arr));
			utki::assert(str_arr == "Hello world!", SL);

			auto str_vec = utki::make_string_view(utki::make_span(vec));
			utki::assert(str_vec == "Hello world!", SL);
		}
		{ // automatic conversion to span
			auto str_arr = utki::make_string_view(arr);
			utki::assert(str_arr == "Hello world!", SL);

			auto str_vec = utki::make_string_view(vec);
			utki::assert(str_vec == "Hello world!", SL);
		}
	}

	// test make_string(std::array<char>)
	{
		std::array<char, 12> arr = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

		auto str_arr = utki::make_string(arr);
		utki::assert(str_arr == "Hello world!", SL);
	}

	// test make_string(std::array<uint8_t>)
	{
		std::array<uint8_t, 12> arr = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

		auto str_arr = utki::make_string(arr);
		utki::assert(str_arr == "Hello world!", SL);
	}

	// test make_string(std::vector<char>)
	{
		std::vector<char> vec = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

		auto str_vec = utki::make_string(vec);
		utki::assert(str_vec == "Hello world!", SL);
	}

	// test make_string(std::vector<uint8_t>)
	{
		std::vector<uint8_t> vec = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

		auto str_vec = utki::make_string(vec);
		utki::assert(str_vec == "Hello world!", SL);
	}

	// test from_chars(float)
	{
		using namespace std::string_view_literals;

		auto str = "3.14159  "sv;

		float v;

		auto r = utki::from_chars(str.data(), str.data() + str.size(), v);

		utki::assert(r.ptr != str.data(), SL);
		utki::assert(r.ec == std::errc(), SL);
	}

	// test from_chars(double)
	{
		using namespace std::string_view_literals;

		auto str = "3.14159  "sv;

		double v;

		auto r = utki::from_chars(str.data(), str.data() + str.size(), v);

		utki::assert(r.ptr != str.data(), SL);
		utki::assert(r.ec == std::errc(), SL);
	}
}
