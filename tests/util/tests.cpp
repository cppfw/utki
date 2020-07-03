#include "../../src/utki/debug.hpp"
#include "../../src/utki/types.hpp"
#include "../../src/utki/util.hpp"

#include "tests.hpp"

#include <array>

using namespace utki;


namespace TestSerialization{
void Run(){
	//16 bit
	for(std::uint32_t i = 0; i <= std::uint16_t(-1); ++i){
		std::array<std::uint8_t, sizeof(std::uint16_t)> buf;
		utki::serialize16le(std::uint16_t(i), &*buf.begin());

		ASSERT_ALWAYS(buf[0] == std::uint8_t(i & 0xff))
		ASSERT_ALWAYS(buf[1] == std::uint8_t((i >> 8) & 0xff))

		std::uint16_t res = utki::deserialize16le(&*buf.begin());
		ASSERT_ALWAYS(res == std::uint16_t(i))
//		TRACE(<< "TestSerialization(): i16 = " << i << std::endl)
	}

	//32 bit
	for(std::uint64_t i = 0; i <= std::uint32_t(-1); i += 1317){//increment by 1317, because if increment by 1 it takes too long to run the test
		std::array<std::uint8_t, sizeof(std::uint32_t)> buf;
		utki::serialize32le(std::uint32_t(i), &*buf.begin());

		ASSERT_ALWAYS(buf[0] == std::uint8_t(i & 0xff))
		ASSERT_ALWAYS(buf[1] == std::uint8_t((i >> 8) & 0xff))
		ASSERT_ALWAYS(buf[2] == std::uint8_t((i >> 16) & 0xff))
		ASSERT_ALWAYS(buf[3] == std::uint8_t((i >> 24) & 0xff))

		std::uint32_t res = utki::deserialize32le(&*buf.begin());
		ASSERT_ALWAYS(res == std::uint32_t(i))
//		TRACE(<< "TestSerialization(): i32 = " << i << std::endl)
	}
	
	
	//64 bit
	{
		std::uint64_t val = 0xabcdef87bcde67af;
		std::array<std::uint8_t, sizeof(std::uint64_t)> buf;
		utki::serialize64le(val, &*buf.begin());

		ASSERT_ALWAYS(buf[0] == std::uint8_t(val & 0xff))
		ASSERT_ALWAYS(buf[1] == std::uint8_t((val >> 8) & 0xff))
		ASSERT_ALWAYS(buf[2] == std::uint8_t((val >> 16) & 0xff))
		ASSERT_ALWAYS(buf[3] == std::uint8_t((val >> 24) & 0xff))
		ASSERT_ALWAYS(buf[4] == std::uint8_t((val >> 32) & 0xff))
		ASSERT_ALWAYS(buf[5] == std::uint8_t((val >> 40) & 0xff))
		ASSERT_ALWAYS(buf[6] == std::uint8_t((val >> 48) & 0xff))
		ASSERT_ALWAYS(buf[7] == std::uint8_t((val >> 56) & 0xff))

		std::uint64_t res = utki::deserialize64le(&*buf.begin());
		ASSERT_ALWAYS(res == val)
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
		ASSERT_ALWAYS(flag)
	}
	
	{
		bool flag = false;
		{
			utki::scope_exit se([&flag](){
				flag = true;
			});
			
			se.reset();
		}
		ASSERT_ALWAYS(!flag)
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
	
	ASSERT_ALWAYS(fm.size() == 3)
	ASSERT_ALWAYS(fm["10"] == 10)
	ASSERT_ALWAYS(fm["13"] == 13)
	ASSERT_ALWAYS(fm["42"] == 42)
}
}
