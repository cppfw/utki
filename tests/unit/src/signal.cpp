#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/signal.hpp>

#include <type_traits>

namespace {
const tst::set set("signal", [](tst::suite& suite) {
	suite.add("basic__copyable_and_movable", []() {
		static_assert(std::is_copy_constructible_v<utki::signal<>>, "signal must be copy constructible");
		static_assert(std::is_copy_assignable_v<utki::signal<>>, "signal must be copy assignable");
		static_assert(std::is_move_constructible_v<utki::signal<>>, "signal must be move constructible");
		static_assert(std::is_move_assignable_v<utki::signal<>>, "signal must be move assignable");

		unsigned num = 0;
		utki::signal<> orig;
		auto id = orig.connect([&]() {
			++num;
		});

		// copy construction
		utki::signal<> copy = orig;
		tst::check_eq(copy.size(), size_t(1), SL);

		// copy assignment
		utki::signal<> copy_assign;
		copy_assign = orig;
		tst::check_eq(copy_assign.size(), size_t(1), SL);

		// copies are independent: disconnecting on the original does not affect the copies
		orig.disconnect(id);
		tst::check(orig.empty(), SL);
		tst::check(!copy.empty(), SL);
		tst::check(!copy_assign.empty(), SL);

		copy.emit();
		copy_assign.emit();
		tst::check_eq(num, unsigned(2), SL);

		// move construction: callbacks are transferred, the source is left empty
		utki::signal<> moved = std::move(copy);
		tst::check(copy.empty(), SL);
		tst::check_eq(moved.size(), size_t(1), SL);

		moved.emit();
		tst::check_eq(num, unsigned(3), SL);

		// move assignment
		utki::signal<> moved_assign;
		moved_assign = std::move(moved);
		tst::check(moved.empty(), SL);
		tst::check_eq(moved_assign.size(), size_t(1), SL);

		moved_assign.emit();
		tst::check_eq(num, unsigned(4), SL);
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