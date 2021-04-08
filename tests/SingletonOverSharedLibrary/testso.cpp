#include "../../src/utki/debug.hpp"

#include "testso.hpp_"
#include "TestSingleton.hpp_"

int& GetA(){
	utki::assert(TestSingleton::is_created(), SL);
	return TestSingleton::inst().a;
}

void IncA(){
	utki::assert(TestSingleton::is_created(), SL);
	++(TestSingleton::inst().a);
}

void PrintA(){
	utki::assert(TestSingleton::is_created(), SL);
	utki::log([](auto&o){o << "PrintA(): a = " << TestSingleton::inst().a << std::endl;});
}
