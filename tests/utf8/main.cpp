#include "tests.hpp"

int main(int argc, char *argv[]){
	TestSimple::Run();
	testUtf32ToUtf8::run();
	test_utf8_to_utf32::run();

	return 0;
}
