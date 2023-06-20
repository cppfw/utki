#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/math.hpp>

namespace {
const tst::set set("math", [](tst::suite& suite) {
	suite.add("basic", []() {
		constexpr auto epsilon = 0.00001;
		constexpr auto two_pi_degrees = 180.0;

		tst::check_eq(std::sin((long double)(0)), (long double)(0), SL);
		tst::check_lt(std::abs(std::sin((long double)(utki::pi) / 2) - 1), (long double)(epsilon), SL);
		tst::check_lt(std::abs(std::sin((long double)(utki::pi))), (long double)(epsilon), SL);
		tst::check_lt(std::abs(std::sin((long double)(utki::pi)*3 / 2) + 1), (long double)(epsilon), SL);

		tst::check_eq(std::cos((long double)(0)), (long double)(1), SL);
		tst::check_lt(std::abs(std::cos((long double)(utki::pi) / 2)), (long double)(epsilon), SL);
		tst::check_lt(std::abs(std::cos((long double)(utki::pi)) + 1), (long double)(epsilon), SL);
		tst::check_lt(std::abs(std::cos((long double)(utki::pi)*3 / 2)), (long double)(epsilon), SL);

		tst::check_eq(std::exp((long double)(0)), (long double)(1), SL);
		tst::check_lt(std::abs(std::exp((long double)(utki::log_2)) - 2), (long double)(epsilon), SL);

		tst::check_eq(std::log((long double)(1)), (long double)(0), SL);
		tst::check_lt(std::abs(std::log((long double)(2)) - (long double)(utki::log_2)), (long double)(epsilon), SL);

		tst::check_eq(utki::rad_to_deg(float(utki::pi)), float(two_pi_degrees), SL);
		tst::check_eq(utki::deg_to_rad(float(two_pi_degrees)), float(utki::pi), SL);
	});
});
} // namespace