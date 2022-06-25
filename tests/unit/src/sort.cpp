#include <array>
#include <string_view>

#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/sort.hpp>

using namespace std::string_view_literals;

#if M_COMPILER != M_COMPILER_MSVC || M_COMPILER_MSVC_TOOLS_V >= 142
namespace {
namespace test_compile_time_sorting {
struct str_num_pair {
	std::string_view first;
	int second;

	constexpr bool operator==(const str_num_pair& p) const
	{
		return this->first == p.first && this->second == p.second;
	}
};

constexpr auto arr = []() constexpr
{
	std::array<str_num_pair, 5> arr = {
		{{"hello"sv, 13}, {"bye"sv, 15}, {"how"sv, 33}, {"are"sv, 4}, {"you"sv, 9}}
    };

	utki::sort( //
		arr.begin(),
		arr.end(),
		[](const auto& a, const auto& b) { //
			return a.first < b.first;
		}
	);
	return arr;
}
();

constexpr decltype(arr) expected = {
	{{"are", 4}, {"bye", 15}, {"hello", 13}, {"how", 33}, {"you", 9}}
};

static_assert(
	// the std::array::operator==() is not constexpr in C++17 (it is in C++20), so we need to compare element by element
	[] {
		if (arr.size() != expected.size()) {
			return false;
		}
		for (auto i = arr.begin(), j = expected.begin(); i != arr.end(); ++i, ++j) {
			if (*i == *j) {
				continue;
			}
			return false;
		}
		return true;
	}(),
	"sorted array is not as expected"
);
} // namespace test_compile_time_sorting
} // namespace
#endif

namespace {
tst::set set("sort", [](tst::suite& suite) {
	suite.add("sort_strings_with_default_comparator", [] {
		std::array<std::string, 5> arr = {
			{
             "hello", "bye",
             "how", "are",
             "you", }
        };

		utki::sort(arr.begin(), arr.end());

		decltype(arr) expected = {
			{
             "are", "bye",
             "hello", "how",
             "you", }
        };

		tst::check(arr == expected, SL);
	});

	suite.add("sort_strings_with_custom_comparator", [] {
		std::array<std::pair<std::string, int>, 5> arr = {
			{{"hello", 13}, {"bye", 15}, {"how", 33}, {"are", 4}, {"you", 9}}
        };

		utki::sort( //
			arr.begin(),
			arr.end(),
			[](const auto& a, const auto& b) {
				return a.first < b.first;
			}
		);

		decltype(arr) expected = {
			{{"are", 4}, {"bye", 15}, {"hello", 13}, {"how", 33}, {"you", 9}}
        };

		tst::check(arr == expected, SL);
	});

	suite.add("sort_integers_with_custom_comparator", [] {
		std::array<std::pair<std::string, int>, 5> arr = {
			{
             {"hello", 13},
             {"bye", 15},
             {"how", 33},
             {"are", 4},
             {"you", 9},
			 }
        };

		utki::sort( //
			arr.begin(),
			arr.end(),
			[](const auto& a, const auto& b) { //
				return a.second < b.second;
			}
		);

		decltype(arr) expected = {
			{
             {"are", 4},
             {"you", 9},
             {"hello", 13},
             {"bye", 15},
             {"how", 33},
			 }
        };

		tst::check(arr == expected, SL);
	});
});
} // namespace
