#include "testso.hpp_"
#include "test_singleton.hpp_"

#include "../../src/utki/debug.hpp"

int& get_a(){
	utki::assert(test_singleton::is_created(), SL);
	return test_singleton::inst().a;
}

void inc_a(){
	utki::assert(test_singleton::is_created(), SL);
	++(test_singleton::inst().a);
}

void print_a(){
	utki::assert(test_singleton::is_created(), SL);
	utki::log([](auto&o){o << "PrintA(): a = " << test_singleton::inst().a << std::endl;});
}
