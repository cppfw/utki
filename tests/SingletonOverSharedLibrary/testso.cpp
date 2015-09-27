#include "../../src/ting/debug.hpp"

#include "testso.hpp"
#include "TestSingleton.hpp"



int& GetA(){
	ASSERT_ALWAYS(TestSingleton::IsCreated())
	return TestSingleton::Inst().a;
}



void IncA(){
	ASSERT_ALWAYS(TestSingleton::IsCreated())
	++(TestSingleton::Inst().a);
}



void PrintA(){
	ASSERT_ALWAYS(TestSingleton::IsCreated())
	TRACE_ALWAYS(<< "PrintA(): a = " << TestSingleton::Inst().a << std::endl)
}
