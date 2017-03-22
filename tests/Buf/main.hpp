#pragma once

#include "../../src/utki/debug.hpp"

#include "tests.hpp"


inline void TestTingBuffer(){
	TestStaticBufferCopyConstructor::Run();
	TestStaticBufferOperatorEquals::Run();
	TestBufferConstCast::Run();
}
