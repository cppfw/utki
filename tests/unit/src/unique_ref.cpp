#include <algorithm>
#include <set>

#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/unique_ref.hpp>
#include <utki/utility.hpp>

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

	bool operator<(const a0& r) const noexcept
	{
		return this < &r;
	}

	bool operator==(const a0& r) const noexcept
	{
		return this == &r;
	}
};

struct a1 : public a0 {
	int a_1;

	a1(int a) :
		a0(a),
		a_1(a)
	{}
};
} // namespace

namespace {
const tst::set set("unique_ref", [](tst::suite& suite) {
	static_assert(
		!std::is_constructible_v<utki::unique_ref<std::string>>,
		"unique_ref must not be default constructible"
		" because unique_ref cannot be nullptr"
	);

	static_assert(
		!std::is_convertible_v<std::unique_ptr<std::string>, utki::unique_ref<std::string>>,
		"unique_ptr must not be convertible to unique_ref"
		" because shared_ptr can be nullptr, but unique_ref cannot"
	);

	static_assert(
		!std::is_copy_constructible_v<utki::unique_ref<std::string>>,
		"unique_ref must not be copy constructible"
	);

	static_assert(std::is_move_constructible_v<utki::unique_ref<std::string>>, "unique_ref must be move constructible");

	suite.add("constructor__unique_ptr", []() {
		auto up = std::make_unique<a1>(3);

		tst::check_eq(up->a_1, 3, SL);

		auto ur = utki::unique_ref(std::move(up));

		tst::check(!up, SL);
		tst::check_eq(ur.get().a_1, 3, SL);
	});

	suite.add("get", []() {
		utki::unique_ref<std::string> ur = utki::make_unique<std::string>(etalon);

		tst::check_eq(ur.get(), etalon, SL);
	});

	suite.add("const_get", []() {
		const utki::unique_ref<std::string> ur = utki::make_unique<std::string>(etalon);

		tst::check_eq(ur.get(), etalon, SL);

		ur.get().append("!");

		tst::check_eq(etalon + "!", ur.get(), SL);
	});

	suite.add("operator_convert_to_reference", []() {
		utki::unique_ref<std::string> ur = utki::make_unique<std::string>(etalon);

		[](std::string& s) {
			tst::check_eq(s, etalon, SL);
		}(ur);

		[](const std::string& s) {
			tst::check_eq(s, etalon, SL);
		}(ur);
	});

	suite.add("operator_convert_to_reference_const", []() {
		utki::unique_ref<const std::string> ur = utki::make_unique<std::string>(etalon);

		[](const std::string& s) {
			tst::check_eq(s, etalon, SL);
		}(ur);
	});

	suite.add("const_autocast", []() {
		utki::unique_ref<const std::string> sr = utki::make_unique<std::string>(etalon);

		tst::check_eq(sr.get(), etalon, SL);
	});

	suite.add("downcast", []() {
		utki::unique_ref<a0> sr = utki::make_unique<a1>(2);

		tst::check_eq(sr.get().a_0, 2, SL);
	});

	suite.add("const_downcast", []() {
		utki::unique_ref<const a0> sr = utki::make_unique<a1>(2);

		tst::check_eq(sr.get().a_0, 2, SL);
	});

	suite.add("move_constructor", []() {
		auto a = utki::make_unique<a1>(13);

		tst::check_eq(a.get().a_0, 13, SL);

		decltype(a) b(std::move(a));

		tst::check_eq(b.get().a_0, 13, SL);
	});

	suite.add("move_assignment", []() {
		auto a = utki::make_unique<a1>(13);

		tst::check_eq(a.get().a_0, 13, SL);

		auto b = utki::make_unique<a1>(10);

		tst::check_eq(b.get().a_0, 10, SL);

		b = std::move(a);

		tst::check_eq(b.get().a_0, 13, SL);
	});

	suite.add("move_constructible_and_move_assignable", []() {
		std::vector<utki::unique_ref<a0>> vec;
		vec.emplace_back(utki::make_unique<a1>(1));
		vec.emplace_back(utki::make_unique<a1>(2));
		vec.emplace_back(utki::make_unique<a1>(3));

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

	suite.add("use_with_std_set", []() {
		std::set<utki::unique_ref<a0>> set;

		auto v1 = utki::make_unique<a0>(1);
		auto v2 = utki::make_unique<a0>(2);
		auto v3 = utki::make_unique<a0>(3);

		auto& v1r = v1.get();
		auto& v2r = v2.get();
		auto& v3r = v3.get();

		set.insert(std::move(v1));
		set.insert(std::move(v2));

		tst::check_eq(set.size(), size_t(2), SL);

		tst::check(!utki::contains(set, v3), SL);

		tst::check(utki::contains(set, v1r), SL);
		tst::check(utki::contains(set, v2r), SL);
		tst::check(!utki::contains(set, v3r), SL);
	});

	suite.add("to_unique_ptr", []() {
		constexpr auto str = "Hello world!";
		utki::unique_ref<std::string> us = utki::make_unique<std::string>(str);

		tst::check_eq(us.get(), std::string(str), SL);

		auto usptr = std::move(us).to_unique_ptr();

		tst::check(usptr, SL);
		tst::check_eq(*usptr, std::string(str), SL);
	});
});
} // namespace