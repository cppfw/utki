#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/math.hpp>

namespace {
tst::set set("math", [](tst::suite& suite) {
	suite.add("basic", []() {
		tst::check_eq(std::sin((long double)(0)), (long double)(0), SL);
		tst::check_lt(std::abs(std::sin(utki::pi<long double>() / 2) - 1), (long double)(0.00001), SL);
		tst::check_lt(std::abs(std::sin(utki::pi<long double>())), (long double)(0.00001), SL);
		tst::check_lt(std::abs(std::sin(utki::pi<long double>() * 3 / 2) + 1), (long double)(0.00001), SL);

		tst::check_eq(std::cos((long double)(0)), (long double)(1), SL);
		tst::check_lt(std::abs(std::cos(utki::pi<long double>() / 2)), (long double)(0.00001), SL);
		tst::check_lt(std::abs(std::cos(utki::pi<long double>()) + 1), (long double)(0.00001), SL);
		tst::check_lt(std::abs(std::cos(utki::pi<long double>() * 3 / 2)), (long double)(0.00001), SL);

		tst::check_eq(std::exp((long double)(0)), (long double)(1), SL);
		tst::check_lt(std::abs(std::exp(utki::log_2<long double>()) - 2), (long double)(0.00001), SL);

		tst::check_eq(std::log((long double)(1)), (long double)(0), SL);
		tst::check_lt(std::abs(std::log((long double)(2)) - utki::log_2<long double>()), (long double)(0.00001), SL);

		tst::check_eq(utki::rad_to_deg(utki::pi<float>()), (float)(180.0), SL);
		tst::check_eq(utki::deg_to_rad(float(180)), utki::pi<float>(), SL);
	});
});
} // namespace