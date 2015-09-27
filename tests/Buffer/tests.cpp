#include "../../src/ting/Buffer.hpp"

#include "tests.hpp"


namespace TestStaticBufferCopyConstructor{

class TestClass{
public:
	int a;

	TestClass() :
			a(0)
	{}

	TestClass(const TestClass& c){
		this->a = c.a + 1;
	}

	TestClass& operator=(const TestClass& tc){
		return *this;
	}
};

void Run(){
	typedef std::array<TestClass, 20> T_TestStaticBuffer;
	T_TestStaticBuffer arr;
	T_TestStaticBuffer brr(arr);

	ASSERT_ALWAYS(arr.size() == brr.size())
	for(size_t i = 0; i < arr.size(); ++i){
		ASSERT_ALWAYS(arr[i].a == 0)
		ASSERT_ALWAYS(brr[i].a == 1)
	}
}

}//~namespace



namespace TestStaticBufferOperatorEquals{

class TestClass{
public:
	unsigned id;
	int a;

	TestClass() :
			a(0)
	{}

	TestClass(const TestClass& tc){
		this->a = tc.a + 1;
	}
};

void Run(){
	typedef std::array<TestClass, 20> T_TestStaticBuffer;
	T_TestStaticBuffer arr;

	for(size_t i = 0; i < arr.size(); ++i){
		arr[i].id = i;
	}

	T_TestStaticBuffer brr;
	TestClass* oldArrBegin = arr.begin();
	TestClass* oldBrrBegin = brr.begin();
	ASSERT_ALWAYS(brr.begin() != arr.begin())
	
	brr = arr;
	
	ASSERT_ALWAYS(brr.begin() != arr.begin())
	ASSERT_ALWAYS(arr.begin() == oldArrBegin)
	ASSERT_ALWAYS(brr.begin() == oldBrrBegin)

	ASSERT_ALWAYS(arr.size() == brr.size())
	for(size_t i = 0; i < arr.size(); ++i){
		ASSERT_ALWAYS(arr[i].a == 0)
		ASSERT_INFO_ALWAYS(brr[i].a == 0, "brr[i].a = " << brr[i].a)
		ASSERT_INFO_ALWAYS(arr[i].id == brr[i].id, "arr[i].id = " << arr[i].id << " brr[i].id = " << brr[i].id)
		ASSERT_ALWAYS(arr[i].id == i)
	}
}

}//~namespace



namespace TestBufferConstCast{

class TestClass{
public:
	int a;

	TestClass() :
			a(0)
	{}
};


int Func2(const ting::Buffer<TestClass> buf){
	if(buf.size() == 0){
		return 0;
	}
	
	return buf[0].a;
}

void Run(){
	{
		std::array<TestClass, 20> buf;
		
//		Func(buf);
		Func2(buf);
	}
	
	{
		std::vector<TestClass> buf(20);
		
//		Func(buf);
		Func2(buf);
	}
	
//	Func(ting::Buffer<TestClass>(0, 0));
	Func2(ting::Buffer<TestClass>(0, 0));
}

}//~namespace
