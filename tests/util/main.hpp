#pragma once

#include "../../src/utki/debug.hpp"

#include "tests.hpp"


inline void TestTingUtil(){
	TestSerialization::Run();
	TestScopeExit::Run();
}
