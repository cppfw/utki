#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/enum_iterable.hpp>

namespace {
tst::set set("enum_iterable", [](tst::suite& suite) {
	suite.add("basic", []() {
		enum class test_enum {
			item1,
			item2,
			item3,

			enum_size
		};

		unsigned count = 0;

		for (auto i : utki::enum_iterable<test_enum>) {
			tst::check_eq(unsigned(i), count, SL);
			++count;
		}

		tst::check_eq(count, unsigned(3), SL);
	});
});
} // namespace
