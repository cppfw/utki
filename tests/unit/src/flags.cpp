#include <tst/check.hpp>
#include <tst/set.hpp>

namespace {
enum class TestEnum {
	ZEROTH,
	FIRST,
	SECOND,
	THIRD,
	FOURTH,
	FIFTH,
	SIXTH,
	SENENTH,
	EIGHTH,
	NINETH,
	TENTH,
	ELEVENTH,
	TWELVETH,
	THIRTEENTH,
	FOURTEENTH,
	FIFTEENTH,
	SIXTEENTH,
	SEVENTEENTH,
	NINETEENTH,
	TWENTYTH,
	TWENTY_FIRST,
	TWENTY_SECOND,
	TWENTY_THIRD,
	TWENTY_FOURTH,
	TWENTY_FIFTH,
	TWENTY_SIXTH,
	TWENTY_SEVENTH,
	TWENTY_EIGHTH,
	TWENTY_NINETH,
	THIRTYTH,
	THIRTY_FIRST,
	THIRTY_SECOND,
	THIRTY_THIRD,
	THIRTY_FOURTH,
	THIRTY_FIFTH,
	THIRTY_SIXTH,
	THIRTY_SENENTH,
	THIRTY_EIGHTH,
	THIRTY_NINETH,

	enum_size
};
} // namespace

namespace {
enum non_flag_enum {
	item1,
	item2,
	item3,
	item4
};
} // namespace

namespace {
tst::set set("flags", [](tst::suite& suite) {
	suite.add("basic", []() {
		utki::flags<TestEnum> fs;

		fs.set(TestEnum::EIGHTH, true).set(TestEnum::SECOND, true).set(TestEnum::EIGHTH, false);
		tst::check(!fs.get(TestEnum::EIGHTH), SL);
		tst::check(fs.get(TestEnum::SECOND), SL);

		LOG([&](auto& o) {
			o << "enum_size = " << size_t(TestEnum::enum_size) << " sizeof(fs) = " << sizeof(fs) << std::endl;
		})

		LOG([&](auto& o) { o << "fs = " << fs << std::endl; })

		{
			utki::flags<TestEnum> fs;
			tst::check(fs.is_clear(), SL);
			tst::check(!fs.is_set(), SL);

			fs.set(TestEnum::EIGHTH, true);
			tst::check(!fs.is_clear(), SL);
			tst::check(!fs.is_set(), SL);
		}
		{
			utki::flags<TestEnum> fs(true);
			tst::check(!fs.is_clear(), SL);
			tst::check(fs.is_set(), SL);

			fs.set();

			fs.set(TestEnum::EIGHTH, false);
			tst::check(!fs.is_clear(), SL);
			tst::check(!fs.is_set(), SL);
		}
	});

	suite.add("enum_type_is_typedefed", []() {
		utki::flags<TestEnum> flags;

		decltype(flags)::enum_type t = TestEnum::EIGHTH;

		tst::check(t == TestEnum::EIGHTH, SL);
	});

	suite.add("empty_initializer_list", []() {
		auto flags = utki::flags<TestEnum>({});

		tst::check(flags.is_clear(), SL);
	});

	suite.add("initializer_list", []() {
		auto flags = utki::flags<TestEnum>({TestEnum::TWENTY_SEVENTH, TestEnum::NINETEENTH});

		tst::check(!flags.is_clear(), SL);
		tst::check(flags.get(TestEnum::TWENTY_SEVENTH), SL);
		tst::check(flags[TestEnum::TWENTY_SEVENTH], SL);
		tst::check(flags.get(TestEnum::NINETEENTH), SL);
		tst::check(flags[TestEnum::NINETEENTH], SL);

		flags.clear(TestEnum::TWENTY_SEVENTH);
		tst::check(!flags.is_clear(), SL);
		tst::check(!flags.get(TestEnum::TWENTY_SEVENTH), SL);
		tst::check(!flags[TestEnum::TWENTY_SEVENTH], SL);
		tst::check(flags.get(TestEnum::NINETEENTH), SL);
		tst::check(flags[TestEnum::NINETEENTH], SL);

		flags.clear(TestEnum::NINETEENTH);
		tst::check(flags.is_clear(), SL);
		tst::check(!flags.get(TestEnum::TWENTY_SEVENTH), SL);
		tst::check(!flags[TestEnum::TWENTY_SEVENTH], SL);
		tst::check(!flags.get(TestEnum::NINETEENTH), SL);
		tst::check(!flags[TestEnum::NINETEENTH], SL);
	});

	suite.add("make_flags", []() {
		auto flags = utki::make_flags({TestEnum::TWENTY_SEVENTH, TestEnum::NINETEENTH});

		tst::check(!flags.is_clear(), SL);
		tst::check(flags.get(TestEnum::TWENTY_SEVENTH), SL);
		tst::check(flags.get(TestEnum::NINETEENTH), SL);

		flags.clear(TestEnum::TWENTY_SEVENTH);
		tst::check(!flags.is_clear(), SL);
		tst::check(!flags.get(TestEnum::TWENTY_SEVENTH), SL);
		tst::check(flags.get(TestEnum::NINETEENTH), SL);

		flags[TestEnum::NINETEENTH] = false;
		tst::check(flags.is_clear(), SL);
		tst::check(!flags.get(TestEnum::TWENTY_SEVENTH), SL);
		tst::check(!flags.get(TestEnum::NINETEENTH), SL);
	});

	suite.add("twice_initialized_flag", []() {
		auto flags = utki::make_flags({TestEnum::NINETEENTH, TestEnum::TWENTY_SEVENTH, TestEnum::NINETEENTH});

		tst::check(!flags.is_clear(), SL);
		tst::check(flags.get(TestEnum::TWENTY_SEVENTH), SL);
		tst::check(flags.get(TestEnum::NINETEENTH), SL);

		flags.clear(TestEnum::TWENTY_SEVENTH);
		tst::check(!flags.is_clear(), SL);
		tst::check(!flags.get(TestEnum::TWENTY_SEVENTH), SL);
		tst::check(flags.get(TestEnum::NINETEENTH), SL);

		flags.clear(TestEnum::NINETEENTH);
		tst::check(flags.is_clear(), SL);
		tst::check(!flags.get(TestEnum::TWENTY_SEVENTH), SL);
		tst::check(!flags.get(TestEnum::NINETEENTH), SL);
	});

	suite.add("operator_or_on_enum", []() {
		auto flags = utki::make_flags({TestEnum::NINETEENTH, TestEnum::TWENTY_SEVENTH, TestEnum::NINETEENTH});

		tst::check(!flags.get(TestEnum::EIGHTH), SL);

		flags = flags | TestEnum::EIGHTH;

		tst::check(flags.get(TestEnum::EIGHTH), SL);
	});

	suite.add("constructor_from_enum", []() {
		utki::flags<TestEnum> flags(TestEnum::ELEVENTH);

		tst::check(flags.get(TestEnum::ELEVENTH), SL);

		flags.clear(TestEnum::ELEVENTH);

		tst::check(flags.is_clear(), SL);
	});

	suite.add("free_operator_or_on_enum", []() {
		auto flags = TestEnum::EIGHTH | TestEnum::ELEVENTH;

		tst::check(flags.get(TestEnum::EIGHTH), SL);
		tst::check(flags.get(TestEnum::ELEVENTH), SL);

		flags.clear(TestEnum::EIGHTH);
		flags.clear(TestEnum::ELEVENTH);

		tst::check(flags.is_clear(), SL);
	});

	suite.add("operator_or_is_not_applied_to_enums_without_enum_size_item", []() {
		auto flags = non_flag_enum::item1 | non_flag_enum::item2;

		tst::check_eq(flags, int(1), SL);
	});

	suite.add("operator_not_equals", []() {
		utki::flags<TestEnum> flags1{TestEnum::EIGHTH, TestEnum::ELEVENTH};
		tst::check(!flags1.is_clear(), SL);
		tst::check(!flags1.is_set(), SL);

		utki::flags<TestEnum> flags2;
		tst::check(flags2.is_clear(), SL);
		tst::check(!flags2.is_set(), SL);

		tst::check_ne(flags1, flags2, SL);

		tst::check_ne(flags1, {TestEnum::EIGHTH}, SL);
		tst::check_ne(flags1, {TestEnum::EIGHTH, TestEnum::TWELVETH}, SL);
		tst::check_ne(flags1, {TestEnum::EIGHTH, TestEnum::TWELVETH, TestEnum::THIRTY_FIFTH}, SL);

		tst::check_ne(flags1, TestEnum::EIGHTH | TestEnum::TWELVETH, SL);
		tst::check_ne(flags1, TestEnum::EIGHTH | TestEnum::TWELVETH | TestEnum::THIRTY_FIFTH, SL);
	});

	suite.add("operator_equals_equals", []() {
		utki::flags<TestEnum> flags1{TestEnum::EIGHTH, TestEnum::ELEVENTH};
		tst::check(!flags1.is_clear(), SL);
		tst::check(!flags1.is_set(), SL);

		utki::flags<TestEnum> flags2;
		tst::check(flags2.is_clear(), SL);
		tst::check(!flags2.is_set(), SL);

		flags2.set(TestEnum::EIGHTH);
		flags2.set(TestEnum::ELEVENTH);

		tst::check_eq(flags1, flags2, SL);

		tst::check_eq(flags1, {TestEnum::EIGHTH, TestEnum::ELEVENTH}, SL);

		tst::check_eq(flags1, TestEnum::EIGHTH | TestEnum::ELEVENTH, SL);
	});
});
} // namespace
