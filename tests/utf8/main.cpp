#include "tests.hpp"

int main(int argc, char *argv[]){
	TestSimple::Run();
	testUtf32ToUtf8::run();
	
	return 0;
}
