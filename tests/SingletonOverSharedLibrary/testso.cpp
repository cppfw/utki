#include "../../src/utki/debug.hpp"

#include "testso.hpp_"
#include "TestSingleton.hpp_"

int& GetA(){
	ASSERT_ALWAYS(TestSingleton::is_created())
	return TestSingleton::inst().a;
}

void IncA(){
	ASSERT_ALWAYS(TestSingleton::is_created())
	++(TestSingleton::inst().a);
}

void PrintA(){
	ASSERT_ALWAYS(TestSingleton::is_created())
	TRACE_ALWAYS(<< "PrintA(): a = " << TestSingleton::inst().a << std::endl)
}
