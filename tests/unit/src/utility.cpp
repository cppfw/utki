#include <array>
#include <string_view>
#include <variant>

#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/string.hpp>
#include <utki/type_traits.hpp>
#include <utki/utility.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace {
const tst::set set("util", [](tst::suite& suite) {
	suite.add("serialization_16_bit_little_endian", []() {
		for (uint32_t i = 0; i <= uint16_t(-1); ++i) {
			std::array<uint8_t, sizeof(uint16_t)> buf = {0};
			auto retp = utki::serialize16le(uint16_t(i), buf.data());

			tst::check_eq(buf[0], uint8_t(i & 0xff), SL);
			tst::check_eq(buf[1], uint8_t((i >> 8) & 0xff), SL);
			tst::check_eq(retp, &*utki::make_span(buf).end(), SL);

			uint16_t res = utki::deserialize16le(buf.data());
			tst::check_eq(res, uint16_t(i), SL);
		}
	});

	suite.add("serialization_16_bit_big_endian", []() {
		for (uint32_t i = 0; i <= uint16_t(-1); ++i) {
			std::array<uint8_t, sizeof(uint16_t)> buf = {0};
			auto retp = utki::serialize16be(uint16_t(i), buf.data());

			tst::check_eq(buf[0], uint8_t((i >> 8) & 0xff), SL);
			tst::check_eq(buf[1], uint8_t(i & 0xff), SL);
			tst::check_eq(retp, &*utki::make_span(buf).end(), SL);

			uint16_t res = utki::deserialize16be(buf.data());
			tst::check_eq(res, uint16_t(i), SL);
		}
	});

	suite.add("serialization_32_bit", []() {
		// increment by 513177, because if increment by 1 it takes too long to run the test
		for (uint64_t i = 0; i <= uint32_t(-1); i += 513177) {
			std::array<uint8_t, sizeof(uint32_t)> buf = {0};
			utki::serialize32le(uint32_t(i), buf.data());

			tst::check_eq(buf[0], uint8_t(i & 0xff), SL);
			tst::check_eq(buf[1], uint8_t((i >> 8) & 0xff), SL);
			tst::check_eq(buf[2], uint8_t((i >> 16) & 0xff), SL);
			tst::check_eq(buf[3], uint8_t((i >> 24) & 0xff), SL);

			uint32_t res = utki::deserialize32le(buf.data());
			tst::check_eq(res, uint32_t(i), SL);
		}
	});

	suite.add("serialization_64_bit", []() {
		uint64_t val = 0xabcdef87bcde67af;
		std::array<uint8_t, sizeof(uint64_t)> buf = {0};
		utki::serialize64le(val, buf.data());

		tst::check_eq(buf[0], uint8_t(val & 0xff), SL);
		tst::check_eq(buf[1], uint8_t((val >> 8) & 0xff), SL);
		tst::check_eq(buf[2], uint8_t((val >> 16) & 0xff), SL);
		tst::check_eq(buf[3], uint8_t((val >> 24) & 0xff), SL);
		tst::check_eq(buf[4], uint8_t((val >> 32) & 0xff), SL);
		tst::check_eq(buf[5], uint8_t((val >> 40) & 0xff), SL);
		tst::check_eq(buf[6], uint8_t((val >> 48) & 0xff), SL);
		tst::check_eq(buf[7], uint8_t((val >> 56) & 0xff), SL);

		uint64_t res = utki::deserialize64le(buf.data());
		tst::check_eq(res, val, SL);
	});

	suite.add<std::pair<float, std::array<uint8_t, 4>>>( //
		"serialization_float",
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
			auto ret = utki::serialize_float_le(p.first, buf.data());

			tst::check(ret == &*buf.end(), SL);

			// std::cout << "p.second = " << std::hex << unsigned(p.second[0]) << " " << unsigned(p.second[1]) << " "
			// 		  << unsigned(p.second[2]) << " " << unsigned(p.second[3]) << std::endl
			// 		  << "buf = " << unsigned(buf[0]) << " " << unsigned(buf[1]) << " " << unsigned(buf[2]) << " "
			// 		  << unsigned(buf[3]) << std::endl;

			tst::check(buf == p.second, SL);

			float f = utki::deserialize_float_le(buf.data());

			tst::check_eq(f, p.first, SL);
		}
	);

	suite.add("scope_exit", []() {
		bool flag = false;
		{
			utki::scope_exit se([&flag]() {
				flag = true;
			});
		}
		tst::check(flag, SL);
	});

	suite.add("scope_exit_release", []() {
		bool flag = false;
		{
			utki::scope_exit se([&flag]() {
				flag = true;
			});

			se.release();
		}
		tst::check(!flag, SL);
	});

	suite.add("flip_map", []() {
		std::map<int, std::string> m = {
			{10, "10"},
			{13, "13"},
			{42, "42"}
		};

		auto fm = utki::flip_map(m);

		tst::check_eq(fm.size(), size_t(3), SL);
		tst::check_eq(fm["10"], 10, SL);
		tst::check_eq(fm["13"], 13, SL);
		tst::check_eq(fm["42"], 42, SL);
	});

	suite.add("flip_map_less", []() {
		std::map<std::string, int, std::less<>> m = {
			{"10", 10},
			{"13", 13},
			{"42", 42}
		};

		auto fm = utki::flip_map(m);

		tst::check_eq(fm.size(), size_t(3), SL);
		tst::check_eq(fm[10], "10"s, SL);
		tst::check_eq(fm[13], "13"s, SL);
		tst::check_eq(fm[42], "42"s, SL);
	});

	suite.add("flip_map_comparator", []() {
		std::map<int, std::string> m = {
			{10, "10"},
			{13, "13"},
			{42, "42"}
		};

		auto fm = utki::flip_map<std::less<>>(m);

		tst::check_eq(fm.size(), size_t(3), SL);
		tst::check_eq(fm.find("10"sv)->second, 10, SL);
		tst::check_eq(fm.find("13"sv)->second, 13, SL);
		tst::check_eq(fm.find("42"sv)->second, 42, SL);
	});

	suite.add("skip_front", []() {
		std::array<int, 10> arr{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

		std::vector<int> result;

		for (const auto& i : utki::skip_front<3>(arr)) {
			result.push_back(i);
		}

		std::vector<int> expected = {3, 4, 5, 6, 7, 8, 9};

		tst::check(result == expected, SL);
	});

	suite.add("version_duplet__to_uint32_t", []() {
		utki::version_duplet dp = {10, 20};

		tst::check_eq(dp.to_uint32_t(), utki::version_duplet{10, 20}.to_uint32_t(), SL);

		tst::check_ne(dp.to_uint32_t(), utki::version_duplet{11, 20}.to_uint32_t(), SL);
		tst::check_ne(dp.to_uint32_t(), utki::version_duplet{10, 21}.to_uint32_t(), SL);
		tst::check_ne(dp.to_uint32_t(), utki::version_duplet{11, 21}.to_uint32_t(), SL);

		tst::check_lt(dp.to_uint32_t(), utki::version_duplet{11, 20}.to_uint32_t(), SL);
		tst::check_lt(dp.to_uint32_t(), utki::version_duplet{10, 21}.to_uint32_t(), SL);
		tst::check_lt(dp.to_uint32_t(), utki::version_duplet{11, 21}.to_uint32_t(), SL);

		tst::check_le(dp.to_uint32_t(), utki::version_duplet{11, 20}.to_uint32_t(), SL);
		tst::check_le(dp.to_uint32_t(), utki::version_duplet{10, 21}.to_uint32_t(), SL);
		tst::check_le(dp.to_uint32_t(), utki::version_duplet{11, 21}.to_uint32_t(), SL);
		tst::check_le(dp.to_uint32_t(), utki::version_duplet{10, 20}.to_uint32_t(), SL);
	});

	suite.add("version_duplet__operator_stream_output", []() {
		utki::version_duplet dp = {10, 20};

		std::stringstream ss;

		ss << dp;

		tst::check_eq(ss.str(), "10.20"s, SL);
	});

	suite.add("contains__vector", []() {
		std::vector<std::string> v = {"aaa", "bbb", "ccc"};

		tst::check(utki::contains(v, "aaa"), SL);
		tst::check(utki::contains(v, "bbb"), SL);
		tst::check(utki::contains(v, "ccc"), SL);
		tst::check(utki::contains(v, "aaa"s), SL);
		tst::check(utki::contains(v, "bbb"s), SL);
		tst::check(utki::contains(v, "ccc"s), SL);
		tst::check(utki::contains(v, "aaa"sv), SL);
		tst::check(utki::contains(v, "bbb"sv), SL);
		tst::check(utki::contains(v, "ccc"sv), SL);

		tst::check(!utki::contains(v, ""), SL);
		tst::check(!utki::contains(v, ""s), SL);
		tst::check(!utki::contains(v, ""sv), SL);
		tst::check(!utki::contains(v, "a"), SL);
		tst::check(!utki::contains(v, "b"s), SL);
		tst::check(!utki::contains(v, "c"sv), SL);
	});

	suite.add("contains__map", []() {
		std::map<std::string, std::u32string, std::less<>> v = {
			{"aaa", U"aaav"},
			{"bbb", U"bbbv"},
			{"ccc", U"cccv"}
		};

		tst::check(utki::contains(v, "aaa"), SL);
		tst::check(utki::contains(v, "bbb"), SL);
		tst::check(utki::contains(v, "ccc"), SL);
		tst::check(utki::contains(v, "aaa"s), SL);
		tst::check(utki::contains(v, "bbb"s), SL);
		tst::check(utki::contains(v, "ccc"s), SL);
		tst::check(utki::contains(v, "aaa"sv), SL);
		tst::check(utki::contains(v, "bbb"sv), SL);
		tst::check(utki::contains(v, "ccc"sv), SL);

		tst::check(!utki::contains(v, ""), SL);
		tst::check(!utki::contains(v, ""s), SL);
		tst::check(!utki::contains(v, ""sv), SL);
		tst::check(!utki::contains(v, "a"), SL);
		tst::check(!utki::contains(v, "b"s), SL);
		tst::check(!utki::contains(v, "c"sv), SL);
	});

	// TODO: test utki::next() and utki::prev()
});
} // namespace
