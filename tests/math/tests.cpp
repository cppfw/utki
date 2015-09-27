#include "../../src/ting/debug.hpp"
#include "../../src/ting/math.hpp"

#include "tests.hpp"



using namespace ting;



namespace TestBasicMathStuff{


void Run(){
	ASSERT_ALWAYS(math::Sin((long double)(0)) == 0)
	ASSERT_ALWAYS(math::Abs(math::Sin(math::DPi<long double>() / 2) - 1) < 0.00001)
	ASSERT_ALWAYS(math::Abs(math::Sin(math::DPi<long double>())) < 0.00001)
	ASSERT_ALWAYS(math::Abs(math::Sin(math::DPi<long double>() * 3 / 2) + 1) < 0.00001)

	ASSERT_ALWAYS(math::Cos((long double)(0)) == 1)
	ASSERT_ALWAYS(math::Abs(math::Cos(math::DPi<long double>() / 2)) < 0.00001)
	ASSERT_ALWAYS(math::Abs(math::Cos(math::DPi<long double>()) + 1) < 0.00001)
	ASSERT_ALWAYS(math::Abs(math::Cos(math::DPi<long double>() * 3 / 2)) < 0.00001)

	ASSERT_ALWAYS(math::Exp((long double)(0)) == 1)
	ASSERT_ALWAYS(math::Abs(math::Exp(math::DLnOf2<long double>()) - 2) < 0.00001)

	ASSERT_ALWAYS(math::Ln((long double)(1)) == 0)
	ASSERT_ALWAYS(math::Abs(math::Ln((long double)(2)) - math::DLnOf2<long double>()) < 0.00001)
}

}//~namespace
