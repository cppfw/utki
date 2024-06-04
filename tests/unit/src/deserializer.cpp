#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/deserializer.hpp>
#include <utki/string.hpp>
#include <utki/util.hpp>

using namespace std::string_view_literals;

namespace {
const tst::set set("deserializer", [](tst::suite& suite) {
	suite.add("read_string", []() {
		auto str = "bbbHello world!ccc"sv;

		utki::deserializer d(utki::to_uint8_t(str));

		auto bbb = d.read_string(3);
		auto hw = d.read_string(12);
		auto ccc = d.read_string(3);

		tst::check_eq(bbb, "bbb"sv, SL);
		tst::check_eq(hw, "Hello world!"sv, SL);
		tst::check_eq(ccc, "ccc"sv, SL);
	});

	suite.add("read_span", []() {
		auto str = "bbbHello world!ccc"sv;

		utki::deserializer d(utki::to_uint8_t(str));

		auto bbb = d.read_span(3);
		auto hw = d.read_span(12);
		auto ccc = d.read_span(3);

		tst::check_eq(utki::make_string_view(bbb), "bbb"sv, SL);
		tst::check_eq(utki::make_string_view(hw), "Hello world!"sv, SL);
		tst::check_eq(utki::make_string_view(ccc), "ccc"sv, SL);
	});

	suite.add("read_uint8", []() {
		std::vector<uint8_t> buf = {0xa1, 0xb2};

		utki::deserializer d(buf);

		tst::check(!d.empty(), SL);
		tst::check_eq(d.size(), buf.size(), SL);

		auto u8_1 = d.read_uint8();
		auto u8_2 = d.read_uint8();

		tst::check_eq(u8_1, uint8_t(0xa1), SL);
		tst::check_eq(u8_2, uint8_t(0xb2), SL);
		tst::check(d.empty(), SL);
		tst::check_eq(d.size(), size_t(0), SL);
	});

	suite.add("read_uint16_le", []() {
		std::vector<uint8_t> buf = {0xa1, 0xb2};

		utki::deserializer d(buf);

		tst::check(!d.empty(), SL);
		tst::check_eq(d.size(), sizeof(uint16_t), SL);

		auto u16 = d.read_uint16_le();

		tst::check_eq(u16, uint16_t(0xb2a1), SL);
		tst::check(d.empty(), SL);
		tst::check_eq(d.size(), size_t(0), SL);
	});

	suite.add("read_uint32_le", []() {
		std::vector<uint8_t> buf = {0xa1, 0xb2, 0xc3, 0xd4};

		utki::deserializer d(buf);

		tst::check(!d.empty(), SL);
		tst::check_eq(d.size(), sizeof(uint32_t), SL);

		auto u32 = d.read_uint32_le();

		tst::check_eq(u32, uint32_t(0xd4c3b2a1), SL);
		tst::check(d.empty(), SL);
		tst::check_eq(d.size(), size_t(0), SL);
	});

	suite.add("read_uint64_le", []() {
		std::vector<uint8_t> buf = {0xa1, 0xb2, 0xc3, 0xd4, 0xe5, 0xf6, 0x17, 0x28};

		utki::deserializer d(buf);

		tst::check(!d.empty(), SL);
		tst::check_eq(d.size(), sizeof(uint64_t), SL);

		auto u64 = d.read_uint64_le();

		tst::check_eq(u64, uint64_t(0x2817f6e5d4c3b2a1), SL);
		tst::check(d.empty(), SL);
		tst::check_eq(d.size(), size_t(0), SL);
	});

	suite.add<std::pair<float, std::array<uint8_t, 4>>>( //
		"read_float_le",
		// clang-format off
		{
			{ 13, {0x00, 0x00, 0x50, 0x41}},
			{ 13666, {0x00, 0x88, 0x55, 0x46}},
			{ 13666e10f, {0x46, 0x95, 0xf8, 0x56}},
			{ 13.666f, {0xf0, 0xa7, 0x5a, 0x41}},
			{ -13, {0x00, 0x00, 0x50, 0xc1}},
			{ -13666, {0x00, 0x88, 0x55, 0xc6}},
			{ -13666e10f, {0x46, 0x95, 0xf8, 0xd6}},
			{ -13.666f, {0xf0, 0xa7, 0x5a, 0xc1}},
		},
		// clang-format on
		[](const auto& p) {
			std::array<uint8_t, 4> buf{};
			utki::serialize_float_le(p.first, buf.data());

			tst::check(buf == p.second, SL);

			utki::deserializer d(buf);

			float f = d.read_float_le();

			tst::check_eq(f, p.first, SL);
			tst::check(d.empty(), SL);
			tst::check_eq(d.size(), size_t(0), SL);
		}
	);

	suite.add("read_uint16_be", []() {
		std::vector<uint8_t> buf = {0xb2, 0xa1};

		utki::deserializer d(buf);

		tst::check(!d.empty(), SL);
		tst::check_eq(d.size(), sizeof(uint16_t), SL);

		auto u16 = d.read_uint16_be();

		tst::check_eq(u16, uint16_t(0xb2a1), SL);
		tst::check(d.empty(), SL);
		tst::check_eq(d.size(), size_t(0), SL);
	});

	suite.add("read_uint32_be", []() {
		std::vector<uint8_t> buf = {0xd4, 0xc3, 0xb2, 0xa1};

		utki::deserializer d(buf);

		tst::check(!d.empty(), SL);
		tst::check_eq(d.size(), sizeof(uint32_t), SL);

		auto u32 = d.read_uint32_be();

		tst::check_eq(u32, uint32_t(0xd4c3b2a1), SL);
		tst::check(d.empty(), SL);
		tst::check_eq(d.size(), size_t(0), SL);
	});

	suite.add("read_uint64_be", []() {
		std::vector<uint8_t> buf = {0x28, 0x17, 0xf6, 0xe5, 0xd4, 0xc3, 0xb2, 0xa1};

		utki::deserializer d(buf);

		tst::check(!d.empty(), SL);
		tst::check_eq(d.size(), sizeof(uint64_t), SL);

		auto u64 = d.read_uint64_be();

		tst::check_eq(u64, uint64_t(0x2817f6e5d4c3b2a1), SL);
		tst::check(d.empty(), SL);
		tst::check_eq(d.size(), size_t(0), SL);
	});

	suite.add<std::pair<float, std::array<uint8_t, 4>>>( //
		"read_float_be",
		// clang-format off
		{
			{ 13, {0x41, 0x50, 0x00, 0x00 }},
			{ 13666, {0x46, 0x55, 0x88, 0x00 }},
			{ 13666e10f, {0x56, 0xf8, 0x95, 0x46 }},
			{ 13.666f, {0x41, 0x5a, 0xa7, 0xf0 }},
			{ -13, {0xc1, 0x50, 0x00, 0x00 }},
			{ -13666, {0xc6, 0x55, 0x88, 0x00 }},
			{ -13666e10f, {0xd6, 0xf8, 0x95, 0x46}},
			{ -13.666f, {0xc1, 0x5a, 0xa7, 0xf0}},
		},
		// clang-format on
		[](const auto& p) {
			std::array<uint8_t, 4> buf{};
			utki::serialize_float_be(p.first, buf.data());

			tst::check(buf == p.second, SL);

			utki::deserializer d(buf);

			float f = d.read_float_be();

			tst::check_eq(f, p.first, SL);
			tst::check(d.empty(), SL);
			tst::check_eq(d.size(), size_t(0), SL);
		}
	);

	suite.add("skip", []() {
		auto str = "bbbHello world!ccc"sv;

		utki::deserializer d(utki::to_uint8_t(str));

		d.skip(3);
		auto hw = d.read_span(12);
		d.skip(3);

		tst::check_eq(utki::make_string_view(hw), "Hello world!"sv, SL);
		tst::check(d.empty(), SL);
	});
});
} // namespace
