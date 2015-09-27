#include <deque>
#include <memory>

#include "../../src/ting/debug.hpp"
#include "../../src/ting/PoolStored.hpp"

#include "tests.hpp"



namespace BasicPoolStoredTest{

class TestClass : public ting::PoolStored<TestClass, 2>{
public:
	int a;
	int b;
	
	char* ptr;
	
	TestClass* child;
	
	TestClass(){
//		TRACE(<<"TestClass(): constructed"<<std::endl)
	}
	~TestClass(){
//		TRACE(<<"~TestClass(): destructed"<<std::endl)
	}
};


void Run(){
	std::deque<std::unique_ptr<TestClass> > vec;

	for(unsigned i = 0; i < 10000; ++i){
		vec.push_back(std::unique_ptr<TestClass>(new TestClass()));
		vec.back()->a = int(i);
		vec.back()->b = -int(i);
	}

	for(unsigned i = 0; i < 50; ++i){
		vec.pop_front();
	}
	
	for(unsigned i = 0; i < 100; ++i){
		vec.push_back(std::unique_ptr<TestClass>(new TestClass()));
	}
	
	while(vec.size() != 0){
		vec.pop_back();
	}
}

}//~namespace
