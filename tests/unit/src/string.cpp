#include <tst/set.hpp>
#include <tst/check.hpp>

#include <utki/string.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace{
tst::set set("string", [](tst::suite& suite){
	suite.add(
		"make_string_from_const_char_ptr",
		[](){
			auto str = utki::make_string(" ");

			tst::check_eq(str.size(), size_t(1), [&](auto&o){o << "str.size = " << str.size();}, SL);
			tst::check_eq(str, " "s, [&](auto&o){o << "str = " << str;}, SL);
		}
	);

	suite.add(
		"make_string_short_formatted",
		[](){
			auto str = utki::make_string("Hello %d world!", 13);

			tst::check_eq(str.size(), size_t(15), [&](auto&o){o << "str.size = " << str.size();}, SL);
			tst::check_eq(str, "Hello 13 world!"s, [&](auto&o){o << "str = " << str;}, SL);
		}
	);

	suite.add(
		"make_string_long_formatted_over_1k",
		[](){
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

			tst::check_eq(str.size(), size_t(1024 + 15), [&](auto&o){o << "str.size = " << str.size();}, SL);
			tst::check_eq(
					str,
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

					"Hello 13 world!"s,
					[&](auto&o){o << "str = " << str;},
					SL
				);
		}
	);

	suite.add(
		"split",
		[](){
			auto str = "qwe/rtyu/io/p//[]";

			auto r = utki::split(str, '/');

			tst::check_eq(r.size(), size_t(6), [&](auto&o){o << "r.size() = " << r.size();}, SL);
			tst::check_eq(r[0], "qwe"s, SL);
			tst::check_eq(r[1], "rtyu"s, SL);
			tst::check_eq(r[2], "io"s, SL);
			tst::check_eq(r[3], "p"s, SL);
			tst::check_eq(r[4], ""s, SL);
			tst::check_eq(r[5], "[]"s, SL);
		}
	);

	suite.add(
		"split_with_trailing_delimiter_character",
		[](){
			auto str = "qwe/rtyu/io/p//[]/";

			auto r = utki::split(str, '/');

			tst::check_eq(r.size(), size_t(7), [&](auto&o){o << "r.size() = " << r.size();}, SL);
			tst::check_eq(r[0], "qwe"s, SL);
			tst::check_eq(r[1], "rtyu"s, SL);
			tst::check_eq(r[2], "io"s, SL);
			tst::check_eq(r[3], "p"s, SL);
			tst::check_eq(r[4], ""s, SL);
			tst::check_eq(r[5], "[]"s, SL);
			tst::check_eq(r[6], ""s, SL);
		}
	);

	suite.add(
		"split_empty_string",
		[](){
			auto r = utki::split("", '/');

			tst::check_eq(r.size(), size_t(1), [&](auto&o){o << "r.size() = " << r.size();}, SL);
		}
	);

	suite.add(
		"split_into_words",
		[](){
			auto str = " hello world    bla\tblah\n!";

			auto r = utki::split(str);
			tst::check_eq(r.size(), size_t(5), [&](auto&o){o << "r.size() = " << r.size();}, SL);
			tst::check_eq(r[0], "hello"s, SL);
			tst::check_eq(r[1], "world"s, SL);
			tst::check_eq(r[2], "bla"s, SL);
			tst::check_eq(r[3], "blah"s, SL);
			tst::check_eq(r[4], "!"s, SL);
		}
	);

	suite.add(
		"word_wrap_basic",
		[](){
			auto res = utki::word_wrap("abcd efgh i jkl mnop", 7);

			tst::check_eq(res.size(), size_t(4), [&](auto&o){o << "res.size() = " << res.size();}, SL);
			tst::check_eq(res[0], "abcd"s, [&](auto&o){o << "res[0] = '" << res[0] << "'";}, SL);
			tst::check_eq(res[1], "efgh i"s, [&](auto&o){o << "res[1] = '" << res[1] << "'";}, SL);
			tst::check_eq(res[2], "jkl"s, [&](auto&o){o << "res[2] = '" << res[2] << "'";}, SL);
			tst::check_eq(res[3], "mnop"s, [&](auto&o){o << "res[3] = '" << res[3] << "'";}, SL);
		}
	);

	suite.add(
		"word_wrap_width_of_0",
		[](){
			auto str = "abcd efgh i jkl mnop";
			auto res = utki::word_wrap(str, 0);

			tst::check_eq(res.size(), size_t(1), [&](auto&o){o << "res.size() = " << res.size();}, SL);
			tst::check_eq(res[0], std::string(str), [&](auto&o){o << "res[0] = '" << res[0] << "'";}, SL);
		}
	);

	suite.add(
		"word_wrap_width_of_1",
		[](){
			auto str = "ab cd e ";
			auto res = utki::word_wrap(str, 1);

			tst::check_eq(res.size(), size_t(5), [&](auto&o){o << "res.size() = " << res.size();}, SL);
			tst::check_eq(res[0], "a"s, [&](auto&o){o << "res[0] = '" << res[0] << "'";}, SL);
			tst::check_eq(res[1], "b"s, [&](auto&o){o << "res[1] = '" << res[1] << "'";}, SL);
			tst::check_eq(res[2], "c"s, [&](auto&o){o << "res[2] = '" << res[2] << "'";}, SL);
			tst::check_eq(res[3], "d"s, [&](auto&o){o << "res[3] = '" << res[3] << "'";}, SL);
			tst::check_eq(res[4], "e"s, [&](auto&o){o << "res[4] = '" << res[4] << "'";}, SL);
		}
	);

	suite.add(
		"word_wrap_long_word",
		[](){
			auto str = "ab cdefghijklmno p";
			auto res = utki::word_wrap(str, 5);

			tst::check_eq(res.size(), size_t(4), [&](auto&o){o << "res.size() = " << res.size();}, SL);
			tst::check_eq(res[0], "ab"s, [&](auto&o){o << "res[0] = '" << res[0] << "'";}, SL);
			tst::check_eq(res[1], "cdefg"s, [&](auto&o){o << "res[1] = '" << res[1] << "'";}, SL);
			tst::check_eq(res[2], "hijkl"s, [&](auto&o){o << "res[2] = '" << res[2] << "'";}, SL);
			tst::check_eq(res[3], "mno p"s, [&](auto&o){o << "res[3] = '" << res[3] << "'";}, SL);
		}
	);

	suite.add(
		"word_wrap_with_new_lines",
		[](){
			auto str = "ab cd\nef\n g\n  h\n  ijk   \nlm   no p \n\nq\n";
			auto res = utki::word_wrap(str, 5);

			tst::check_eq(res.size(), size_t(12), [&](auto&o){o << "res.size() = " << res.size();}, SL);
			tst::check_eq(res[0], "ab cd"s, [&](auto&o){o << "res[0] = '" << res[0] << "'";}, SL);
			tst::check_eq(res[1], "ef"s, [&](auto&o){o << "res[1] = '" << res[1] << "'";}, SL);
			tst::check_eq(res[2], " g"s, [&](auto&o){o << "res[2] = '" << res[2] << "'";}, SL);
			tst::check_eq(res[3], "  h"s, [&](auto&o){o << "res[3] = '" << res[3] << "'";}, SL);
			tst::check_eq(res[4], "  ijk"s, [&](auto&o){o << "res[4] = '" << res[4] << "'";}, SL);
			tst::check_eq(res[5], "   "s, [&](auto&o){o << "res[5] = '" << res[5] << "'";}, SL);
			tst::check_eq(res[6], "lm"s, [&](auto&o){o << "res[6] = '" << res[6] << "'";}, SL);
			tst::check_eq(res[7], "  no"s, [&](auto&o){o << "res[7] = '" << res[7] << "'";}, SL);
			tst::check_eq(res[8], "p "s, [&](auto&o){o << "res[8] = '" << res[8] << "'";}, SL);
			tst::check_eq(res[9], ""s, [&](auto&o){o << "res[9] = '" << res[9] << "'";}, SL);
			tst::check_eq(res[10], "q"s, [&](auto&o){o << "res[10] = '" << res[10] << "'";}, SL);
			tst::check_eq(res[11], ""s, [&](auto&o){o << "res[11] = '" << res[11] << "'";}, SL);
		}
	);

	suite.add(
		"make_string_from_span_char",
		[](){
			std::array<char, 12> arr = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};
			std::vector<char> vec = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

			auto str_arr = utki::make_string(utki::make_span(arr));
			tst::check_eq(str_arr, "Hello world!"s, SL);

			auto str_vec = utki::make_string(utki::make_span(vec));
			tst::check_eq(str_vec, "Hello world!"s, SL);
		}
	);

	suite.add(
		"make_string_view_from_span_char_explicit_conversion",
		[](){
			std::array<char, 12> arr = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};
			std::vector<char> vec = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

			auto str_arr = utki::make_string_view(utki::make_span(arr));
			tst::check_eq(str_arr, "Hello world!"sv, SL);

			auto str_vec = utki::make_string_view(utki::make_span(vec));
			tst::check_eq(str_vec, "Hello world!"sv, SL);
		}
	);

	suite.add(
		"make_string_view_from_span_char_implicit_conversion",
		[](){
			std::array<char, 12> arr = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};
			std::vector<char> vec = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

			auto str_arr = utki::make_string_view(arr);
			tst::check_eq(str_arr, "Hello world!"sv, SL);

			auto str_vec = utki::make_string_view(vec);
			tst::check_eq(str_vec, "Hello world!"sv, SL);
		}
	);

	suite.add(
		"make_string_from_array_char",
		[](){
			std::array<char, 12> arr = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

			auto str_arr = utki::make_string(arr);
			tst::check_eq(str_arr, "Hello world!"s, SL);
		}
	);

	suite.add(
		"make_string_from_array_uint8_t",
		[](){
			std::array<uint8_t, 12> arr = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

			auto str_arr = utki::make_string(arr);
			tst::check_eq(str_arr, "Hello world!"s, SL);
		}
	);

	suite.add(
		"make_string_from_vector_char",
		[](){
			std::vector<char> vec = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

			auto str_vec = utki::make_string(vec);
			tst::check_eq(str_vec, "Hello world!"s, SL);
		}
	);

	suite.add(
		"make_string_from_vector_uint8_t",
		[](){
			std::vector<uint8_t> vec = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

			auto str_vec = utki::make_string(vec);
			tst::check_eq(str_vec, "Hello world!"s, SL);
		}
	);

	suite.add(
		"from_chars_float",
		[](){
			std::string_view str = "3.14159  ";

			float v;

			auto r = utki::from_chars(str.data(), str.data() + str.size(), v);

			tst::check_ne(r.ptr, str.data(), SL);
			tst::check(r.ec == std::errc(), SL);
			tst::check_eq(v, 3.14159f, SL);
		}
	);

	suite.add(
		"from_chars_double",
		[](){
			std::string_view str = "3.14159  ";

			double v;

			auto r = utki::from_chars(str.data(), str.data() + str.size(), v);

			tst::check_ne(r.ptr, str.data(), SL);
			tst::check(r.ec == std::errc(), SL);
			tst::check_eq(v, 3.14159, SL);
		}
	);

	suite.add(
		"string_parser",
		[](){
			// make sure the locale does not affect parsing (decimal delimiter can be "." or "," in different locales)
			// so, set DE locale which has "," to make sure it does not affect the parsing
			if(!std::setlocale(LC_ALL, "de_DE.UTF-8")){
				utki::log([](auto& o){o << "WARNING: failed to set locale de_DE.UTF-8, perhaps the locale is not installed. Testing that locale does not affect parsing will not be done." << std::endl;});
			}
			
			std::string_view str = "  3.14159, some_word, bla , bla e=2.7 and that's it, 13, -13, 0b1101, 015, 0xd";

			utki::string_parser p(str);

			tst::check(!p.empty(), SL);

			auto pi = p.read_number<float>();
			tst::check_eq(pi, 3.14159f, SL);
			tst::check(!p.empty(), SL);
			tst::check_eq(p.peek_char(), ',', SL);

			p.skip_whitespaces_and_comma();
			tst::check(!p.empty(), SL);
			tst::check_eq(p.peek_char(), 's', SL);

			{
				auto w = p.read_word_until(',');
				tst::check(!p.empty(), SL);
				tst::check_eq(p.peek_char(), ',', SL);
				tst::check_eq(w, "some_word"sv, SL);
			}

			tst::check_eq(p.read_char(), ',', SL);
			tst::check(!p.empty(), SL);

			{
				p.skip_whitespaces();
				auto w = p.read_word();
				tst::check(!p.empty(), SL);
				tst::check_eq(p.peek_char(), ' ', SL);
				tst::check_eq(w, "bla"sv, SL);
			}

			p.skip_whitespaces_and_comma();
			tst::check(!p.empty(), SL);
			tst::check_eq(p.peek_char(), 'b', SL);

			p.skip_inclusive_until('=');
			tst::check(!p.empty(), SL);
			tst::check_eq(p.peek_char(), '2', SL);

			auto e = p.read_number<double>();
			tst::check_eq(e, 2.7, SL);
			tst::check(!p.empty(), SL);
			tst::check_eq(p.peek_char(), ' ', SL);

			p.skip_whitespaces();
			{
				auto w = p.read_word();
				tst::check(!p.empty(), SL);
				tst::check_eq(p.peek_char(), ' ', SL);
				tst::check_eq(w, "and"sv, SL);
			}

			p.skip_whitespaces();
			{
				auto w = p.read_word();
				tst::check(!p.empty(), SL);
				tst::check_eq(p.peek_char(), ' ', SL);
				tst::check_eq(w, "that's"sv, SL);
			}

			p.skip_whitespaces();
			{
				auto w = p.read_word();
				tst::check(!p.empty(), SL);
				tst::check_eq(w, "it,"sv, SL);
			}

			p.skip_whitespaces();
			{
				auto n = p.read_number<int>();
				tst::check(!p.empty(), SL);
				tst::check_eq(n, 13, SL);
				tst::check_eq(p.peek_char(), ',', SL);
			}

			p.skip_whitespaces_and_comma();
			{
				auto n = p.read_number<int>();
				tst::check(!p.empty(), SL);
				tst::check_eq(n, -13, SL);
				tst::check_eq(p.peek_char(), ',', SL);
			}

			p.skip_whitespaces_and_comma();
			{
				auto n = p.read_number<unsigned>();
				tst::check(!p.empty(), SL);
				tst::check_eq(n, unsigned(13), [&](auto&o){o << "n = " << n;}, SL);
				tst::check_eq(p.peek_char(), ',', SL);
			}

			p.skip_whitespaces_and_comma();
			{
				auto n = p.read_number<unsigned>();
				tst::check(!p.empty(), SL);
				tst::check_eq(n, unsigned(13), [&](auto&o){o << "n = " << n;}, SL);
				tst::check_eq(p.peek_char(), ',', SL);
			}

			p.skip_whitespaces_and_comma();
			{
				auto n = p.read_number<unsigned>();
				tst::check(p.empty(), SL);
				tst::check_eq(n, unsigned(13), [&](auto&o){o << "n = " << n;}, SL);
			}
		}
	);
});
}
