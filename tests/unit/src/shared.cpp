#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/shared.hpp>

namespace {
class test_class : public utki::shared
{
public:
	int a = 4;

	test_class() = default;

	test_class(int i) :
		a(i)
	{}

	std::shared_ptr<test_class> get_ptr()
	{
		return utki::make_shared_from(*this).to_shared_ptr();
	}
};
} // namespace

namespace {
tst::set set("shared", [](tst::suite& suite) {
	suite.add("basic", []() {
		std::shared_ptr<test_class> p1 = std::make_shared<test_class>();

		std::shared_ptr<test_class> p2 = std::make_shared<test_class>(21);

		tst::check_eq(p1->a, 4, SL);
		tst::check_eq(p2->a, 21, SL);

		tst::check_eq(p2->get_ptr().operator->(), p2.operator->(), SL);
	});

	suite.add("make_weak_from_shared_ptr", []() {
		auto o = std::make_shared<test_class>();
		tst::check(o, SL);

		auto w = utki::make_weak(o);

		auto p = w.lock();

		tst::check(p, SL);
		tst::check_eq(p->a, 4, SL);
	});

	suite.add("make_weak_from_shared_ref", []() {
		auto o = utki::make_shared_ref<test_class>();

		auto w = utki::make_weak(o);

		auto p = w.lock();

		tst::check(p, SL);
		tst::check_eq(p->a, 4, SL);
	});

	suite.add("make_shared_from", []() {
		auto o = std::make_shared<test_class>();
		tst::check(o, SL);

		auto sft = utki::make_shared_from(*o);
		tst::check_eq(sft->a, 4, SL);
	});

	suite.add("make_weak_from", []() {
		auto o = std::make_shared<test_class>();
		tst::check(o, SL);

		auto sft = utki::make_weak_from(*o);
		tst::check(sft.lock(), SL);
		tst::check_eq(sft.lock()->a, 4, SL);
	});
});
} // namespace
