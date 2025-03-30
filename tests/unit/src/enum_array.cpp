#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/enum_array.hpp>

using namespace std::string_literals;

namespace {
const tst::set set("enum_array", [](tst::suite& suite) {
	suite.add("zip_with_enum", []() {
		enum class en {
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
		enum class en {
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

	suite.add("operator_subscript", []() {
		enum class en {
			a,
			b,
			c,
			d,

			enum_size
		};

		utki::enum_array<std::string, en> arr = {"a", "b", "c", "d"};

		tst::check_eq(arr[0], "a"s, SL);
		tst::check_eq(arr[1], "b"s, SL);
		tst::check_eq(arr[2], "c"s, SL);
		tst::check_eq(arr[3], "d"s, SL);

		tst::check_eq(arr[en::a], "a"s, SL);
		tst::check_eq(arr[en::b], "b"s, SL);
		tst::check_eq(arr[en::c], "c"s, SL);
		tst::check_eq(arr[en::d], "d"s, SL);

		arr[en::a].append("!");

		tst::check_eq(arr[en::a], "a!"s, SL);
	});

	suite.add("operator_subscript__const", []() {
		enum class en {
			a,
			b,
			c,
			d,

			enum_size
		};

		const utki::enum_array<std::string, en> arr = {"a", "b", "c", "d"};

		tst::check_eq(arr[0], "a"s, SL);
		tst::check_eq(arr[1], "b"s, SL);
		tst::check_eq(arr[2], "c"s, SL);
		tst::check_eq(arr[3], "d"s, SL);

		tst::check_eq(arr[en::a], "a"s, SL);
		tst::check_eq(arr[en::b], "b"s, SL);
		tst::check_eq(arr[en::c], "c"s, SL);
		tst::check_eq(arr[en::d], "d"s, SL);
	});
});
} // namespace
