#include <tst/check.hpp>
#include <tst/set.hpp>

namespace {
enum class test_enum {
	zeroth,
	first,
	second,
	third,
	fourth,
	fifth,
	sixth,
	seventh,
	eighth,
	nineth,
	tenth,
	eleventh,
	twelveth,
	thirteenth,
	fourteenth,
	fifteenth,
	sixteenth,
	seventeenth,
	nineteenth,
	twentyth,
	twenty_first,
	twenty_second,
	twenty_third,
	twenty_fourth,
	twenty_fifth,
	twenty_sixth,
	twenty_seventh,
	twenty_eighth,
	twenty_nineth,
	thirtyth,
	thirty_first,
	thirty_second,
	thirty_third,
	thirty_fourth,
	thirty_fifth,
	thirty_sixth,
	thirty_senenth,
	thirty_eighth,
	thirty_nineth,

	enum_size
};
} // namespace

namespace {
// NOLINTNEXTLINE(cppcoreguidelines-use-enum-class, "inentionally testing non-class enum")
enum non_flag_enum {
	item1,
	item2,
	item3,
	item4
};
} // namespace

namespace {
const tst::set set("flags", [](tst::suite& suite) {
	suite.add("basic", []() {
		utki::flags<test_enum> fs;

		fs.set(test_enum::eighth, true).set(test_enum::second, true).set(test_enum::eighth, false);
		tst::check(!fs.get(test_enum::eighth), SL);
		tst::check(fs.get(test_enum::second), SL);

		LOG([&](auto& o) {
			o << "enum_size = " << size_t(test_enum::enum_size) << " sizeof(fs) = " << sizeof(fs) << std::endl;
		})

		LOG([&](auto& o) {
			o << "fs = " << fs << std::endl;
		})

		{
			utki::flags<test_enum> fs;
			tst::check(fs.is_clear(), SL);
			tst::check(!fs.is_set(), SL);

			fs.set(test_enum::eighth, true);
			tst::check(!fs.is_clear(), SL);
			tst::check(!fs.is_set(), SL);
		}
		{
			utki::flags<test_enum> fs(true);
			tst::check(!fs.is_clear(), SL);
			tst::check(fs.is_set(), SL);

			fs.set();

			fs.set(test_enum::eighth, false);
			tst::check(!fs.is_clear(), SL);
			tst::check(!fs.is_set(), SL);
		}
	});

	suite.add("enum_type_is_typedefed", []() {
		utki::flags<test_enum> flags;

		decltype(flags)::enum_type t = test_enum::eighth;

		tst::check(t == test_enum::eighth, SL);
	});

	suite.add("empty_initializer_list", []() {
		auto flags = utki::flags<test_enum>({});

		tst::check(flags.is_clear(), SL);
	});

	suite.add("initializer_list", []() {
		auto flags = utki::flags<test_enum>({test_enum::twenty_seventh, test_enum::nineteenth});

		tst::check(!flags.is_clear(), SL);
		tst::check(flags.get(test_enum::twenty_seventh), SL);
		tst::check(flags[test_enum::twenty_seventh], SL);
		tst::check(flags.get(test_enum::nineteenth), SL);
		tst::check(flags[test_enum::nineteenth], SL);

		flags.clear(test_enum::twenty_seventh);
		tst::check(!flags.is_clear(), SL);
		tst::check(!flags.get(test_enum::twenty_seventh), SL);
		tst::check(!flags[test_enum::twenty_seventh], SL);
		tst::check(flags.get(test_enum::nineteenth), SL);
		tst::check(flags[test_enum::nineteenth], SL);

		flags.clear(test_enum::nineteenth);
		tst::check(flags.is_clear(), SL);
		tst::check(!flags.get(test_enum::twenty_seventh), SL);
		tst::check(!flags[test_enum::twenty_seventh], SL);
		tst::check(!flags.get(test_enum::nineteenth), SL);
		tst::check(!flags[test_enum::nineteenth], SL);
	});

	suite.add("make_flags", []() {
		auto flags = utki::make_flags({test_enum::twenty_seventh, test_enum::nineteenth});

		tst::check(!flags.is_clear(), SL);
		tst::check(flags.get(test_enum::twenty_seventh), SL);
		tst::check(flags.get(test_enum::nineteenth), SL);

		flags.clear(test_enum::twenty_seventh);
		tst::check(!flags.is_clear(), SL);
		tst::check(!flags.get(test_enum::twenty_seventh), SL);
		tst::check(flags.get(test_enum::nineteenth), SL);

		flags[test_enum::nineteenth] = false;
		tst::check(flags.is_clear(), SL);
		tst::check(!flags.get(test_enum::twenty_seventh), SL);
		tst::check(!flags.get(test_enum::nineteenth), SL);
	});

	suite.add("twice_initialized_flag", []() {
		auto flags = utki::make_flags({test_enum::nineteenth, test_enum::twenty_seventh, test_enum::nineteenth});

		tst::check(!flags.is_clear(), SL);
		tst::check(flags.get(test_enum::twenty_seventh), SL);
		tst::check(flags.get(test_enum::nineteenth), SL);

		flags.clear(test_enum::twenty_seventh);
		tst::check(!flags.is_clear(), SL);
		tst::check(!flags.get(test_enum::twenty_seventh), SL);
		tst::check(flags.get(test_enum::nineteenth), SL);

		flags.clear(test_enum::nineteenth);
		tst::check(flags.is_clear(), SL);
		tst::check(!flags.get(test_enum::twenty_seventh), SL);
		tst::check(!flags.get(test_enum::nineteenth), SL);
	});

	suite.add("operator_or_on_enum", []() {
		auto flags = utki::make_flags({test_enum::nineteenth, test_enum::twenty_seventh, test_enum::nineteenth});

		tst::check(!flags.get(test_enum::eighth), SL);

		flags = flags | test_enum::eighth;

		tst::check(flags.get(test_enum::eighth), SL);
	});

	suite.add("constructor_from_enum", []() {
		utki::flags<test_enum> flags(test_enum::eleventh);

		tst::check(flags.get(test_enum::eleventh), SL);

		flags.clear(test_enum::eleventh);

		tst::check(flags.is_clear(), SL);
	});

	suite.add("free_operator_or_on_enum", []() {
		auto flags = test_enum::eighth | test_enum::eleventh;

		tst::check(flags.get(test_enum::eighth), SL);
		tst::check(flags.get(test_enum::eleventh), SL);

		flags.clear(test_enum::eighth);
		flags.clear(test_enum::eleventh);

		tst::check(flags.is_clear(), SL);
	});

	suite.add("operator_or_is_not_applied_to_enums_without_enum_size_item", []() {
		auto flags = non_flag_enum::item1 | non_flag_enum::item2;

		tst::check_eq(flags, int(1), SL);
	});

	suite.add("operator_not_equals", []() {
		utki::flags<test_enum> flags1{test_enum::eighth, test_enum::eleventh};
		tst::check(!flags1.is_clear(), SL);
		tst::check(!flags1.is_set(), SL);

		utki::flags<test_enum> flags2;
		tst::check(flags2.is_clear(), SL);
		tst::check(!flags2.is_set(), SL);

		tst::check_ne(flags1, flags2, SL);

		tst::check_ne(flags1, {test_enum::eighth}, SL);
		tst::check_ne(flags1, {test_enum::eighth, test_enum::twelveth}, SL);
		tst::check_ne(flags1, {test_enum::eighth, test_enum::twelveth, test_enum::twenty_fifth}, SL);

		tst::check_ne(flags1, test_enum::eighth | test_enum::twelveth, SL);
		tst::check_ne(flags1, test_enum::eighth | test_enum::twelveth | test_enum::twenty_fifth, SL);
	});

	suite.add("operator_equals_equals", []() {
		utki::flags<test_enum> flags1{test_enum::eighth, test_enum::eleventh};
		tst::check(!flags1.is_clear(), SL);
		tst::check(!flags1.is_set(), SL);

		utki::flags<test_enum> flags2;
		tst::check(flags2.is_clear(), SL);
		tst::check(!flags2.is_set(), SL);

		flags2.set(test_enum::eighth);
		flags2.set(test_enum::eleventh);

		tst::check_eq(flags1, flags2, SL);

		tst::check_eq(flags1, {test_enum::eighth, test_enum::eleventh}, SL);

		tst::check_eq(flags1, test_enum::eighth | test_enum::eleventh, SL);
	});
});
} // namespace
