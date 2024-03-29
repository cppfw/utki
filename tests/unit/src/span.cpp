#include <string>

#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/span.hpp>

// undefine possibly defined macro
#ifdef assert
#	undef assert
#endif

using namespace std::string_literals;

// declare templates to instantiate all template methods to include all methods to gcov coverage
template class utki::span<const int>;

namespace {
const tst::set set("span", [](tst::suite& suite) {
	suite.add("output_to_stream_operator", []() {
		std::array<char, 12> buf = {
			{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}
		};

		std::stringstream ss;

		ss << utki::make_span(buf);

		tst::check_eq(ss.str(), "Hello world!"s, SL);
	});

	suite.add("make_span_from_const_string", []() {
		std::string str = "Hello world!";

		auto s = utki::make_span(str);

		tst::check_eq(s.size(), str.size(), SL);
		tst::check_eq(s.data(), str.data(), SL);
	});

	suite.add("make_span_from_const_char_ptr", []() {
		const char* str = "Hello world!";
		auto s = utki::make_span(str);

		tst::check_eq(s.size(), strlen(str), SL);
		tst::check_eq(s.data(), str, SL);
	});

	suite.add("constructor_from_initializer_list", []() {
		std::vector<int> v = {10, 13, 14};

		utki::span<const int> cs({10, 13, 14});

		auto f = [&](utki::span<int> s) {
			tst::check(s == v, SL);
		};

		f(v);

		auto cf = [&](utki::span<const int> s) {
			tst::check(s == v, SL);
		};
		cf(v);
	});

	suite.add("initializer_list_downcast", []() {
		struct a0 {
			int a_0;

			a0(int a) :
				a_0(a)
			{}
		};

		struct a1 : public a0 {
			a1(int a) :
				a0(a)
			{}
		};

		auto v = [](utki::span<const std::shared_ptr<a0>> c) {
			std::vector<std::shared_ptr<a0>> ret;
			for (const auto& r : c) {
				ret.push_back(r);
			}
			return ret;
		}({std::make_shared<a1>(2), std::make_shared<a0>(1)});

		tst::check_eq(v.size(), size_t(2), SL);
		tst::check(v.front(), SL);
		tst::check_eq(v.front()->a_0, 2, SL);

		// NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDeleteLeaks, "false positive")
	});

	suite.add("constructor_from_vector", []() {
		std::vector<int> v = {10, 13, 14};

		utki::span<int> s(v);
		tst::check_eq(s, utki::make_span(v), SL);

		utki::span<const int> cs(v);
		tst::check_eq(s, utki::make_span(v), SL);

		auto f = [&](utki::span<int> s) {
			tst::check(s == v, SL);
		};

		f(v);

		auto cf = [&](utki::span<const int> s) {
			tst::check(s == v, SL);
		};
		cf(v);
		cf(s);
	});

	suite.add("constructor_from_const_vector", []() {
		const std::vector<int> v = {10, 13, 14};

		utki::span<const int> cs(v);
		tst::check_eq(cs, utki::make_span(v), SL);

		auto cf = [&](utki::span<const int> s) {
			tst::check(s == v, SL);
		};
		cf(v);
	});

	suite.add("constructor_from_array", []() {
		std::array<int, 3> v = {10, 13, 14};

		utki::span<int> s(v);
		tst::check_eq(s, utki::make_span(v), SL);

		utki::span<const int> cs(v);
		tst::check_eq(s, utki::make_span(v), SL);

		auto f = [&](utki::span<int> s) {
			tst::check(s == v, SL);
		};

		f(v);

		auto cf = [&](utki::span<const int> s) {
			tst::check(s == v, SL);
		};
		cf(v);
		cf(s);
	});

	suite.add("constructor_from_const_array", []() {
		const std::array<int, 3> v = {10, 13, 14};

		utki::span<const int> cs(v);
		tst::check_eq(cs, utki::make_span(v), SL);

		auto cf = [&](utki::span<const int> s) {
			tst::check(s == v, SL);
		};
		cf(v);
	});

	suite.add("constructor_from_string", []() {
		std::string v = "hello world!";

		utki::span<char> s(v);
		tst::check_eq(s, utki::make_span(v), SL);

		utki::span<const char> cs(v);
		tst::check_eq(s, utki::make_span(v), SL);

		auto f = [&](utki::span<char> s) {
			tst::check(s == v, SL);
		};

		f(v);

		auto cf = [&](utki::span<const char> s) {
			tst::check(s == v, SL);
		};
		cf(v);
		cf(s);
	});

	suite.add("constructor_from_const_string", []() {
		const std::string v = "hello world!";

		utki::span<const char> cs(v);
		tst::check_eq(cs, utki::make_span(v), SL);

		auto cf = [&](utki::span<const char> s) {
			tst::check_eq(s, utki::make_span(v), SL);
		};
		cf(v);
	});

	suite.add("constructor_from_string_view", []() {
		std::string_view v = "hello world!";

		utki::span<const char> cs(v);
		tst::check_eq(cs, utki::make_span(v), SL);

		auto cf = [&](utki::span<const char> s) {
			tst::check(s == v, SL);
		};
		cf(v);
	});

	suite.add("constructor_from_const_char_ptr", []() {
		const char* v = "hello world!";

		utki::span<const char> cs(v);
		tst::check_eq(cs, utki::make_span(v), SL);

		auto cf = [&](utki::span<const char> s) {
			tst::check(s == v, SL);
		};
		cf(v);
	});

	suite.add("constructor_copy", []() {
		const char* v = "hello world!";

		utki::span<const char> cs(v);

		utki::span<const char> cs2(cs);

		tst::check_eq(cs, utki::make_span(v), SL);
		tst::check_eq(cs, cs2, SL);
	});

	suite.add("operator_assign_copy", []() {
		const char* v = "hello world!";

		utki::span<const char> cs(v);

		utki::span<const char> cs2;

		cs2 = cs;

		tst::check_eq(cs, utki::make_span(v), SL);
		tst::check_eq(cs, cs2, SL);
	});

	suite.add("constructor_move", []() {
		const char* v = "hello world!";

		utki::span<const char> cs(v);

		utki::span<const char> cs2(std::move(cs));

		// moved from span should remain valid
		// NOLINTNEXTLINE(bugprone-use-after-move)
		tst::check_eq(cs, utki::make_span(v), SL);
		tst::check_eq(cs, cs2, SL);
	});

	suite.add("operator_assign_move", []() {
		const char* v = "hello world!";

		utki::span<const char> cs(v);

		utki::span<const char> cs2;

		cs2 = std::move(cs);

		// moved from span should remain valid
		// NOLINTNEXTLINE(bugprone-use-after-move)
		tst::check_eq(cs, utki::make_span(v), SL);
		tst::check_eq(cs, cs2, SL);
	});

	struct subspan_fixture {
		std::string str = "Hello world!";

		utki::span<char> s = utki::make_span(str);

		subspan_fixture()
		{
			utki::assert(this->s.size() == this->str.size(), SL);
			utki::assert(this->s.data() == this->str.data(), SL);
		}
	};

	suite.add("subspan_offset", []() {
		subspan_fixture f;

		auto ss = f.s;

		ss = ss.subspan(3);

		tst::check_eq(ss.size(), f.str.size() - 3, SL);
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		tst::check_eq(ss.data(), f.str.data() + 3, SL);
	});

	suite.add("subspan_offset_equals_size", []() {
		subspan_fixture f;

		auto ss = f.s.subspan(f.s.size());

		tst::check_eq(
			ss.size(),
			size_t(0),
			[&](auto& o) {
				o << "ss.size() = " << ss.size();
			},
			SL
		);
	});

	suite.add("subspan_offset_greater_than_size", []() {
		subspan_fixture f;

		auto ss = f.s.subspan(1000);

		tst::check_eq(
			ss.size(),
			size_t(0),
			[&](auto& o) {
				o << "ss.size() = " << ss.size();
			},
			SL
		);
	});

	suite.add("subspan_offset_count", []() {
		subspan_fixture f;

		auto ss = f.s.subspan(3, 4);

		tst::check_eq(
			ss.size(),
			size_t(4),
			[&](auto& o) {
				o << "ss.size() = " << ss.size();
			},
			SL
		);
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		tst::check_eq(ss.data(), f.str.data() + 3, SL);
	});

	suite.add("make_vector_from_span", []() {
		std::vector<size_t> in = {10, 20, 13, 65};

		auto sp = utki::make_span(in);

		auto out = utki::make_vector(sp);

		tst::check(in == out, SL);
	});

	suite.add("make_vector_from_const_span", []() {
		std::vector<size_t> in = {10, 20, 13, 65};

		auto sp = utki::make_span(in);

		utki::span<const size_t> csp = sp;

		auto out = utki::make_vector(csp);

		tst::check(in == out, SL);
	});

	suite.add("operators_equals_and_not_equals", []() {
		std::vector<size_t> v1 = {10, 20, 13, 65};
		std::array<size_t, 6> v2 = {3, 10, 20, 13, 65, 73};

		tst::check(utki::make_span(v1) != utki::make_span(v2), SL);
		tst::check(utki::make_span(v1) == utki::make_span(v2).subspan(1, 4), SL);
	});

	suite.add("const_operators_equals_and_not_equals", []() {
		const std::vector<size_t> v1 = {10, 20, 13, 65};
		const std::array<size_t, 6> v2 = {3, 10, 20, 13, 65, 73};

		tst::check(utki::make_span(v1) != utki::make_span(v2), SL);
		tst::check(utki::make_span(v1) == utki::make_span(v2).subspan(1, 4), SL);
	});

	suite.add("semiconst_operaots_equals_and_not_equals", []() {
		const std::vector<size_t> v1 = {10, 20, 13, 65};
		std::array<size_t, 6> v2 = {3, 10, 20, 13, 65, 73};

		tst::check(utki::make_span(v1) != utki::make_span(v2), SL);
		tst::check(utki::make_span(v1) == utki::make_span(v2).subspan(1, 4), SL);
	});

	suite.add("another_semiconst_operaots_equals_and_not_equals", []() {
		std::vector<size_t> v1 = {10, 20, 13, 65};
		const std::array<size_t, 6> v2 = {3, 10, 20, 13, 65, 73};

		tst::check(utki::make_span(v1) != utki::make_span(v2), SL);
		tst::check(utki::make_span(v1) == utki::make_span(v2).subspan(1, 4), SL);
	});

	suite.add("operator_less_than", []() {
		std::vector<size_t> v1 = {10, 20, 13, 65};
		std::array<size_t, 6> v2 = {3, 10, 20, 13, 65, 73};

		tst::check(utki::make_span(v2) < utki::make_span(v1), SL);
		tst::check(!(utki::make_span(v1) < utki::make_span(v2).subspan(1, 4)), SL);
		tst::check(utki::make_span(v1) < utki::make_span(v2).subspan(1, 5), SL);
	});

	suite.add("operator_greater_than_or_equals", []() {
		std::vector<size_t> v1 = {10, 20, 13, 65};
		std::array<size_t, 6> v2 = {3, 10, 20, 13, 65, 73};

		tst::check(utki::make_span(v1) >= utki::make_span(v2), SL);
		tst::check(!(utki::make_span(v2) >= utki::make_span(v1).subspan(1, 4)), SL);
		tst::check(utki::make_span(v1) >= utki::make_span(v2).subspan(1, 4), SL);
		tst::check(utki::make_span(v2).subspan(1, 5) >= utki::make_span(v1), SL);
	});

	suite.add("operator_greater_than", []() {
		std::vector<size_t> v1 = {10, 20, 13, 65};
		std::array<size_t, 6> v2 = {3, 10, 20, 13, 65, 73};

		tst::check(utki::make_span(v1) > utki::make_span(v2), SL);
		tst::check(!(utki::make_span(v1) > utki::make_span(v2).subspan(1, 4)), SL);
		tst::check(utki::make_span(v2).subspan(1, 5) > utki::make_span(v1), SL);
	});

	suite.add("operator_less_than_or_equals", []() {
		std::vector<size_t> v1 = {10, 20, 13, 65};
		std::array<size_t, 6> v2 = {3, 10, 20, 13, 65, 73};

		tst::check(utki::make_span(v2) <= utki::make_span(v1), SL);
		tst::check(utki::make_span(v1) <= utki::make_span(v2).subspan(1, 4), SL);
		tst::check(utki::make_span(v2).subspan(1, 4) <= utki::make_span(v1), SL);
		tst::check(utki::make_span(v1) <= utki::make_span(v2).subspan(1, 5), SL);
	});

	suite.add("method_empty", [] {
		std::vector<size_t> v1;
		utki::span<size_t> s(v1);

		tst::check(s.empty(), SL);
	});

	suite.add("to_uint8_t__and__to_char", []() {
		std::string str = "abcd";

		auto s = to_uint8_t(utki::make_span(str));

		tst::check_eq(s.size(), str.size(), SL);
		tst::check_eq(s[0], uint8_t(str[0]), SL);
		tst::check_eq(s[1], uint8_t(str[1]), SL);
		tst::check_eq(s[2], uint8_t(str[2]), SL);
		tst::check_eq(s[3], uint8_t(str[3]), SL);

		auto cs = to_char(s);

		tst::check_eq(cs.size(), str.size(), SL);
		tst::check_eq(cs[0], str[0], SL);
		tst::check_eq(cs[1], str[1], SL);
		tst::check_eq(cs[2], str[2], SL);
		tst::check_eq(cs[3], str[3], SL);
	});

	suite.add("to_uint8_t__and__to_char__const", []() {
		const std::string str = "abcd";

		auto s = to_uint8_t(utki::make_span(str));

		tst::check_eq(s.size(), str.size(), SL);
		tst::check_eq(s[0], uint8_t(str[0]), SL);
		tst::check_eq(s[1], uint8_t(str[1]), SL);
		tst::check_eq(s[2], uint8_t(str[2]), SL);
		tst::check_eq(s[3], uint8_t(str[3]), SL);

		auto cs = to_char(s);

		tst::check_eq(cs.size(), str.size(), SL);
		tst::check_eq(cs[0], str[0], SL);
		tst::check_eq(cs[1], str[1], SL);
		tst::check_eq(cs[2], str[2], SL);
		tst::check_eq(cs[3], str[3], SL);
	});
});
} // namespace
