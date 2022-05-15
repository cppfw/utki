#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/debug.hpp>

namespace {
tst::set set("debug", [](tst::suite& suite) {
	suite.add("assert_macro", []() {
		int a = 13;
		ASSERT(a == 13)
		ASSERT(sizeof(int) == 4)
		ASSERT(sizeof(a) == 4 && sizeof(a) == sizeof(int))
		a += 4;
	});

	suite.add("assert_macro_with_message", []() {
		int a = 13;
		ASSERT(a == 13, [&](auto& o) { o << "a is not 13, a is " << a; })
		ASSERT_INFO(a == 13, "a is not 13, a is " << a)
		a += 4;
	});

	suite.add("assert", []() {
		int b = 13;
		utki::assert(b == 13, SL);
#if M_CPP >= 20
		utki::assert(b == 13);
#endif
	});

	suite.add("assert_with_message", []() {
		int b = 13;
		utki::assert(
			b == 13,
			[&](auto& o) { o << "b = " << b; },
			SL
		);
#if M_CPP >= 20
		utki::assert(b == 13, [&](auto& o) { o << "b = " << b; });
#endif
	});
});
} // namespace
