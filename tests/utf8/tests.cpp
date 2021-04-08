#include <cstring>

#include "tests.hpp"

#include "../../src/utki/debug.hpp"
#include "../../src/utki/unicode.hpp"

namespace TestSimple{

void Run(){
	//string in utf8 = aБцﺶ𠀋
	std::vector<uint8_t> buf = {{0x61, 0xd0, 0x91, 0xd1, 0x86, 0xef, 0xba, 0xb6, 0xf0, 0xa0, 0x80, 0x8b}};
	
//	TRACE_ALWAYS(<< "buf.Size() = " << buf.Size() << std::endl)
	
	std::vector<uint8_t> str(buf.size() + 1);
	memcpy(&*str.begin(), &*buf.begin(), buf.size());
	str[buf.size()] = 0; // null-terminate
	
	utki::utf8_iterator i(reinterpret_cast<char*>(&*str.begin()));
	
	ASSERT_ALWAYS(i.character() == 'a')
	++i;
	utki::assert(i.character() == 0x0411, [&](auto&o){o << "i.Char() = " << i.character();}, SL); // capital russian B
	++i;
	ASSERT_ALWAYS(i.character() == 0x0446) // small russian C
	++i;
	utki::assert(i.character() == 0xfeb6, [&](auto&o){o << "i.Char() = " << i.character();}, SL); // some arabic stuff
	++i;
	utki::assert(i.character() == 0x2000b, [&](auto&o){o << "i.Char() = " << i.character();}, SL); // some compatibility char
	++i;
	utki::assert(i.character() == 0, [&](auto&o){o << "i.Char() = " << i.character();}, SL);
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
		utki::assert(ret[0] == char(0xd0), [&](auto&o){o << "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff);}, SL);
		ASSERT_ALWAYS(ret[1] == char(0x9a));
		ASSERT_ALWAYS(ret[2] == 0);
	}
	{
		auto ret = utki::to_utf8(0xbf5); // ௵
		utki::assert(ret[0] == char(0xe0), [&](auto&o){o << "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff);}, SL);
		utki::assert(ret[1] == char(0xaf), [&](auto&o){o << "ret[1] = 0x" << std::hex << (unsigned(ret[1]) & 0xff);}, SL);
		ASSERT_ALWAYS(ret[2] == char(0xb5));
		ASSERT_ALWAYS(ret[3] == 0);
	}
	{
		auto ret = utki::to_utf8(0x26218); // 𦈘
		utki::assert(ret[0] == char(0xf0), [&](auto&o){o << "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff);}, SL);
		utki::assert(ret[1] == char(0xa6), [&](auto&o){o << "ret[1] = 0x" << std::hex << (unsigned(ret[1]) & 0xff);}, SL);
		ASSERT_ALWAYS(ret[2] == char(0x88));
		ASSERT_ALWAYS(ret[3] == char(0x98));
		ASSERT_ALWAYS(ret[4] == 0);
	}
	{
		auto ret = utki::to_utf8(0x3FFFFFF);
		utki::assert(ret[0] == char(0xfb), [&](auto&o){o << "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff);}, SL);
		utki::assert(ret[1] == char(0xbf), [&](auto&o){o << "ret[1] = 0x" << std::hex << (unsigned(ret[1]) & 0xff);}, SL);
		ASSERT_ALWAYS(ret[2] == char(0xbf));
		ASSERT_ALWAYS(ret[3] == char(0xbf));
		ASSERT_ALWAYS(ret[4] == char(0xbf));
		ASSERT_ALWAYS(ret[5] == 0);
	}
	{
		auto ret = utki::to_utf8(0x7FFFFFFF);
		utki::assert(ret[0] == char(0xfd), [&](auto&o){o << "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff);}, SL);
		utki::assert(ret[1] == char(0xbf), [&](auto&o){o << "ret[1] = 0x" << std::hex << (unsigned(ret[1]) & 0xff);}, SL);
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
