#include "tests.hpp"

int main(int argc, char *argv[]){
	TestStaticBufferCopyConstructor::Run();
	TestStaticBufferOperatorEquals::Run();
	TestBufferConstCast::Run();
	testOutputToStream::run();
	test_make_span::run();
	return 0;
}
