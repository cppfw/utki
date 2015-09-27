#pragma once

#include "../../src/ting/debug.hpp"

#include "tests.hpp"



inline void TestTingMath(){
	TestBasicMathStuff::Run();

	TRACE_ALWAYS(<<"[PASSED]: math test"<<std::endl)
}
