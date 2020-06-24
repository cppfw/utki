#include <cstring>

#include "tests.hpp"

#include "../../src/utki/debug.hpp"
#include "../../src/utki/utf8.hpp"

namespace TestSimple{

void Run(){
	//string in utf8 = aБцﺶ𠀋
	std::vector<std::uint8_t> buf = {{0x61, 0xd0, 0x91, 0xd1, 0x86, 0xef, 0xba, 0xb6, 0xf0, 0xa0, 0x80, 0x8b}};
	
//	TRACE_ALWAYS(<< "buf.Size() = " << buf.Size() << std::endl)
	
	std::vector<std::uint8_t> str(buf.size() + 1);
	memcpy(&*str.begin(), &*buf.begin(), buf.size());
	str[buf.size()] = 0; // null-terminate
	
	utki::utf8_iterator i(reinterpret_cast<char*>(&*str.begin()));
	
	ASSERT_ALWAYS(i.character() == 'a')
	++i;
	ASSERT_INFO_ALWAYS(i.character() == 0x0411, "i.Char() = " << i.character()) // capital russian B
	++i;
	ASSERT_ALWAYS(i.character() == 0x0446) // small russian C
	++i;
	ASSERT_INFO_ALWAYS(i.character() == 0xfeb6, "i.Char() = " << i.character()) // some arabic stuff
	++i;
	ASSERT_INFO_ALWAYS(i.character() == 0x2000b, "i.Char() = " << i.character()) // some compatibility char
	++i;
	ASSERT_INFO_ALWAYS(i.character() == 0, "i.Char() = " << i.character())
	ASSERT_ALWAYS(i.is_end())
}

}


namespace testUtf32ToUtf8{
void run(){
	{
		auto ret = utki::to_utf8(0);
		ASSERT_ALWAYS(ret[0] == 0);
		ASSERT_ALWAYS(ret[1] == 0);
	}
	{
		auto ret = utki::to_utf8(0x73); //s
		ASSERT_ALWAYS(ret[0] == 0x73);
		ASSERT_ALWAYS(ret[1] == 0);
	}
	{
		auto ret = utki::to_utf8(0x41a); // Capital cyrillic K
		ASSERT_INFO_ALWAYS(ret[0] == char(0xd0), "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff));
		ASSERT_ALWAYS(ret[1] == char(0x9a));
		ASSERT_ALWAYS(ret[2] == 0);
	}
	{
		auto ret = utki::to_utf8(0xbf5); // ௵
		ASSERT_INFO_ALWAYS(ret[0] == char(0xe0), "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff));
		ASSERT_INFO_ALWAYS(ret[1] == char(0xaf), "ret[1] = 0x" << std::hex << (unsigned(ret[1]) & 0xff));
		ASSERT_ALWAYS(ret[2] == char(0xb5));
		ASSERT_ALWAYS(ret[3] == 0);
	}
	{
		auto ret = utki::to_utf8(0x26218); // 𦈘
		ASSERT_INFO_ALWAYS(ret[0] == char(0xf0), "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff));
		ASSERT_INFO_ALWAYS(ret[1] == char(0xa6), "ret[1] = 0x" << std::hex << (unsigned(ret[1]) & 0xff));
		ASSERT_ALWAYS(ret[2] == char(0x88));
		ASSERT_ALWAYS(ret[3] == char(0x98));
		ASSERT_ALWAYS(ret[4] == 0);
	}
	{
		auto ret = utki::to_utf8(0x3FFFFFF);
		ASSERT_INFO_ALWAYS(ret[0] == char(0xfb), "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff));
		ASSERT_INFO_ALWAYS(ret[1] == char(0xbf), "ret[1] = 0x" << std::hex << (unsigned(ret[1]) & 0xff));
		ASSERT_ALWAYS(ret[2] == char(0xbf));
		ASSERT_ALWAYS(ret[3] == char(0xbf));
		ASSERT_ALWAYS(ret[4] == char(0xbf));
		ASSERT_ALWAYS(ret[5] == 0);
	}
	{
		auto ret = utki::to_utf8(0x7FFFFFFF);
		ASSERT_INFO_ALWAYS(ret[0] == char(0xfd), "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff));
		ASSERT_INFO_ALWAYS(ret[1] == char(0xbf), "ret[1] = 0x" << std::hex << (unsigned(ret[1]) & 0xff));
		ASSERT_ALWAYS(ret[2] == char(0xbf));
		ASSERT_ALWAYS(ret[3] == char(0xbf));
		ASSERT_ALWAYS(ret[4] == char(0xbf));
		ASSERT_ALWAYS(ret[5] == char(0xbf));
		ASSERT_ALWAYS(ret[6] == 0);
	}

	// test UTF-32 string to UTF-8 string conversion
	{
		auto utf32 = U"Hello world!";

		auto utf8 = utki::to_utf8(utf32);

		ASSERT_ALWAYS(utf8 == "Hello world!")
	}
}
}
