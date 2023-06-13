#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/config.hpp>
#include <utki/singleton.hpp>

#if CFG_COMPILER != CFG_COMPILER_MSVC

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

namespace {
class test_singleton : public utki::singleton<test_singleton>
{
public:
	int a = 13;
};
} // namespace

namespace {
const tst::set set("singleton", [](tst::suite& suite) {
	suite.add( //
		"only_one_singleton_instance_can_exist_at_a_time",
		tst::flag::no_parallel,
		[] {
			test_singleton sing1;

			try {
				test_singleton sing2;
				tst::check(false, SL) << "creating second singleton object should throw";
			} catch (std::logic_error&) {
			}
		}
	);
});
} // namespace
#endif // ~msvc compiler

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
