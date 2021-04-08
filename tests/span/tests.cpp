#include "../../src/utki/span.hpp"

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

}


namespace TestStaticBufferOperatorEquals{

class TestClass{
public:
	size_t id;
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
	TestClass* oldArrBegin = &*arr.begin();
	TestClass* oldBrrBegin = &*brr.begin();
	ASSERT_ALWAYS(&*brr.begin() != &*arr.begin())
	
	brr = arr;
	
	ASSERT_ALWAYS(&*brr.begin() != &*arr.begin())
	ASSERT_ALWAYS(&*arr.begin() == oldArrBegin)
	ASSERT_ALWAYS(&*brr.begin() == oldBrrBegin)

	ASSERT_ALWAYS(arr.size() == brr.size())
	for(size_t i = 0; i < arr.size(); ++i){
		ASSERT_ALWAYS(arr[i].a == 0)
		utki::assert(brr[i].a == 0, [&](auto&o){o << "brr[i].a = " << brr[i].a;}, SL);
		utki::assert(arr[i].id == brr[i].id, [&](auto&o){o << "arr[i].id = " << arr[i].id << " brr[i].id = " << brr[i].id;}, SL);
		ASSERT_ALWAYS(arr[i].id == i)
	}
}

}

namespace TestBufferConstCast{

class TestClass{
public:
	int a;

	TestClass() :
			a(0)
	{}
};


int Func2(utki::span<const TestClass> buf){
	if(buf.size() == 0){
		return 0;
	}
	
	return buf[0].a;
}

void Run(){
	{
		std::array<TestClass, 20> buf;
		
//		Func(buf);
		Func2(utki::make_span(buf));
	}
	
	{
		std::vector<TestClass> buf(20);
		
//		Func(buf);
		Func2(utki::make_span(buf));
	}
	
	Func2(utki::make_span<TestClass>(0, 0));
}

}

namespace testOutputToStream{
void run(){
	{
		std::array<char, 12> buf = {{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}};

		std::stringstream ss;

		ss << utki::make_span(buf);

		ASSERT_ALWAYS(ss.str() == "Hello world!")
	}
}
}

namespace test_make_span{
void run(){
	// test make_span(const basic_string)
	{
		std::string str = "Hello world!";

		auto s = utki::make_span(str);

		ASSERT_ALWAYS(s.size() == str.size())
		ASSERT_ALWAYS(s.data() == str.data())
	}

	// test make_span(const char*)
	{
		const char* str = "Hello world!";
		auto s = utki::make_span(str);

		ASSERT_ALWAYS(s.size() == strlen(str))
		ASSERT_ALWAYS(s.data() == str)
	}
}
}

namespace test_subspan{
void run(){
	std::string str = "Hello world!";

	auto s = utki::make_span(str);

	ASSERT_ALWAYS(s.size() == str.size())
	ASSERT_ALWAYS(s.data() == str.data())
	
	// test subspan(offset)
	{
		auto ss = s.subspan(3);

		ASSERT_ALWAYS(ss.size() == str.size() - 3)
		ASSERT_ALWAYS(ss.data() == str.data() + 3)
	}

	// test subspan(offset == size)
	{
		auto ss = s.subspan(s.size());

		utki::assert(ss.size() == 0, [&](auto&o){o << "ss.size() = " << ss.size();}, SL);
	}

	// test subspan(offset > size)
	{
		auto ss = s.subspan(1000);

		utki::assert(ss.size() == 0, [&](auto&o){o << "ss.size() = " << ss.size();}, SL);
	}

	// test subspan(offset, count)
	{
		auto ss = s.subspan(3, 4);

		utki::assert(ss.size() == 4, [&](auto&o){o << "ss.size() = " << ss.size();}, SL);
		ASSERT_ALWAYS(ss.data() == str.data() + 3)
	}
}
}
