#pragma once

#include "../../src/ting/debug.hpp"

#include "tests.hpp"



inline void TestTingFlags(){
	TestFlags::Run();

	TRACE_ALWAYS(<<"[PASSED]: Flags test"<<std::endl)
}
