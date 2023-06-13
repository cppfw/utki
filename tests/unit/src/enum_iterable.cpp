#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/enum_iterable.hpp>

namespace {
const tst::set set("enum_iterable", [](tst::suite& suite) {
	enum class test_enum {
		item1,
		item2,
		item3,

		enum_size
	};

	suite.add("enum_iterable_v", []() {
		unsigned count = 0;

		for (auto i : utki::enum_iterable_v<test_enum>) {
			tst::check_eq(unsigned(i), count, SL);
			++count;
		}

		tst::check_eq(count, unsigned(3), SL);
	});
});
} // namespace
