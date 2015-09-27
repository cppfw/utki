#include "../../src/utki/debug.hpp"
#include "../../src/utki/math.hpp"

#include "tests.hpp"




namespace TestBasicMathStuff{


void Run(){
	ASSERT_ALWAYS(sin((long double)(0)) == 0)
	ASSERT_ALWAYS(abs(sin(utki::pi<long double>() / 2) - 1) < 0.00001)
	ASSERT_ALWAYS(abs(sin(utki::pi<long double>())) < 0.00001)
	ASSERT_ALWAYS(abs(sin(utki::pi<long double>() * 3 / 2) + 1) < 0.00001)

	ASSERT_ALWAYS(cos((long double)(0)) == 1)
	ASSERT_ALWAYS(abs(cos(utki::pi<long double>() / 2)) < 0.00001)
	ASSERT_ALWAYS(abs(cos(utki::pi<long double>()) + 1) < 0.00001)
	ASSERT_ALWAYS(abs(cos(utki::pi<long double>() * 3 / 2)) < 0.00001)

	ASSERT_ALWAYS(exp((long double)(0)) == 1)
	ASSERT_ALWAYS(abs(exp(utki::logOf2<long double>()) - 2) < 0.00001)

	ASSERT_ALWAYS(log((long double)(1)) == 0)
	ASSERT_ALWAYS(abs(log((long double)(2)) - utki::logOf2<long double>()) < 0.00001)
}

}//~namespace
