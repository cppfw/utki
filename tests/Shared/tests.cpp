#include "../../src/ting/debug.hpp"
#include "../../src/ting/Shared.hpp"

#include "tests.hpp"



namespace TestBasicTingShared{

class TestClass : public ting::Shared{
public:
	int a = 4;
	
	TestClass(){
		
	}
	
	TestClass(int i) : a(i) {}
	
	std::shared_ptr<TestClass> getPtr(){
		return this->SharedFromThis(this);
	}
};



void Run(){
	std::shared_ptr<TestClass> p1 = ting::New<TestClass>();
	
	std::shared_ptr<TestClass> p2 = ting::New<TestClass>(21);
	
	ASSERT_ALWAYS(p1->a == 4)
	ASSERT_ALWAYS(p2->a == 21)
	
	ASSERT_ALWAYS(p2->getPtr().operator->() == p2.operator->())
}


}//~namespace
