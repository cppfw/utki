#pragma once

#include "../../src/utki/debug.hpp"

#include "tests.hpp"


inline void TestUtkiUtil(){
	TestSerialization::Run();
	TestScopeExit::Run();
	testFlipMap::run();
}
