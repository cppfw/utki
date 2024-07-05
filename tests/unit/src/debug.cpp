#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/debug.hpp>

// undefine possibly defined macro
#ifdef assert
#	undef assert
#endif

using namespace std::string_literals;

class test_class
{};

namespace {
const tst::set set("debug", [](tst::suite& suite) {
	suite.add("assert_macro", []() {
		int a = 13;
		ASSERT(a == 13)
		ASSERT(sizeof(int) == 4)
		ASSERT(sizeof(a) == 4 && sizeof(a) == sizeof(int))
		a += 4;
		utki::assert(a == 17, SL);
	});

	suite.add("assert_macro_with_message", []() {
		int a = 13;
		ASSERT(a == 13, [&](auto& o) {
			o << "a is not 13, a is " << a;
		})
		a += 4;
		utki::assert(a == 17, SL);
	});

	suite.add("assert", []() {
		int b = 13;
		utki::assert(b == 13, SL);
#if CFG_CPP >= 20
		utki::assert(b == 13);
#endif
	});

	suite.add("assert_with_message", []() {
		int b = 13;
		utki::assert(
			b == 13,
			[&](auto& o) {
				o << "b = " << b;
			},
			SL
		);
#if CFG_CPP >= 20
		utki::assert(b == 13, [&](auto& o) {
			o << "b = " << b;
		});
#endif
	});

	suite.add("demangle__class", []() {
		test_class a;

		auto mangled_name = std::string(typeid(a).name());

		auto name = utki::demangle(mangled_name.c_str());

		auto expected =
#if CFG_COMPILER == CFG_COMPILER_GCC || CFG_COMPILER == CFG_COMPILER_CLANG
			"test_class"s;
#else
		mangled_name;
#endif
		tst::check_eq(name, expected, SL);
	});

	suite.add("demangle__unsigned", []() {
		unsigned a = 0;

		auto mangled_name = std::string(typeid(a).name());

		auto name = utki::demangle(mangled_name.c_str());

		auto expected =
#if CFG_COMPILER == CFG_COMPILER_GCC || CFG_COMPILER == CFG_COMPILER_CLANG
			"unsigned int"s;
#else
		mangled_name;
#endif
		tst::check_eq(name, expected, SL);
	});
});
} // namespace
