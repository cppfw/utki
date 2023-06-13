#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/destructable.hpp>

namespace {
const tst::set set("destructable", [](tst::suite& suite) {
	suite.add("virtual_destructor", []() {
		class test : public utki::destructable
		{
		public:
			// NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
			bool& destructor_called;

			test(bool& destructor_called) :
				destructor_called(destructor_called)
			{}

			test(const test&) = delete;
			test& operator=(const test&) = delete;

			test(test&&) = delete;
			test& operator=(test&&) = delete;

			~test() override
			{
				this->destructor_called = true;
			}
		};

		bool destructor_called = false;
		std::unique_ptr<utki::destructable> p = std::make_unique<test>(destructor_called);

		tst::check(!destructor_called, SL);

		p.reset();

		tst::check(destructor_called, SL);
	});
});
} // namespace
