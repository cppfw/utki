#include <algorithm>

#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/config.hpp>
#include <utki/shared_ref.hpp>

namespace {
const std::string etalon = "hello world!";

struct a0 {
	int a_0;

	a0(int a) :
		a_0(a)
	{}

	a0(const a0&) = default;
	a0& operator=(const a0&) = default;

	a0(a0&&) = default;
	a0& operator=(a0&&) = default;

	virtual ~a0() = default;
};

struct a1 : public a0 {
	int a_1;

	a1(int a) :
		a0(a),
		a_1(a)
	{}
};

const tst::set set("shared_ref", [](tst::suite& suite) {
	static_assert(
		!std::is_constructible_v<utki::shared_ref<std::string>>,
		"shared_ref must not be default constructible"
		" because shared_ref cannot be nullptr"
	);

	static_assert(
		!std::is_convertible_v<std::shared_ptr<std::string>, utki::shared_ref<std::string>>,
		"shared_ptr must not be convertible to shared_ref"
		" because shared_ptr can be nullptr, but shared_ref cannot"
	);

	static_assert(std::is_move_constructible_v<utki::shared_ref<std::string>>, "shared_ref must be move constructible");

	suite.add("constructor__shared_ptr", []() {
		auto sp = std::make_shared<a1>(3);

		auto sr1 = utki::shared_ref(sp);

		tst::check(sp, SL);
		tst::check(sr1.to_shared_ptr(), SL);
		tst::check(sp == sr1.to_shared_ptr(), SL);

		auto sr2 = utki::shared_ref(std::move(sp));

		tst::check(!sp, SL);
		tst::check(sr2.to_shared_ptr(), SL);
		tst::check(sr2.to_shared_ptr() == sr1.to_shared_ptr(), SL);
	});

	suite.add("copy_constructor", []() {
		utki::shared_ref<a1> sr = utki::make_shared<a1>(3);

		utki::shared_ref<a1> mr(sr);

		tst::check(sr.to_shared_ptr(), SL);
		tst::check(mr.to_shared_ptr(), SL);
		tst::check_eq(sr.get().a_0, mr.get().a_0, SL);
	});

	suite.add("operator_assignment", []() {
		utki::shared_ref<a1> sr1 = utki::make_shared<a1>(3);
		utki::shared_ref<a1> sr2 = utki::make_shared<a1>(4);

		tst::check(!(sr1.to_shared_ptr() == sr2.to_shared_ptr()), SL);

		sr2 = sr1;

		tst::check_eq(sr1.get().a_0, 3, SL);
		tst::check_eq(sr2.get().a_0, 3, SL);
		tst::check_eq(sr1.to_shared_ptr(), sr2.to_shared_ptr(), SL);
	});

	suite.add("get", []() {
		utki::shared_ref<std::string> sr = utki::make_shared<std::string>(etalon);

		tst::check_eq(sr.get(), etalon, SL);
	});

	suite.add("const_get", []() {
		const utki::shared_ref<std::string> sr = utki::make_shared<std::string>(etalon);

		tst::check_eq(sr.get(), etalon, SL);

		sr.get().append("!");

		tst::check_eq(etalon + "!", sr.get(), SL);
	});

	suite.add("operator_convert_to_reference", []() {
		utki::shared_ref<std::string> sr = utki::make_shared<std::string>(etalon);

		[](std::string& s) {
			tst::check_eq(s, etalon, SL);
		}(sr);

		[](const std::string& s) {
			tst::check_eq(s, etalon, SL);
		}(sr);
	});

	suite.add("operator_convert_to_reference_const", []() {
		utki::shared_ref<const std::string> sr = utki::make_shared<std::string>(etalon);

		[](const std::string& s) {
			tst::check_eq(s, etalon, SL);
		}(sr);
	});

	suite.add("const_autocast", []() {
		utki::shared_ref<const std::string> sr = utki::make_shared<std::string>(etalon);

		tst::check_eq(sr.get(), etalon, SL);
	});

	suite.add("downcast", []() {
		utki::shared_ref<a0> sr = utki::make_shared<a1>(2);

		tst::check_eq(sr.get().a_0, 2, SL);
	});

	suite.add("const_downcast", []() {
		utki::shared_ref<const a0> sr = utki::make_shared<a1>(2);

		tst::check_eq(sr.get().a_0, 2, SL);
	});

	suite.add("dynamic_reference_cast", []() {
		utki::shared_ref<a0> sr = utki::make_shared<a1>(3);

		auto sr1 = utki::dynamic_reference_cast<a1>(sr);

		tst::check_eq(sr1.get().a_0, 3, SL);
	});

	suite.add("dynamic_reference_cast_bad_cast", []() {
		utki::shared_ref<a0> sr = utki::make_shared<a1>(3);

		bool bad_cast_thrown = false;
		try {
			auto sr1 = utki::dynamic_reference_cast<std::string>(sr);
			tst::check(false, SL);
		} catch (std::bad_cast&) {
			bad_cast_thrown = true;
		}
		tst::check(bad_cast_thrown, SL);
	});

	suite.add("move_constructor", []() {
		auto a = utki::make_shared<a1>(13);

		tst::check_eq(a.get().a_0, 13, SL);

		decltype(a) b(std::move(a));

		// 'a' should remain invalid
		// NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
		tst::check(!a.to_shared_ptr(), SL); // NOLINT(bugprone-use-after-move)

		tst::check_eq(b.get().a_0, 13, SL);
	});

	suite.add("move_assignment", []() {
		auto a = utki::make_shared<a1>(13);

		tst::check_eq(a.get().a_0, 13, SL);

		auto b = utki::make_shared<a1>(10);

		tst::check_eq(b.get().a_0, 10, SL);

		b = std::move(a);

		// 'a' should remain invalid
		// NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
		tst::check(!a.to_shared_ptr(), SL); // NOLINT(bugprone-use-after-move)

		tst::check_eq(b.get().a_0, 13, SL);
	});

	suite.add("move_constructible_and_move_assignable", []() {
		std::vector<utki::shared_ref<a0>> vec =
			{utki::make_shared<a1>(1), utki::make_shared<a1>(2), utki::make_shared<a1>(3)};

		tst::check_eq(vec[0].get().a_0, 1, SL);
		tst::check_eq(vec[1].get().a_0, 2, SL);
		tst::check_eq(vec[2].get().a_0, 3, SL);

		// std::rotate requires container element type to be
		// MoveAssignable and MoveConstructible
#if CFG_CPP >= 20
		std::ranges::rotate(vec.begin(), std::next(vec.begin()), vec.end());
#else
		std::rotate(vec.begin(), std::next(vec.begin()), vec.end());
#endif

		tst::check_eq(vec[0].get().a_0, 2, SL);
		tst::check_eq(vec[1].get().a_0, 3, SL);
		tst::check_eq(vec[2].get().a_0, 1, SL);
	});

	suite.add("operator_shared_ptr", []() {
		auto a = utki::make_shared<a1>(13);

		utki::shared_ref<const a1> ca = a;

		std::shared_ptr<a1> p;
		std::shared_ptr<const a1> cp1;
		std::shared_ptr<const a1> cp2;

		tst::check(a.to_shared_ptr(), SL);
		tst::check(ca.to_shared_ptr(), SL);
		tst::check(!p, SL);
		tst::check(!cp1, SL);

		p = a;
		cp1 = a;
		cp2 = ca;

		tst::check(p, SL);
		tst::check_eq(p, a.to_shared_ptr(), SL);

		tst::check(cp1, SL);
		tst::check_eq(cp1, ca.to_shared_ptr(), SL);

		tst::check(cp2, SL);
		tst::check_eq(cp2, ca.to_shared_ptr(), SL);
	});
});

} // namespace
