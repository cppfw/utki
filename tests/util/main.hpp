#pragma once

#include "../../src/ting/debug.hpp"

#include "tests.hpp"


inline void TestTingUtil(){
	TestSerialization::Run();
	TestScopeExit::Run();
	
	TRACE_ALWAYS(<< "[PASSED]: utils test" << std::endl)
}
