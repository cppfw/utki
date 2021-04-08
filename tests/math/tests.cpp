#include "../../src/utki/debug.hpp"
#include "../../src/utki/math.hpp"

#include "tests.hpp"

namespace TestBasicMathStuff{

void Run(){
	utki::assert(std::sin((long double)(0)) == 0, SL);
	utki::assert(std::abs(std::sin(utki::pi<long double>() / 2) - 1) < 0.00001, SL);
	utki::assert(std::abs(std::sin(utki::pi<long double>())) < 0.00001, SL);
	utki::assert(std::abs(std::sin(utki::pi<long double>() * 3 / 2) + 1) < 0.00001, SL);

	utki::assert(std::cos((long double)(0)) == 1, SL);
	utki::assert(std::abs(std::cos(utki::pi<long double>() / 2)) < 0.00001, SL);
	utki::assert(std::abs(std::cos(utki::pi<long double>()) + 1) < 0.00001, SL);
	utki::assert(std::abs(std::cos(utki::pi<long double>() * 3 / 2)) < 0.00001, SL);

	utki::assert(std::exp((long double)(0)) == 1, SL);
	utki::assert(std::abs(std::exp(utki::log_2<long double>()) - 2) < 0.00001, SL);

	utki::assert(std::log((long double)(1)) == 0, SL);
	utki::assert(std::abs(std::log((long double)(2)) - utki::log_2<long double>()) < 0.00001, SL);

	utki::assert(utki::rad_to_deg(utki::pi<float>()) == 180.0, SL);
	utki::assert(utki::deg_to_rad(float(180)) == utki::pi<float>(), SL);
}

}
