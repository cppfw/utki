#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/shared_ref.hpp>

namespace {
const std::string etalon = "hello world!";

struct a0 {
	int a_0;

	a0(int a) :
		a_0(a)
	{}
};

struct a1 : public a0 {
	int a_1;

	a1(int a) :
		a0(a),
		a_1(a)
	{}
};

tst::set set("shared_ref", [](tst::suite& suite) {
	suite.add("get", []() {
		utki::shared_ref<std::string> sr = utki::make_shared_ref<std::string>(etalon);

		tst::check_eq(sr.get(), etalon, SL);
	});

	suite.add("const_get", []() {
		const utki::shared_ref<std::string> sr = utki::make_shared_ref<std::string>(etalon);

		tst::check_eq(sr.get(), etalon, SL);

		sr->append("!");

		tst::check_eq(etalon + "!", sr.get(), SL);
	});

	suite.add("operator_arrow", []() {
		utki::shared_ref<std::string> sr = utki::make_shared_ref<std::string>(etalon);

		tst::check_eq(etalon, std::string(sr->c_str()), SL);
	});

	suite.add("const_operator_arrow", []() {
		const utki::shared_ref<std::string> sr = utki::make_shared_ref<std::string>(etalon);

		tst::check_eq(etalon, std::string(sr->c_str()), SL);

		sr->append("!");

		tst::check_eq(etalon + "!", std::string(sr->c_str()), SL);
	});

	suite.add("operator_asterisk", []() {
		utki::shared_ref<std::string> sr = utki::make_shared_ref<std::string>(etalon);

		tst::check_eq(*sr, etalon, SL);
	});

	suite.add("const_operator_asterisk", []() {
		const utki::shared_ref<std::string> sr = utki::make_shared_ref<std::string>(etalon);

		tst::check_eq(*sr, etalon, SL);

		sr->append("!");

		tst::check_eq(etalon + "!", *sr, SL);
	});

	suite.add("const_autocast", []() {
		utki::shared_ref<const std::string> sr = utki::make_shared_ref<std::string>(etalon);

		tst::check_eq(*sr, etalon, SL);
	});

	suite.add("downcast", []() {
		utki::shared_ref<a0> sr = utki::make_shared_ref<a1>(2);

		tst::check_eq(sr->a_0, 2, SL);
	});

	suite.add("const_downcast", []() {
		utki::shared_ref<const a0> sr = utki::make_shared_ref<a1>(2);

		tst::check_eq(sr->a_0, 2, SL);
	});

	suite.add("std_shared_ptr_auto_conversion", []() {
		std::shared_ptr<std::string> sp = utki::make_shared_ref<std::string>(etalon);

		tst::check_eq(*sp, etalon, SL);
	});

	suite.add("std_shared_ptr_downcast", []() {
		std::shared_ptr<a0> sp = utki::make_shared_ref<a1>(2);

		tst::check_eq(sp->a_0, 2, SL);
	});

	suite.add("std_shared_ptr_const_auto_conversion", []() {
		std::shared_ptr<const std::string> sp = utki::make_shared_ref<std::string>(etalon);

		tst::check_eq(*sp, etalon, SL);
	});

	suite.add("std_shared_ptr_const_downcast", []() {
		std::shared_ptr<const a0> sp = utki::make_shared_ref<a1>(2);

		tst::check_eq(sp->a_0, 2, SL);
	});

	suite.add("std_shared_ptr_const_auto_conversion_from_const", []() {
		std::shared_ptr<const std::string> sp = utki::make_shared_ref<const std::string>(etalon);

		tst::check_eq(*sp, etalon, SL);
	});

	suite.add("std_shared_ptr_const_downcast_from_const", []() {
		std::shared_ptr<const a0> sp = utki::make_shared_ref<const a1>(2);

		tst::check_eq(sp->a_0, 2, SL);
	});

	suite.add("weak_ptr", []() {
		auto sr = utki::make_shared_ref<a1>(3);

		std::weak_ptr<a1> wp = sr;

		auto sp = wp.lock();
		tst::check(sp, SL);

		tst::check_eq(sp->a_0, 3, SL);
		tst::check_eq(sp->a_1, 3, SL);
	});

	suite.add("weak_ptr_const_autocast", []() {
		auto sr = utki::make_shared_ref<a1>(3);

		std::weak_ptr<const a1> wp = sr;

		auto sp = wp.lock();
		tst::check(sp, SL);

		tst::check_eq(sp->a_0, 3, SL);
		tst::check_eq(sp->a_1, 3, SL);
	});

	suite.add("weak_ptr_downcast", []() {
		auto sr = utki::make_shared_ref<a1>(3);

		std::weak_ptr<a0> wp = sr;

		auto sp = wp.lock();
		tst::check(sp, SL);

		tst::check_eq(sp->a_0, 3, SL);
	});

	suite.add("weak_ptr_const_downcast", []() {
		auto sr = utki::make_shared_ref<a1>(3);

		std::weak_ptr<const a0> wp = sr;

		auto sp = wp.lock();
		tst::check(sp, SL);

		tst::check_eq(sp->a_0, 3, SL);
	});
});

} // namespace
