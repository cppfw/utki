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
		return utki::make_shared_from(*this);
	}
};



void run(){
	// basic test
	{
		std::shared_ptr<TestClass> p1 = std::make_shared<TestClass>();
		
		std::shared_ptr<TestClass> p2 = std::make_shared<TestClass>(21);
		
		utki::assert(p1->a == 4, SL);
		utki::assert(p2->a == 21, SL);
		
		utki::assert(p2->getPtr().operator->() == p2.operator->(), SL);
	}

	// test make_shared_from
	{
		auto o = std::make_shared<TestClass>();
		utki::assert(o, SL);

		auto sft = utki::make_shared_from(*o);
		utki::assert(sft, SL);
		utki::assert(sft->a == 4, SL);
	}

	// test make_weak_from
	{
		auto o = std::make_shared<TestClass>();
		utki::assert(o, SL);

		auto sft = utki::make_weak_from(*o);
		utki::assert(sft.lock(), SL);
		utki::assert(sft.lock()->a == 4, SL);
	}
}


}
