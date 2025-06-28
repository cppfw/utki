#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/type_traits.hpp>

namespace {
const tst::set set("type_traits", [](tst::suite& suite) {
	suite.add("is_scoped_enum", []() {
		// NOLINTNEXTLINE(cppcoreguidelines-use-enum-class, "intentionally testing non-class enum")
		enum unscoped {
			a,
			b,
			c
		};

		enum class scoped {
			a,
			b,
			c
		};

		static_assert(!utki::is_scoped_enum_v<int>);
		static_assert(!utki::is_scoped_enum_v<unscoped>);
		static_assert(utki::is_scoped_enum_v<scoped>);

		tst::check(!utki::is_scoped_enum_v<int>, SL);
		tst::check(!utki::is_scoped_enum_v<unscoped>, SL);
		tst::check(utki::is_scoped_enum_v<scoped>, SL);
	});
});
} // namespace