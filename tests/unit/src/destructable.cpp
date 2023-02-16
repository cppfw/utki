#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/destructable.hpp>

namespace {
tst::set set("destructable", [](tst::suite& suite) {
	suite.add("virtual_destructor", []() {
		class test : public utki::destructable
		{
		public:
			bool& destructor_called;

			test(bool& destructor_called) :
				destructor_called(destructor_called)
			{}

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
