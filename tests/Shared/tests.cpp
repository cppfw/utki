#include "../../src/utki/debug.hpp"
#include "../../src/utki/shared.hpp"

#include "tests.hpp"



namespace TestBasicTingShared{

class TestClass : public utki::shared{
public:
	int a = 4;
	
	TestClass(){
		
	}
	
	TestClass(int i) : a(i) {}
	
	std::shared_ptr<TestClass> getPtr(){
		return this->shared_from_this(this);
	}
};



void Run(){
	std::shared_ptr<TestClass> p1 = std::make_shared<TestClass>();
	
	std::shared_ptr<TestClass> p2 = std::make_shared<TestClass>(21);
	
	ASSERT_ALWAYS(p1->a == 4)
	ASSERT_ALWAYS(p2->a == 21)
	
	ASSERT_ALWAYS(p2->getPtr().operator->() == p2.operator->())
}


}
