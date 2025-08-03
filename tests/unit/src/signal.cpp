#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/signal.hpp>

namespace {
const tst::set set("signal", [](tst::suite& suite) {
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