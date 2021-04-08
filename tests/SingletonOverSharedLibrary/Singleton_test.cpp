#include "../../src/utki/debug.hpp"

#include "TestSingleton.hpp_"
#include "testso.hpp_"



int main(int argc, char *argv[]){
//	TRACE_ALWAYS(<< "Started" << std::endl)

	TestSingleton ts;

//	TRACE_ALWAYS(<< "32 a = " << TestSingleton::Inst().a << std::endl)

	utki::assert(TestSingleton::inst().a == 32, SL);

	++TestSingleton::inst().a;

	utki::assert(TestSingleton::inst().a == 33, SL);

	IncA();

	utki::assert(TestSingleton::inst().a == 34, SL);
	utki::assert(ts.a == 34, SL);

	TestSingleton::inst().a = 101;

	utki::assert(GetA() == 101, SL);

	GetA() = 145;

	utki::assert(ts.a == 145, SL);
	utki::assert(TestSingleton::inst().a == 145, SL);

	return 0;
}
