#include "../../src/utki/debug.hpp"

#include "TestSingleton.hpp_"
#include "testso.hpp"



int main(int argc, char *argv[]){
//	TRACE_ALWAYS(<< "Started" << std::endl)

	TestSingleton ts;

//	TRACE_ALWAYS(<< "32 a = " << TestSingleton::Inst().a << std::endl)

	ASSERT_ALWAYS(TestSingleton::inst().a == 32)

	++TestSingleton::inst().a;

	ASSERT_ALWAYS(TestSingleton::inst().a == 33)

	IncA();

	ASSERT_ALWAYS(TestSingleton::inst().a == 34)
	ASSERT_ALWAYS(ts.a == 34)

	TestSingleton::inst().a = 101;

	ASSERT_ALWAYS(GetA() == 101)

	GetA() = 145;

	ASSERT_ALWAYS(ts.a == 145)
	ASSERT_ALWAYS(TestSingleton::inst().a == 145)

	TRACE_ALWAYS(<< "[PASSED]: Singleton over shared library test" << std::endl)

	return 0;
}
