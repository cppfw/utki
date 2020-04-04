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



void run(){
	// basic test
	{
		std::shared_ptr<TestClass> p1 = std::make_shared<TestClass>();
		
		std::shared_ptr<TestClass> p2 = std::make_shared<TestClass>(21);
		
		ASSERT_ALWAYS(p1->a == 4)
		ASSERT_ALWAYS(p2->a == 21)
		
		ASSERT_ALWAYS(p2->getPtr().operator->() == p2.operator->())
	}

	// test make_shared_from_this
	{
		auto o = std::make_shared<TestClass>();
		ASSERT_ALWAYS(o)

		auto sft = utki::make_shared_from_this(*o);
		ASSERT_ALWAYS(sft)
		ASSERT_ALWAYS(sft->a == 4)
	}
}


}
