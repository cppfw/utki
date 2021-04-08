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

	utki::assert(arr.size() == brr.size(), SL);
	for(size_t i = 0; i < arr.size(); ++i){
		utki::assert(arr[i].a == 0, SL);
		utki::assert(brr[i].a == 1, SL);
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
	utki::assert(&*brr.begin() != &*arr.begin(), SL);
	
	brr = arr;
	
	utki::assert(&*brr.begin() != &*arr.begin(), SL);
	utki::assert(&*arr.begin() == oldArrBegin, SL);
	utki::assert(&*brr.begin() == oldBrrBegin, SL);

	utki::assert(arr.size() == brr.size(), SL);
	for(size_t i = 0; i < arr.size(); ++i){
		utki::assert(arr[i].a == 0, SL);
		utki::assert(brr[i].a == 0, [&](auto&o){o << "brr[i].a = " << brr[i].a;}, SL);
		utki::assert(arr[i].id == brr[i].id, [&](auto&o){o << "arr[i].id = " << arr[i].id << " brr[i].id = " << brr[i].id;}, SL);
		utki::assert(arr[i].id == i, SL);
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

		utki::assert(ss.str() == "Hello world!", SL);
	}
}
}

namespace test_make_span{
void run(){
	// test make_span(const basic_string)
	{
		std::string str = "Hello world!";

		auto s = utki::make_span(str);

		utki::assert(s.size() == str.size(), SL);
		utki::assert(s.data() == str.data(), SL);
	}

	// test make_span(const char*)
	{
		const char* str = "Hello world!";
		auto s = utki::make_span(str);

		utki::assert(s.size() == strlen(str), SL);
		utki::assert(s.data() == str, SL);
	}
}
}

namespace test_subspan{
void run(){
	std::string str = "Hello world!";

	auto s = utki::make_span(str);

	utki::assert(s.size() == str.size(), SL);
	utki::assert(s.data() == str.data(), SL);
	
	// test subspan(offset)
	{
		auto ss = s.subspan(3);

		utki::assert(ss.size() == str.size() - 3, SL);
		utki::assert(ss.data() == str.data() + 3, SL);
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
		utki::assert(ss.data() == str.data() + 3, SL);
	}
}
}
