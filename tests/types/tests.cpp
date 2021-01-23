#include "../../src/utki/types.hpp"
#include "../../src/utki/debug.hpp"

#include "tests.hpp"

#include <vector>
#include <list>
#include <map>

namespace{
void test_func(int){}

struct test_class{
	void func1(){}
	void func2(int){}
	void func3(int, char*){}
};

void test_get_num_arguments(){
#if __cplusplus >= 201703L
	ASSERT_ALWAYS(utki::get_num_arguments([](){}) == 0)
	ASSERT_ALWAYS(utki::get_num_arguments([&](int, float){}) == 2)
	ASSERT_ALWAYS(utki::get_num_arguments(test_func) == 1)
	ASSERT_ALWAYS(utki::get_num_arguments(&test_class::func1) == 0)
	ASSERT_ALWAYS(utki::get_num_arguments(&test_class::func2) == 1)
	ASSERT_ALWAYS(utki::get_num_arguments(&test_class::func3) == 2)
#endif
}
}

void test_utki_types(){
	test_get_num_arguments();
}
