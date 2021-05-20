#include "tests.hpp"

int main(int argc, char *argv[]){
	TestStaticBufferCopyConstructor::Run();
	TestStaticBufferOperatorEquals::Run();
	TestBufferConstCast::Run();
	testOutputToStream::run();
	test_make_span::run();
	test_subspan::run();
	test_make_vector::run();
	test_operator_equals::run();
	return 0;
}
