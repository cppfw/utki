#include "tests.hpp"

int main(int argc, char *argv[]){
	TestStaticBufferCopyConstructor::Run();
	TestStaticBufferOperatorEquals::Run();
	TestBufferConstCast::Run();
	testOutputToStream::run();
	
	return 0;
}
