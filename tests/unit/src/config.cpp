#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/config.hpp>

namespace {
tst::set set("config", [](tst::suite& suite) {
	suite.add("endianness", []() {
		unsigned integer = 1;
		static_assert(sizeof(integer) >= 2, "1 byte integer detected");
		auto p = reinterpret_cast<const uint8_t*>(&integer);

		bool is_little_endian = (*p == 1);

		if (is_little_endian) {
			tst::check(CFG_ENDIANNESS == CFG_ENDIANNESS_LITTLE, SL) << "CFG_ENDIANNESS = " << CFG_ENDIANNESS;
			tst::check(CFG_ENDIANNESS != CFG_ENDIANNESS_BIG, SL) << "CFG_ENDIANNESS = " << CFG_ENDIANNESS;
		} else {
			tst::check(CFG_ENDIANNESS == CFG_ENDIANNESS_BIG, SL) << "CFG_ENDIANNESS = " << CFG_ENDIANNESS;
			tst::check(CFG_ENDIANNESS != CFG_ENDIANNESS_LITTLE, SL) << "CFG_ENDIANNESS = " << CFG_ENDIANNESS;
		}
	});
});
} // namespace
