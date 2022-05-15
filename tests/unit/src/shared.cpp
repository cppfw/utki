#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/shared.hpp>

namespace {
class TestClass : public utki::shared
{
public:
	int a = 4;

	TestClass() {}

	TestClass(int i) :
		a(i)
	{}

	std::shared_ptr<TestClass> getPtr()
	{
		return utki::make_shared_from(*this);
	}
};
} // namespace

namespace {
tst::set set("shared", [](tst::suite& suite) {
	suite.add("basic", []() {
		std::shared_ptr<TestClass> p1 = std::make_shared<TestClass>();

		std::shared_ptr<TestClass> p2 = std::make_shared<TestClass>(21);

		tst::check_eq(p1->a, 4, SL);
		tst::check_eq(p2->a, 21, SL);

		tst::check_eq(p2->getPtr().operator->(), p2.operator->(), SL);
	});

	suite.add("make_shared_from", []() {
		auto o = std::make_shared<TestClass>();
		tst::check(o, SL);

		auto sft = utki::make_shared_from(*o);
		tst::check(sft, SL);
		tst::check_eq(sft->a, 4, SL);
	});

	suite.add("make_weak_from", []() {
		auto o = std::make_shared<TestClass>();
		tst::check(o, SL);

		auto sft = utki::make_weak_from(*o);
		tst::check(sft.lock(), SL);
		tst::check_eq(sft.lock()->a, 4, SL);
	});
});
} // namespace
