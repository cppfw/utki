#include "../../src/utki/debug.hpp"
#include "../../src/utki/types.hpp"
#include "../../src/utki/util.hpp"

#include "tests.hpp"

#include <array>
#include <vector>

using namespace utki;


namespace TestSerialization{
void Run(){
	//16 bit
	for(uint32_t i = 0; i <= uint16_t(-1); ++i){
		std::array<uint8_t, sizeof(uint16_t)> buf;
		utki::serialize16le(uint16_t(i), &*buf.begin());

		utki::assert(buf[0] == uint8_t(i & 0xff), SL);
		utki::assert(buf[1] == uint8_t((i >> 8) & 0xff), SL);

		uint16_t res = utki::deserialize16le(&*buf.begin());
		utki::assert(res == uint16_t(i), SL);
//		TRACE(<< "TestSerialization(): i16 = " << i << std::endl)
	}

	//32 bit
	for(uint64_t i = 0; i <= uint32_t(-1); i += 1317){//increment by 1317, because if increment by 1 it takes too long to run the test
		std::array<uint8_t, sizeof(uint32_t)> buf;
		utki::serialize32le(uint32_t(i), &*buf.begin());

		utki::assert(buf[0] == uint8_t(i & 0xff), SL);
		utki::assert(buf[1] == uint8_t((i >> 8) & 0xff), SL);
		utki::assert(buf[2] == uint8_t((i >> 16) & 0xff), SL);
		utki::assert(buf[3] == uint8_t((i >> 24) & 0xff), SL);

		uint32_t res = utki::deserialize32le(&*buf.begin());
		utki::assert(res == uint32_t(i), SL);
//		TRACE(<< "TestSerialization(): i32 = " << i << std::endl)
	}
	
	
	//64 bit
	{
		uint64_t val = 0xabcdef87bcde67af;
		std::array<uint8_t, sizeof(uint64_t)> buf;
		utki::serialize64le(val, &*buf.begin());

		utki::assert(buf[0] == uint8_t(val & 0xff), SL);
		utki::assert(buf[1] == uint8_t((val >> 8) & 0xff), SL);
		utki::assert(buf[2] == uint8_t((val >> 16) & 0xff), SL);
		utki::assert(buf[3] == uint8_t((val >> 24) & 0xff), SL);
		utki::assert(buf[4] == uint8_t((val >> 32) & 0xff), SL);
		utki::assert(buf[5] == uint8_t((val >> 40) & 0xff), SL);
		utki::assert(buf[6] == uint8_t((val >> 48) & 0xff), SL);
		utki::assert(buf[7] == uint8_t((val >> 56) & 0xff), SL);

		uint64_t res = utki::deserialize64le(&*buf.begin());
		utki::assert(res == val, SL);
//		TRACE(<< "TestSerialization(): i64 = " << i << std::endl)
	}
}
}//~namespace



namespace TestScopeExit{
void Run(){
	{
		bool flag = false;
		{
			utki::scope_exit se([&flag](){
				flag = true;
			});
		}
		utki::assert(flag, SL);
	}
	
	{
		bool flag = false;
		{
			utki::scope_exit se([&flag](){
				flag = true;
			});
			
			se.reset();
		}
		utki::assert(!flag, SL);
	}
}
}

namespace testFlipMap{
void run(){
	std::map<int, std::string> m = {
		{10, "10"},
		{13, "13"},
		{42, "42"}
	};
	
	auto fm = utki::flip_map(m);
	
	utki::assert(fm.size() == 3, SL);
	utki::assert(fm["10"] == 10, SL);
	utki::assert(fm["13"] == 13, SL);
	utki::assert(fm["42"] == 42, SL);
}
}

#if __cplusplus >= 201703L
namespace test_variant_get_index{
void run(){
	typedef std::variant<int, const char*, std::string, std::pair<bool, int>> variant_type;

	std::array<variant_type, std::variant_size<variant_type>::value> variants = {{
		std::string("hello world!"),
		int(123),
		"const char*",
		std::make_pair(true, 45)
	}};

	utki::assert(variants.size() == 4, SL);

	std::vector<std::string> res;

	for(auto& v : variants){
		switch(v.index()){
			case utki::get_index<int, decltype(variants)::value_type>::value:
				res.push_back("int");
				break;
			case utki::get_index<std::pair<bool, int>, decltype(variants)::value_type>::value:
				res.push_back("pair");
				break;
			case utki::get_index<std::string, decltype(variants)::value_type>::value:
				res.push_back("string");
				break;
			case utki::get_index<const char*, decltype(variants)::value_type>::value:
				res.push_back("const char*");
				break;
			default:
				break;
		}
	}

#ifdef DEBUG
	for(auto& r : res){
		TRACE(<< "r = " << r << std::endl)
	}
#endif

	std::vector<std::string> expected = {{"string", "int", "const char*", "pair"}};

	utki::assert(res == expected, SL);
}
}
#endif
