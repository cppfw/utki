#include "test_singleton.hpp_"
#include "testso.hpp_"

#include "../../src/utki/debug.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

// NOLINTNEXTLINE(bugprone-exception-escape): global exceptions are not caught
int main(int argc, char *argv[]){
	test_singleton ts;

	utki::assert(test_singleton::inst().a == 32, SL);

	++test_singleton::inst().a;

	utki::assert(test_singleton::inst().a == 33, SL);

	inc_a();

	utki::assert(test_singleton::inst().a == 34, SL);
	utki::assert(ts.a == 34, SL);

	test_singleton::inst().a = 101;

	utki::assert(get_a() == 101, SL);

	get_a() = 145;

	utki::assert(ts.a == 145, SL);
	utki::assert(test_singleton::inst().a == 145, SL);

	return 0;
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
