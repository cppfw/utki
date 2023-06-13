#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/math.hpp>

namespace {
const tst::set set("math", [](tst::suite& suite) {
	suite.add("basic", []() {
		static const auto epsilon = 0.00001;
		static const auto two_pi_degrees = 180.0;

		tst::check_eq(std::sin((long double)(0)), (long double)(0), SL);
		tst::check_lt(std::abs(std::sin(utki::pi<long double>() / 2) - 1), (long double)(epsilon), SL);
		tst::check_lt(std::abs(std::sin(utki::pi<long double>())), (long double)(epsilon), SL);
		tst::check_lt(std::abs(std::sin(utki::pi<long double>() * 3 / 2) + 1), (long double)(epsilon), SL);

		tst::check_eq(std::cos((long double)(0)), (long double)(1), SL);
		tst::check_lt(std::abs(std::cos(utki::pi<long double>() / 2)), (long double)(epsilon), SL);
		tst::check_lt(std::abs(std::cos(utki::pi<long double>()) + 1), (long double)(epsilon), SL);
		tst::check_lt(std::abs(std::cos(utki::pi<long double>() * 3 / 2)), (long double)(epsilon), SL);

		tst::check_eq(std::exp((long double)(0)), (long double)(1), SL);
		tst::check_lt(std::abs(std::exp(utki::log_2<long double>()) - 2), (long double)(epsilon), SL);

		tst::check_eq(std::log((long double)(1)), (long double)(0), SL);
		tst::check_lt(std::abs(std::log((long double)(2)) - utki::log_2<long double>()), (long double)(epsilon), SL);

		tst::check_eq(utki::rad_to_deg(utki::pi<float>()), float(two_pi_degrees), SL);
		tst::check_eq(utki::deg_to_rad(float(two_pi_degrees)), utki::pi<float>(), SL);
	});
});
} // namespace