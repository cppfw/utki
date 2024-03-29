#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/debug.hpp>

// undefine possibly defined macro
#ifdef assert
#	undef assert
#endif

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
});
} // namespace
