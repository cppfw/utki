#include "../../src/utki/debug.hpp"
#include "../../src/utki/math.hpp"

#include "tests.hpp"




namespace TestBasicMathStuff{


void Run(){
	ASSERT_ALWAYS(std::sin((long double)(0)) == 0)
	ASSERT_ALWAYS(std::abs(std::sin(utki::pi<long double>() / 2) - 1) < 0.00001)
	ASSERT_ALWAYS(std::abs(std::sin(utki::pi<long double>())) < 0.00001)
	ASSERT_ALWAYS(std::abs(std::sin(utki::pi<long double>() * 3 / 2) + 1) < 0.00001)

	ASSERT_ALWAYS(std::cos((long double)(0)) == 1)
	ASSERT_ALWAYS(std::abs(std::cos(utki::pi<long double>() / 2)) < 0.00001)
	ASSERT_ALWAYS(std::abs(std::cos(utki::pi<long double>()) + 1) < 0.00001)
	ASSERT_ALWAYS(std::abs(std::cos(utki::pi<long double>() * 3 / 2)) < 0.00001)

	ASSERT_ALWAYS(std::exp((long double)(0)) == 1)
	ASSERT_ALWAYS(std::abs(std::exp(utki::logOf2<long double>()) - 2) < 0.00001)

	ASSERT_ALWAYS(std::log((long double)(1)) == 0)
	ASSERT_ALWAYS(std::abs(std::log((long double)(2)) - utki::logOf2<long double>()) < 0.00001)
}

}//~namespace
