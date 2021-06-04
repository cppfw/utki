#include <cstring>

#include "tests.hpp"

#include "../../src/utki/debug.hpp"
#include "../../src/utki/unicode.hpp"

namespace TestSimple{
void Run(){
	// string in utf8 = aБцﺶ𠀋
	std::vector<uint8_t> buf = {{0x61, 0xd0, 0x91, 0xd1, 0x86, 0xef, 0xba, 0xb6, 0xf0, 0xa0, 0x80, 0x8b}};
	
	std::vector<uint8_t> str(buf.size() + 1);
	memcpy(&*str.begin(), &*buf.begin(), buf.size());
	str[buf.size()] = 0; // null-terminate
	
	utki::utf8_iterator i(reinterpret_cast<char*>(&*str.begin()));
	
	utki::assert(i.character() == 'a', SL);
	++i;
	utki::assert(i.character() == 0x0411, [&](auto&o){o << "i.Char() = " << i.character();}, SL); // capital russian B
	++i;
	utki::assert(i.character() == 0x0446, SL); // small russian C
	++i;
	utki::assert(i.character() == 0xfeb6, [&](auto&o){o << "i.Char() = " << i.character();}, SL); // some arabic stuff
	++i;
	utki::assert(i.character() == 0x2000b, [&](auto&o){o << "i.Char() = " << i.character();}, SL); // some compatibility char
	++i;
	utki::assert(i.character() == 0, [&](auto&o){o << "i.Char() = " << i.character();}, SL);
	utki::assert(i.is_end(), SL);
}
}

namespace testUtf32ToUtf8{
void run(){
	{
		auto ret = utki::to_utf8(0);
		utki::assert(ret[0] == 0, SL);
		utki::assert(ret[1] == 0, SL);
	}
	{
		auto ret = utki::to_utf8(0x73); //s
		utki::assert(ret[0] == 0x73, SL);
		utki::assert(ret[1] == 0, SL);
	}
	{
		auto ret = utki::to_utf8(0x41a); // Capital cyrillic K
		utki::assert(ret[0] == char(0xd0), [&](auto&o){o << "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff);}, SL);
		utki::assert(ret[1] == char(0x9a), SL);
		utki::assert(ret[2] == 0, SL);
	}
	{
		auto ret = utki::to_utf8(0xbf5); // ௵
		utki::assert(ret[0] == char(0xe0), [&](auto&o){o << "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff);}, SL);
		utki::assert(ret[1] == char(0xaf), [&](auto&o){o << "ret[1] = 0x" << std::hex << (unsigned(ret[1]) & 0xff);}, SL);
		utki::assert(ret[2] == char(0xb5), SL);
		utki::assert(ret[3] == 0, SL);
	}
	{
		auto ret = utki::to_utf8(0x26218); // 𦈘
		utki::assert(ret[0] == char(0xf0), [&](auto&o){o << "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff);}, SL);
		utki::assert(ret[1] == char(0xa6), [&](auto&o){o << "ret[1] = 0x" << std::hex << (unsigned(ret[1]) & 0xff);}, SL);
		utki::assert(ret[2] == char(0x88), SL);
		utki::assert(ret[3] == char(0x98), SL);
		utki::assert(ret[4] == 0, SL);
	}
	{
		auto ret = utki::to_utf8(0x3FFFFFF);
		utki::assert(ret[0] == char(0xfb), [&](auto&o){o << "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff);}, SL);
		utki::assert(ret[1] == char(0xbf), [&](auto&o){o << "ret[1] = 0x" << std::hex << (unsigned(ret[1]) & 0xff);}, SL);
		utki::assert(ret[2] == char(0xbf), SL);
		utki::assert(ret[3] == char(0xbf), SL);
		utki::assert(ret[4] == char(0xbf), SL);
		utki::assert(ret[5] == 0, SL);
	}
	{
		auto ret = utki::to_utf8(0x7FFFFFFF);
		utki::assert(ret[0] == char(0xfd), [&](auto&o){o << "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff);}, SL);
		utki::assert(ret[1] == char(0xbf), [&](auto&o){o << "ret[1] = 0x" << std::hex << (unsigned(ret[1]) & 0xff);}, SL);
		utki::assert(ret[2] == char(0xbf), SL);
		utki::assert(ret[3] == char(0xbf), SL);
		utki::assert(ret[4] == char(0xbf), SL);
		utki::assert(ret[5] == char(0xbf), SL);
		utki::assert(ret[6] == 0, SL);
	}

	// test UTF-32 string to UTF-8 string conversion
	{
		auto utf32 = U"Hello world!";

		auto utf8 = utki::to_utf8(utf32);

		utki::assert(utf8 == "Hello world!", SL);
	}
}
}

namespace test_utf8_to_utf32{
void run(){
	// test span<const uint8_t> to utf32
	{
		// string in utf8 = aБцﺶ𠀋
		std::vector<uint8_t> buf = {{0x61, 0xd0, 0x91, 0xd1, 0x86, 0xef, 0xba, 0xb6, 0xf0, 0xa0, 0x80, 0x8b}};

		auto str = utki::to_utf32(buf);

		utki::assert(
				str == U"aБцﺶ𠀋",
				[&](auto&o){
					o << "str = " << utki::to_utf8(str) << '\n';
					for(auto c : str){
						o << std::hex << "0x" << c << ", ";
					}
				},
				SL
			);
	}

	// test span<const char> to utf32
	{
		// string in utf8 = aБцﺶ𠀋
		const char* buf = "aБцﺶ𠀋";

		auto str = utki::to_utf32(utki::make_span(buf));

		utki::assert(str == U"aБцﺶ𠀋", SL);
	}

	// test string_view to utf32
	{
		// string in utf8 = aБцﺶ𠀋
		std::string_view buf = "aБцﺶ𠀋";

		auto str = utki::to_utf32(buf);

		utki::assert(str == U"aБцﺶ𠀋", SL);
	}
}
}
