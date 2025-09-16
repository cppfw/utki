#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/version.hpp>

using namespace std::string_literals;

namespace {
const tst::set set("version", [](tst::suite& suite) {
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

	suite.add("operator_greater_or_equal", []() {
		tst::check(utki::version_duplet{1, 2} >= utki::version_duplet{1, 1}, SL);
		tst::check(utki::version_duplet{1, 2} >= utki::version_duplet{1, 2}, SL);
		tst::check(!(utki::version_duplet{1, 2} >= utki::version_duplet{1, 3}), SL);
		tst::check(utki::version_duplet{2, 0} >= utki::version_duplet{1, 9}, SL);
		tst::check(utki::version_duplet{2, 0} >= utki::version_duplet{0, 9}, SL);
	});

	suite.add("operator_less", []() {
		tst::check(!(utki::version_duplet{1, 2} < utki::version_duplet{1, 1}), SL);
		tst::check(!(utki::version_duplet{1, 2} < utki::version_duplet{1, 2}), SL);
		tst::check(utki::version_duplet{1, 2} < utki::version_duplet{1, 3}, SL);
		tst::check(!(utki::version_duplet{2, 0} < utki::version_duplet{1, 9}), SL);
		tst::check(!(utki::version_duplet{2, 0} < utki::version_duplet{0, 9}), SL);
	});
});
} // namespace