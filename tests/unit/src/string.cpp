#include <utki/config.hpp>

#if CFG_COMPILER != CFG_COMPILER_MSVC || CFG_COMPILER_MSVC_TOOLS_V >= 142

#	include <clocale>
#	include <tst/check.hpp>
#	include <tst/set.hpp>
#	include <utki/string.hpp>

#	include <fast_float/fast_float.h>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace {
const tst::set set("string", [](tst::suite& suite) {
	suite.add("make_string_from_const_char_ptr", []() {
		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
		auto str = utki::make_string(" ");

		tst::check_eq(
			str.size(),
			size_t(1),
			[&](auto& o) {
				o << "str.size = " << str.size();
			},
			SL
		);
		tst::check_eq(
			str,
			" "s,
			[&](auto& o) {
				o << "str = " << str;
			},
			SL
		);
	});

	suite.add("make_string_short_formatted", []() {
		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
		auto str = utki::make_string("Hello %d world!", 13);

		tst::check_eq(
			str.size(),
			size_t(15),
			[&](auto& o) {
				o << "str.size = " << str.size();
			},
			SL
		);
		tst::check_eq(
			str,
			"Hello 13 world!"s,
			[&](auto& o) {
				o << "str = " << str;
			},
			SL
		);
	});

	suite.add("make_string_long_formatted_over_1k", []() {
		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
		auto str = utki::make_string(
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"

			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"

			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"

			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"

			"Hello %d world!",
			13
		);

		tst::check_eq(
			str.size(),
			size_t(1024 + 15),
			[&](auto& o) {
				o << "str.size = " << str.size();
			},
			SL
		);
		tst::check_eq(
			str,
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"

			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"

			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"

			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"
			"1234567890abcdef"

			"Hello 13 world!"s,
			[&](auto& o) {
				o << "str = " << str;
			},
			SL
		);
	});

	suite.add("split_const_char_ptr", []() {
		auto str = "qwe/rtyu/io/p//[]";

		auto r = utki::split(str, '/');

		tst::check_eq(
			r.size(),
			size_t(6),
			[&](auto& o) {
				o << "r.size() = " << r.size();
			},
			SL
		);
		tst::check_eq(r[0], "qwe"s, SL);
		tst::check_eq(r[1], "rtyu"s, SL);
		tst::check_eq(r[2], "io"s, SL);
		tst::check_eq(r[3], "p"s, SL);
		tst::check_eq(r[4], ""s, SL);
		tst::check_eq(r[5], "[]"s, SL);
	});

	suite.add("split_string_view", []() {
		auto str = "qwe/rtyu/io/p//[]"sv;

		auto r = utki::split(str, '/');

		tst::check_eq(
			r.size(),
			size_t(6),
			[&](auto& o) {
				o << "r.size() = " << r.size();
			},
			SL
		);
		tst::check_eq(r[0], "qwe"s, SL);
		tst::check_eq(r[1], "rtyu"s, SL);
		tst::check_eq(r[2], "io"s, SL);
		tst::check_eq(r[3], "p"s, SL);
		tst::check_eq(r[4], ""s, SL);
		tst::check_eq(r[5], "[]"s, SL);
	});

	suite.add("split_string", []() {
		auto str = "qwe/rtyu/io/p//[]"s;

		auto r = utki::split(str, '/');

		tst::check_eq(
			r.size(),
			size_t(6),
			[&](auto& o) {
				o << "r.size() = " << r.size();
			},
			SL
		);
		tst::check_eq(r[0], "qwe"s, SL);
		tst::check_eq(r[1], "rtyu"s, SL);
		tst::check_eq(r[2], "io"s, SL);
		tst::check_eq(r[3], "p"s, SL);
		tst::check_eq(r[4], ""s, SL);
		tst::check_eq(r[5], "[]"s, SL);
	});

	suite.add("split_u32string_view", []() {
		auto str = U"qwe/rtyu/io/p//[]"sv;

		auto r = utki::split(str, U'/');

		tst::check_eq(
			r.size(),
			size_t(6),
			[&](auto& o) {
				o << "r.size() = " << r.size();
			},
			SL
		);
		tst::check(r[0] == U"qwe"s, SL);
		tst::check(r[1] == U"rtyu"s, SL);
		tst::check(r[2] == U"io"s, SL);
		tst::check(r[3] == U"p"s, SL);
		tst::check(r[4] == U""s, SL);
		tst::check(r[5] == U"[]"s, SL);
	});

	suite.add("split_u32string", []() {
		auto str = U"qwe/rtyu/io/p//[]"s;

		auto r = utki::split(str, U'/');

		tst::check_eq(
			r.size(),
			size_t(6),
			[&](auto& o) {
				o << "r.size() = " << r.size();
			},
			SL
		);
		tst::check(r[0] == U"qwe"s, SL);
		tst::check(r[1] == U"rtyu"s, SL);
		tst::check(r[2] == U"io"s, SL);
		tst::check(r[3] == U"p"s, SL);
		tst::check(r[4] == U""s, SL);
		tst::check(r[5] == U"[]"s, SL);
	});

	suite.add("split_with_trailing_delimiter_character", []() {
		auto str = "qwe/rtyu/io/p//[]/";

		auto r = utki::split(str, '/');

		tst::check_eq(
			r.size(),
			size_t(7),
			[&](auto& o) {
				o << "r.size() = " << r.size();
			},
			SL
		);
		tst::check_eq(r[0], "qwe"s, SL);
		tst::check_eq(r[1], "rtyu"s, SL);
		tst::check_eq(r[2], "io"s, SL);
		tst::check_eq(r[3], "p"s, SL);
		tst::check_eq(r[4], ""s, SL);
		tst::check_eq(r[5], "[]"s, SL);
		tst::check_eq(r[6], ""s, SL);
	});

	suite.add("split_empty_string", []() {
		auto r = utki::split("", '/');

		tst::check_eq(
			r.size(),
			size_t(1),
			[&](auto& o) {
				o << "r.size() = " << r.size();
			},
			SL
		);
	});

	suite.add("split_into_words", []() {
		auto str = " hello world    bla\tblah\n!";

		auto r = utki::split(str);
		tst::check_eq(
			r.size(),
			size_t(5),
			[&](auto& o) {
				o << "r.size() = " << r.size();
			},
			SL
		);
		tst::check_eq(r[0], "hello"s, SL);
		tst::check_eq(r[1], "world"s, SL);
		tst::check_eq(r[2], "bla"s, SL);
		tst::check_eq(r[3], "blah"s, SL);
		tst::check_eq(r[4], "!"s, SL);
	});

	suite.add("join_vector_of_strings", []() {
		std::vector<std::string> strings = {"hello", "world", "!"};

		tst::check_eq(utki::join(strings, '#'), "hello#world#!"s, SL);
	});

	suite.add("join_vector_of_string_views", []() {
		std::vector<std::string_view> strings = {"hello"sv, "world"sv, "!"sv};

		tst::check_eq(utki::join(strings, '#'), "hello#world#!"s, SL);
	});

	suite.add("join_array_of_strings", []() {
		std::array<std::string, 3> strings = {"hello", "world", "!"};

		tst::check_eq(utki::join(strings, '#'), "hello#world#!"s, SL);
	});

	suite.add("join_array_of_string_views", []() {
		std::array<std::string_view, 3> strings = {"hello"sv, "world"sv, "!"sv};

		tst::check_eq(utki::join(strings, '#'), "hello#world#!"s, SL);
	});

	suite.add("join_span_of_strings", []() {
		std::vector<std::string> strings = {"hello", "world", "!"};

		tst::check_eq(utki::join(utki::make_span(strings), '#'), "hello#world#!"s, SL);
	});

	suite.add("join_span_of_string_views", []() {
		std::vector<std::string_view> strings = {"hello"sv, "world"sv, "!"sv};

		tst::check_eq(utki::join(utki::make_span(strings), '#'), "hello#world#!"s, SL);
	});

	suite.add("word_wrap_basic", []() {
		auto res = utki::word_wrap("abcd efgh i jkl mnop", 7);

		tst::check_eq(
			res.size(),
			size_t(4),
			[&](auto& o) {
				o << "res.size() = " << res.size();
			},
			SL
		);
		tst::check_eq(
			res[0],
			"abcd"s,
			[&](auto& o) {
				o << "res[0] = '" << res[0] << "'";
			},
			SL
		);
		tst::check_eq(
			res[1],
			"efgh i"s,
			[&](auto& o) {
				o << "res[1] = '" << res[1] << "'";
			},
			SL
		);
		tst::check_eq(
			res[2],
			"jkl"s,
			[&](auto& o) {
				o << "res[2] = '" << res[2] << "'";
			},
			SL
		);
		tst::check_eq(
			res[3],
			"mnop"s,
			[&](auto& o) {
				o << "res[3] = '" << res[3] << "'";
			},
			SL
		);
	});

	suite.add("word_wrap_width_of_0", []() {
		auto str = "abcd efgh i jkl mnop";
		auto res = utki::word_wrap(str, 0);

		tst::check_eq(
			res.size(),
			size_t(1),
			[&](auto& o) {
				o << "res.size() = " << res.size();
			},
			SL
		);
		tst::check_eq(
			res[0],
			std::string(str),
			[&](auto& o) {
				o << "res[0] = '" << res[0] << "'";
			},
			SL
		);
	});

	suite.add("word_wrap_width_of_1", []() {
		auto str = "ab cd e ";
		auto res = utki::word_wrap(str, 1);

		tst::check_eq(
			res.size(),
			size_t(5),
			[&](auto& o) {
				o << "res.size() = " << res.size();
			},
			SL
		);
		tst::check_eq(
			res[0],
			"a"s,
			[&](auto& o) {
				o << "res[0] = '" << res[0] << "'";
			},
			SL
		);
		tst::check_eq(
			res[1],
			"b"s,
			[&](auto& o) {
				o << "res[1] = '" << res[1] << "'";
			},
			SL
		);
		tst::check_eq(
			res[2],
			"c"s,
			[&](auto& o) {
				o << "res[2] = '" << res[2] << "'";
			},
			SL
		);
		tst::check_eq(
			res[3],
			"d"s,
			[&](auto& o) {
				o << "res[3] = '" << res[3] << "'";
			},
			SL
		);
		tst::check_eq(
			res[4],
			"e"s,
			[&](auto& o) {
				o << "res[4] = '" << res[4] << "'";
			},
			SL
		);
	});

	suite.add("word_wrap_long_word", []() {
		auto str = "ab cdefghijklmno p";
		auto res = utki::word_wrap(str, 5);

		tst::check_eq(
			res.size(),
			size_t(4),
			[&](auto& o) {
				o << "res.size() = " << res.size();
			},
			SL
		);
		tst::check_eq(
			res[0],
			"ab"s,
			[&](auto& o) {
				o << "res[0] = '" << res[0] << "'";
			},
			SL
		);
		tst::check_eq(
			res[1],
			"cdefg"s,
			[&](auto& o) {
				o << "res[1] = '" << res[1] << "'";
			},
			SL
		);
		tst::check_eq(
			res[2],
			"hijkl"s,
			[&](auto& o) {
				o << "res[2] = '" << res[2] << "'";
			},
			SL
		);
		tst::check_eq(
			res[3],
			"mno p"s,
			[&](auto& o) {
				o << "res[3] = '" << res[3] << "'";
			},
			SL
		);
	});

	suite.add("word_wrap_with_new_lines", []() {
		auto str = "ab cd\nef\n g\n  h\n  ijk   \nlm   no p \n\nq\n";
		auto res = utki::word_wrap(str, 5);

		tst::check_eq(res.size(), size_t(12), SL);
		tst::check_eq(res[0], "ab cd"s, SL);
		tst::check_eq(res[1], "ef"s, SL);
		tst::check_eq(res[2], " g"s, SL);
		tst::check_eq(res[3], "  h"s, SL);
		tst::check_eq(res[4], "  ijk"s, SL);
		tst::check_eq(res[5], "   "s, SL);
		tst::check_eq(res[6], "lm"s, SL);
		tst::check_eq(res[7], "  no"s, SL);
		tst::check_eq(res[8], "p "s, SL);
		tst::check_eq(res[9], ""s, SL);
		tst::check_eq(res[10], "q"s, SL);
		tst::check_eq(res[11], ""s, SL);
	});

	suite.add("make_string_from_span_char", []() {
		std::array<char, 12> arr = {
			{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}
		};
		std::vector<char> vec = {
			{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}
		};

		auto str_arr = utki::make_string(utki::make_span(arr));
		tst::check_eq(str_arr, "Hello world!"s, SL);

		auto str_vec = utki::make_string(utki::make_span(vec));
		tst::check_eq(str_vec, "Hello world!"s, SL);
	});

	suite.add("make_string_view_from_span_char_explicit_conversion", []() {
		std::array<char, 12> arr = {
			{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}
		};
		std::vector<char> vec = {
			{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}
		};

		auto str_arr = utki::make_string_view(utki::make_span(arr));
		tst::check_eq(str_arr, "Hello world!"sv, SL);

		auto str_vec = utki::make_string_view(utki::make_span(vec));
		tst::check_eq(str_vec, "Hello world!"sv, SL);
	});

	suite.add("make_string_view_from_span_char_implicit_conversion", []() {
		std::array<char, 12> arr = {
			{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}
		};
		std::vector<char> vec = {
			{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}
		};

		auto str_arr = utki::make_string_view(arr);
		tst::check_eq(str_arr, "Hello world!"sv, SL);

		auto str_vec = utki::make_string_view(vec);
		tst::check_eq(str_vec, "Hello world!"sv, SL);
	});

	suite.add("make_string_view_from_span_uint8_t_explicit_conversion", []() {
		std::array<uint8_t, 12> arr = {
			{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}
		};
		std::vector<uint8_t> vec = {
			{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}
		};

		auto str_arr = utki::make_string_view(utki::make_span(arr));
		tst::check_eq(str_arr, "Hello world!"sv, SL);

		auto str_vec = utki::make_string_view(utki::make_span(vec));
		tst::check_eq(str_vec, "Hello world!"sv, SL);
	});

	suite.add("make_string_view_from_span_uint8_t_implicit_conversion", []() {
		std::array<uint8_t, 12> arr = {
			{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}
		};
		std::vector<uint8_t> vec = {
			{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}
		};

		auto str_arr = utki::make_string_view(arr);
		tst::check_eq(str_arr, "Hello world!"sv, SL);

		auto str_vec = utki::make_string_view(vec);
		tst::check_eq(str_vec, "Hello world!"sv, SL);
	});

	suite.add("make_string_from_array_char", []() {
		std::array<char, 12> arr = {
			{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}
		};

		auto str_arr = utki::make_string(arr);
		tst::check_eq(str_arr, "Hello world!"s, SL);
	});

	suite.add("make_string_from_array_uint8_t", []() {
		std::array<uint8_t, 12> arr = {
			{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}
		};

		auto str_arr = utki::make_string(arr);
		tst::check_eq(str_arr, "Hello world!"s, SL);
	});

	suite.add("make_string_from_vector_char", []() {
		std::vector<char> vec = {
			{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}
		};

		auto str_vec = utki::make_string(vec);
		tst::check_eq(str_vec, "Hello world!"s, SL);
	});

	suite.add("make_string_from_vector_uint8_t", []() {
		std::vector<uint8_t> vec = {
			{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}
		};

		auto str_vec = utki::make_string(vec);
		tst::check_eq(str_vec, "Hello world!"s, SL);
	});

	suite.add("from_chars_float", []() {
		std::string_view str = "3.14159  ";

		// NOLINTNEXTLINE(cppcoreguidelines-init-variables)
		float v;

		auto r = utki::from_chars(str.data(), str.data() + str.size(), v);

		tst::check_ne(r.ptr, str.data(), SL);
		tst::check(r.ec == std::errc(), SL);
		tst::check_eq(v, 3.14159f, SL);
	});

	suite.add("from_chars_double", []() {
		std::string_view str = "3.14159  ";

		// NOLINTNEXTLINE(cppcoreguidelines-init-variables)
		double v;

		auto r = utki::from_chars(str.data(), str.data() + str.size(), v);

		tst::check_ne(r.ptr, str.data(), SL);
		tst::check(r.ec == std::errc(), SL);
		tst::check_eq(v, 3.14159, SL);
	});

	suite.add("string_parser", []() {
		// make sure the locale does not affect parsing (decimal delimiter can be "." or "," in different locales)
		// so, set DE locale which has "," to make sure it does not affect the parsing
		if (!std::setlocale(LC_ALL, "de_DE.UTF-8")) {
			utki::log([](auto& o) {
				o << "WARNING: failed to set locale de_DE.UTF-8, perhaps the locale is not installed. Testing that locale does not affect parsing will not be done."
				  << std::endl;
			});
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
			tst::check_eq(
				n,
				unsigned(13),
				[&](auto& o) {
					o << "n = " << n;
				},
				SL
			);
			tst::check_eq(p.peek_char(), ',', SL);
		}

		p.skip_whitespaces_and_comma();
		{
			auto n = p.read_number<unsigned>();
			tst::check(!p.empty(), SL);
			tst::check_eq(
				n,
				unsigned(13),
				[&](auto& o) {
					o << "n = " << n;
				},
				SL
			);
			tst::check_eq(p.peek_char(), ',', SL);
		}

		p.skip_whitespaces_and_comma();
		{
			auto n = p.read_number<unsigned>();
			tst::check(p.empty(), SL);
			tst::check_eq(
				n,
				unsigned(13),
				[&](auto& o) {
					o << "n = " << n;
				},
				SL
			);
		}
	});

	suite.add("string_parser_skip_inclusive_until_one_of", []() {
		auto str = "Hello _ World!";

		utki::string_parser p(str);

		tst::check_eq(p.peek_char(), 'H', SL);

		{
			const std::array<char, 2> one_of = {'W', 'o'};
			auto c = p.skip_inclusive_until_one_of(one_of);
			tst::check(!p.empty(), SL);
			tst::check_eq(p.peek_char(), ' ', SL);
			tst::check_eq(c, 'o', SL);
		}

		{
			const std::array<char, 2> one_of = {'W', 'o'};
			auto c = p.skip_inclusive_until_one_of(one_of);
			tst::check(!p.empty(), SL);
			tst::check_eq(p.peek_char(), 'o', SL);
			tst::check_eq(c, 'W', SL);
		}

		{
			auto c = p.skip_inclusive_until_one_of("Wnm");
			tst::check(p.empty(), SL);
			tst::check_eq(c, '\0', SL);
		}
	});

	suite.add("string_parser_skip_inclusive_until_one_of__empty_char_set", []() {
		auto str = "Hello _ World!";

		utki::string_parser p(str);

		tst::check_eq(p.peek_char(), 'H', SL);

		{
			auto c = p.skip_inclusive_until_one_of(nullptr);
			tst::check(p.empty(), SL);
			tst::check_eq(c, '\0', SL);
		}
	});

	suite.add("string_parser_parse_float_close_to_zero_with_exponent", []() {
		auto str = "5.47382e-48";

		utki::string_parser p(str);

		auto num = p.read_number<float>();

		tst::check_lt(num, 0.00001f, SL);
		tst::check(p.empty(), SL);
	});

	suite.add("fast_float_small_float", []() {
		std::string_view str = "5.47382e-48";
		float val = 100.0f;
		auto res = fast_float::from_chars(str.data(), str.data() + str.size(), val, fast_float::general);

		std::cout << "val = " << val << std::endl;

		tst::check(res.ptr == str.data() + str.size(), SL);

		tst::check_eq(val, 0.0f, SL);

		tst::check(res.ec == std::errc(), SL);
	});

	suite.add<std::pair<size_t, std::string>>(
		"to_string__size_t",
		// clang-format off
		{
			{ 0, "0"},
			{ 1, "1"},
			{ 13, "13"},

			// size_t can be 32 bit on 32 bit CPUs, so don't go over 32 bit max value
			{std::numeric_limits<uint32_t>::max(), "4294967295"}
		},
		// clang-format on
		[](const auto& p) {
			auto s = utki::to_string(p.first);

			tst::check_eq(s, p.second, SL);
		}
	);

	suite.add("cat", []() {
		tst::check_eq(utki::cat(), ""s, SL);
		tst::check_eq(utki::cat("hello "), "hello "s, SL);
		tst::check_eq(utki::cat("num = "s, 1234), "num = 1234"s, SL);
		tst::check_eq(utki::cat("hello "s, "world"sv, "!"), "hello world!"s, SL);
	});

	suite.add("trim_front", []() {
		const char* str = "\n  hello world \t\n";
		std::string string = str;
		std::string_view string_view = string;
		std::string_view expected = "hello world \t\n"sv;
		tst::check_eq(utki::trim_front(str), expected, SL);
		tst::check_eq(utki::trim_front(string), expected, SL);
		tst::check_eq(utki::trim_front(string_view), expected, SL);
	});

	suite.add("trim_back", []() {
		const char* str = "\n  hello world \t\n";
		std::string string = str;
		std::string_view string_view = string;
		std::string_view expected = "\n  hello world"sv;
		tst::check_eq(utki::trim_back(str), expected, SL);
		tst::check_eq(utki::trim_back(string), expected, SL);
		tst::check_eq(utki::trim_back(string_view), expected, SL);
	});

	suite.add("trim", []() {
		const char* str = "\n  hello world \t\n";
		std::string string = str;
		std::string_view string_view = string;
		std::string_view expected = "hello world"sv;
		tst::check_eq(utki::trim(str), expected, SL);
		tst::check_eq(utki::trim(string), expected, SL);
		tst::check_eq(utki::trim(string_view), expected, SL);
	});

	suite.add("indentation", []() {
		tst::check_eq(utki::make_indentation(0, 1), ""s, SL);
		tst::check_eq(utki::make_indentation(0, 2), ""s, SL);
		tst::check_eq(utki::make_indentation(1, 1), " "s, SL);
		tst::check_eq(utki::make_indentation(2, 1), "  "s, SL);
		tst::check_eq(utki::make_indentation(1, 2), "  "s, SL);
		tst::check_eq(utki::make_indentation(2, 2), "    "s, SL);
		tst::check_eq(utki::make_indentation(3, 2), "      "s, SL);
		tst::check_eq(utki::make_indentation(2, 3), "      "s, SL);
		tst::check_eq(utki::make_indentation(2, 3), "      "s, SL);
		tst::check_eq(utki::make_indentation(0, 0), ""s, SL);
		tst::check_eq(utki::make_indentation(1, 0), "\t"s, SL);
		tst::check_eq(utki::make_indentation(2, 0), "\t\t"s, SL);
		tst::check_eq(utki::make_indentation(3, 0), "\t\t\t"s, SL);
	});

	suite.add("starts_with", []() {
		tst::check(utki::starts_with("asdfghjk", "asd"), SL);
		tst::check(!utki::starts_with(" s asdfghjk", "  asd"), SL);
		tst::check(utki::starts_with("asdfghjk", ""), SL);
		tst::check(!utki::starts_with("", "asd"), SL);
		tst::check(!utki::starts_with(" asdfghjk", "asd"), SL);
		tst::check(!utki::starts_with("a", "asd"), SL);
	});

	suite.add("ends_with", []() {
		tst::check(utki::ends_with("asdfghjk", "hjk"), SL);
		tst::check(!utki::ends_with(" s asdfghjk", "  asdfghjk"), SL);
		tst::check(utki::ends_with("asdfghjk", ""), SL);
		tst::check(!utki::ends_with("", "asd"), SL);
		tst::check(!utki::ends_with(" asdfghjk ", "hjk"), SL);
		tst::check(!utki::ends_with("k", "hjk"), SL);
	});
});
} // namespace

#endif // ~ non-MSVC compiler or MSVC compiler tools >= v142
