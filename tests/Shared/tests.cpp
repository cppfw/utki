#include "../../src/utki/debug.hpp"
#include "../../src/utki/Shared.hpp"

#include "tests.hpp"



namespace TestBasicTingShared{

class TestClass : public utki::Shared{
public:
	int a = 4;
	
	TestClass(){
		
	}
	
	TestClass(int i) : a(i) {}
	
	std::shared_ptr<TestClass> getPtr(){
		return this->sharedFromThis(this);
	}
};



void Run(){
	std::shared_ptr<TestClass> p1 = utki::makeShared<TestClass>();
	
	std::shared_ptr<TestClass> p2 = utki::makeShared<TestClass>(21);
	
	ASSERT_ALWAYS(p1->a == 4)
	ASSERT_ALWAYS(p2->a == 21)
	
	ASSERT_ALWAYS(p2->getPtr().operator->() == p2.operator->())
}


}//~namespace
