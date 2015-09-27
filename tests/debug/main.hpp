#pragma once

#include "../../src/ting/debug.hpp"

#include "tests.hpp"


inline void TestTingDebug(){
	TestBasicDebugStuff::Run();
	
	TRACE_ALWAYS(<< "[PASSED]: debug test" << std::endl)	
}
