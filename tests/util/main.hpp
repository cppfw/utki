#pragma once

#include "../../src/utki/debug.hpp"

#include "tests.hpp"


inline void TestUtkiUtil(){
	TestSerialization::Run();
	TestScopeExit::Run();
	testFlipMap::run();

#if __cplusplus >= 201703L
	test_variant_get_index::run();
#endif
}
