#include <type_traits>

#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/signal.hpp>

namespace {
const tst::set set("signal", [](tst::suite& suite) {
	suite.add("basic__copy_construct", []() {
		static_assert(std::is_copy_constructible_v<utki::signal<>>, "signal must be copy constructible");

		unsigned num = 0;
		utki::signal<> orig;
		auto id = orig.connect([&]() {
			++num;
		});

		utki::signal<> copy = orig;
		tst::check_eq(copy.size(), size_t(1), SL);

		// copies are independent: disconnecting on the original does not affect the copy
		orig.disconnect(id);
		tst::check(orig.empty(), SL);
		tst::check(!copy.empty(), SL);

		copy.emit();
		tst::check_eq(num, unsigned(1), SL);
	});

	suite.add("basic__copy_assign", []() {
		static_assert(std::is_copy_assignable_v<utki::signal<>>, "signal must be copy assignable");

		unsigned num = 0;
		utki::signal<> orig;
		auto id = orig.connect([&]() {
			++num;
		});

		utki::signal<> copy;
		copy = orig;
		tst::check_eq(copy.size(), size_t(1), SL);

		// the copy is independent: disconnecting on the original does not affect it
		orig.disconnect(id);
		tst::check(orig.empty(), SL);
		tst::check(!copy.empty(), SL);

		copy.emit();
		tst::check_eq(num, unsigned(1), SL);
	});

	suite.add("basic__move_construct", []() {
		static_assert(std::is_move_constructible_v<utki::signal<>>, "signal must be move constructible");

		unsigned num = 0;
		utki::signal<> src;
		src.connect([&]() {
			++num;
		});

		utki::signal<> dst = std::move(src);

		// callbacks are transferred, the source is left empty
		// NOLINTNEXTLINE(bugprone-use-after-move, "checking the post-move state of src is the point of this test")
		tst::check(src.empty(), SL);
		tst::check_eq(dst.size(), size_t(1), SL);

		dst.emit();
		tst::check_eq(num, unsigned(1), SL);
	});

	suite.add("basic__move_assign", []() {
		static_assert(std::is_move_assignable_v<utki::signal<>>, "signal must be move assignable");

		unsigned num = 0;
		utki::signal<> src;
		src.connect([&]() {
			++num;
		});

		utki::signal<> dst;
		dst = std::move(src);

		// callbacks are transferred, the source is left empty
		// NOLINTNEXTLINE(bugprone-use-after-move, "checking the post-move state of src is the point of this test")
		tst::check(src.empty(), SL);
		tst::check_eq(dst.size(), size_t(1), SL);

		dst.emit();
		tst::check_eq(num, unsigned(1), SL);
	});

	suite.add("basic__no_arguments", []() {
		utki::signal<> sig;

		tst::check(sig.empty(), SL);
		tst::check_eq(sig.size(), size_t(0), SL);

		sig.emit();

		tst::check(sig.empty(), SL);
		tst::check_eq(sig.size(), size_t(0), SL);

		unsigned num = 0;

		auto id1 = sig.connect([&]() {
			++num;
		});

		tst::check(!sig.empty(), SL);
		tst::check_eq(sig.size(), size_t(1), SL);
		tst::check_eq(num, unsigned(0), SL);

		sig.emit();

		tst::check(!sig.empty(), SL);
		tst::check_eq(sig.size(), size_t(1), SL);
		tst::check_eq(num, unsigned(1), SL);

		auto id2 = sig.connect([&]() {
			num += 3;
		});

		tst::check(id1 != id2, SL);

		tst::check(!sig.empty(), SL);
		tst::check_eq(sig.size(), size_t(2), SL);
		tst::check_eq(num, unsigned(1), SL);

		sig.emit();

		tst::check(!sig.empty(), SL);
		tst::check_eq(sig.size(), size_t(2), SL);
		tst::check_eq(num, unsigned(5), SL);

		sig.disconnect(id1);

		tst::check(!sig.empty(), SL);
		tst::check_eq(sig.size(), size_t(1), SL);
		tst::check_eq(num, unsigned(5), SL);

		sig.disconnect(id2);
		tst::check(sig.empty(), SL);
		tst::check_eq(sig.size(), size_t(0), SL);
	});
});
} // namespace