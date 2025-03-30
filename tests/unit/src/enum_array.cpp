#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/enum_array.hpp>

namespace {
const tst::set set("enum_array", [](tst::suite& suite) {
	suite.add("zip_with_enum", []() {
		enum en {
			a,
			b,
			c,
			d,

			enum_size
		};

		utki::enum_array<std::string, en> arr = {"a", "b", "c", "d"};

		tst::check_eq(arr.size(), size_t(en::enum_size), SL);

		std::vector<std::pair<std::string, en>> actual;

		const std::vector<std::pair<std::string, en>> expected = {
			{"a", en::a},
			{"b", en::b},
			{"c", en::c},
			{"d", en::d},
		};

		for (auto [v, e] : arr.zip_with_enum()) {
			actual.emplace_back(v, e);

			// modify original array contents
			v.append("!");
		}

		tst::check(actual == expected, SL);

		for (const auto& i : arr) {
			tst::check_eq(i.size(), size_t(2), SL);
			tst::check_eq(i.back(), '!', SL);
		}
	});

	suite.add("zip_with_enum__const", []() {
		enum en {
			a,
			b,
			c,
			d,

			enum_size
		};

		const utki::enum_array<std::string, en> arr = {"a", "b", "c", "d"};

		tst::check_eq(arr.size(), size_t(en::enum_size), SL);

		std::vector<std::pair<std::string, en>> actual;

		const std::vector<std::pair<std::string, en>> expected = {
			{"a", en::a},
			{"b", en::b},
			{"c", en::c},
			{"d", en::d},
		};

		for (auto [v, e] : arr.zip_with_enum()) {
			actual.emplace_back(v, e);
		}

		tst::check(actual == expected, SL);
	});
});
} // namespace
