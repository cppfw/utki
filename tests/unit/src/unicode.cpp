#include <utki/config.hpp>

#if M_COMPILER != M_COMPILER_MSVC || M_COMPILER_MSVC_TOOLS_V >= 142

#	include <tst/check.hpp>
#	include <tst/set.hpp>
#	include <utki/unicode.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace {
tst::set set("unicode", [](tst::suite& suite) {
	suite.add("utf8_iterator", []() {
		// string in utf8 = aБцﺶ𠀋
		std::vector<uint8_t> buf = {0x61, 0xd0, 0x91, 0xd1, 0x86, 0xef, 0xba, 0xb6, 0xf0, 0xa0, 0x80, 0x8b};

		std::vector<uint8_t> str(buf.size() + 1);
		memcpy(str.data(), buf.data(), buf.size());
		str[buf.size()] = 0; // null-terminate

		utki::utf8_iterator i(reinterpret_cast<char*>(str.data()));

		tst::check_eq(uint32_t(i.character()), uint32_t(U'a'), SL);
		++i;
		tst::check_eq(
			uint32_t(i.character()),
			uint32_t(0x0411),
			[&](auto& o) {
				o << "i.character() = " << uint32_t(i.character());
			},
			SL
		); // capital russian B
		++i;
		tst::check_eq(uint32_t(i.character()), uint32_t(0x0446), SL); // small russian C
		++i;
		tst::check_eq(
			uint32_t(i.character()),
			uint32_t(0xfeb6),
			[&](auto& o) {
				o << "i.character() = " << uint32_t(i.character());
			},
			SL
		); // some arabic stuff
		++i;
		tst::check_eq(
			uint32_t(i.character()),
			uint32_t(0x2000b),
			[&](auto& o) {
				o << "i.character() = " << uint32_t(i.character());
			},
			SL
		); // some compatibility char
		++i;
		tst::check_eq(
			uint32_t(i.character()),
			uint32_t(0),
			[&](auto& o) {
				o << "i.character() = " << uint32_t(i.character());
			},
			SL
		);
		tst::check(i.is_end(), SL);
	});

	suite.add("utf32_to_utf8_zero", []() {
		auto ret = utki::to_utf8(0);
		tst::check_eq(ret[0], char(0), SL);
		tst::check_eq(ret[1], char(0), SL);
	});

	suite.add("utf32_to_utf8_one_byte", []() {
		auto ret = utki::to_utf8(0x73); //s
		tst::check_eq(ret[0], char(0x73), SL);
		tst::check_eq(ret[1], char(0), SL);
	});

	suite.add("utf32_to_utf8_two_byte", []() {
		auto ret = utki::to_utf8(0x41a); // Capital cyrillic K
		tst::check_eq(
			ret[0],
			char(0xd0),
			[&](auto& o) {
				o << "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff);
			},
			SL
		);
		tst::check_eq(ret[1], char(0x9a), SL);
		tst::check_eq(ret[2], char(0), SL);
	});

	suite.add("utf32_to_utf8_three_byte", []() {
		auto ret = utki::to_utf8(0xbf5); // ௵
		tst::check_eq(
			ret[0],
			char(0xe0),
			[&](auto& o) {
				o << "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff);
			},
			SL
		);
		tst::check_eq(
			ret[1],
			char(0xaf),
			[&](auto& o) {
				o << "ret[1] = 0x" << std::hex << (unsigned(ret[1]) & 0xff);
			},
			SL
		);
		tst::check_eq(ret[2], char(0xb5), SL);
		tst::check_eq(ret[3], char(0), SL);
	});

	suite.add("utf32_to_utf8_four_byte", []() {
		auto ret = utki::to_utf8(0x26218); // 𦈘
		tst::check_eq(
			ret[0],
			char(0xf0),
			[&](auto& o) {
				o << "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff);
			},
			SL
		);
		tst::check_eq(
			ret[1],
			char(0xa6),
			[&](auto& o) {
				o << "ret[1] = 0x" << std::hex << (unsigned(ret[1]) & 0xff);
			},
			SL
		);
		tst::check_eq(ret[2], char(0x88), SL);
		tst::check_eq(ret[3], char(0x98), SL);
		tst::check_eq(ret[4], char(0), SL);
	});

	suite.add("utf32_to_utf8_five_byte", []() {
		auto ret = utki::to_utf8(0x3FFFFFF);
		tst::check_eq(
			ret[0],
			char(0xfb),
			[&](auto& o) {
				o << "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff);
			},
			SL
		);
		tst::check_eq(
			ret[1],
			char(0xbf),
			[&](auto& o) {
				o << "ret[1] = 0x" << std::hex << (unsigned(ret[1]) & 0xff);
			},
			SL
		);
		tst::check_eq(ret[2], char(0xbf), SL);
		tst::check_eq(ret[3], char(0xbf), SL);
		tst::check_eq(ret[4], char(0xbf), SL);
		tst::check_eq(ret[5], char(0), SL);
	});

	suite.add("utf32_to_utf8_six_byte", []() {
		auto ret = utki::to_utf8(0x7FFFFFFF);
		tst::check_eq(
			ret[0],
			char(0xfd),
			[&](auto& o) {
				o << "ret[0] = 0x" << std::hex << (unsigned(ret[0]) & 0xff);
			},
			SL
		);
		tst::check_eq(
			ret[1],
			char(0xbf),
			[&](auto& o) {
				o << "ret[1] = 0x" << std::hex << (unsigned(ret[1]) & 0xff);
			},
			SL
		);
		tst::check_eq(ret[2], char(0xbf), SL);
		tst::check_eq(ret[3], char(0xbf), SL);
		tst::check_eq(ret[4], char(0xbf), SL);
		tst::check_eq(ret[5], char(0xbf), SL);
		tst::check_eq(ret[6], char(0), SL);
	});

	suite.add("utf32_to_utf8", []() {
		auto utf32 = U"Hello world!";

		auto utf8 = utki::to_utf8(utf32);

		tst::check_eq(utf8, "Hello world!"s, SL);
	});

	suite.add("utf8_to_utf32_span_uint8_t", []() {
		auto expected = U"aБцﺶ𠀋"sv;
		// string in utf8 = aБцﺶ𠀋
		std::vector<uint8_t> buf = {0x61, 0xd0, 0x91, 0xd1, 0x86, 0xef, 0xba, 0xb6, 0xf0, 0xa0, 0x80, 0x8b};

		auto str = utki::to_utf32(buf);

		tst::check_eq(str.size(), expected.size(), SL);

		tst::check(
			str == expected,
			[&](auto& o) {
				o << "str = " << utki::to_utf8(str) << '\n';
				o << "str = ";
				for (auto c : str) {
					o << std::hex << "0x" << uint32_t(c) << ", ";
				}
				o << std::endl;
				o << "expected = ";
				for (char32_t c : expected) {
					o << std::hex << "0x" << uint32_t(c) << ", ";
				}
			},
			SL
		);
	});

	suite.add("utf8_to_utf32_span_char", []() {
		// string in utf8 = aБцﺶ𠀋
		const char* buf = "aБцﺶ𠀋";

		auto str = utki::to_utf32(utki::make_span(buf));

		tst::check(
			str == U"aБцﺶ𠀋",
			[&](auto& o) {
				o << "str = " << utki::to_utf8(str) << '\n';
				for (auto c : str) {
					o << std::hex << "0x" << uint32_t(c) << ", ";
				}
			},
			SL
		);
	});

	suite.add("utf8_to_utf32_string_view", []() {
		// string in utf8 = aБцﺶ𠀋
		std::string_view buf = "aБцﺶ𠀋";

		auto str = utki::to_utf32(buf);

		tst::check(
			str == U"aБцﺶ𠀋",
			[&](auto& o) {
				o << "str = " << utki::to_utf8(str) << '\n';
				for (auto c : str) {
					o << std::hex << "0x" << uint32_t(c) << ", ";
				}
			},
			SL
		);
	});
});
} // namespace

#endif // ~ non-MSVC compiler or MSVC compiler tools >= v142
