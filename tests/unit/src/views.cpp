#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/string.hpp>
#include <utki/views.hpp>

using namespace std::string_literals;

namespace {
const tst::set set("views", [](tst::suite& suite) {
	suite.add("reverse_view", []() {
		std::array<int, 10> arr{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

		std::vector<int> result;

		for (const auto& i : utki::views::reverse(arr)) {
			result.push_back(i);
		}

		std::vector<int> expected = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

		tst::check(result == expected, SL);
	});

	suite.add("zip_view", []() {
		std::vector<int> vec = {0, 1, 2, 3, 4, 5, 6};
		std::array<float, 3> arr = {
			{3.0f, 4.0f, 5.0f}
		};
		std::vector<std::string> vecstr = {"1"s, "2"s, "3"s, "4"s};

		std::vector<std::string> result;

		for (auto [a, b, c] : utki::views::zip(vec, arr, vecstr)) {
			result.push_back(utki::cat(a, b, c));
			a += 1;
		}

		std::vector<std::string> expected = {"031"s, "142"s, "253"s};

		tst::check_eq(result.size(), expected.size(), SL);
		tst::check(result == expected, SL)
			<< "[0] = " << result[0] << ", [1] = " << result[1] << ", [2] = " << result[2];

		std::vector<int> vec_expected = {1, 2, 3, 3, 4, 5, 6};
		tst::check(vec == vec_expected, SL) << vec[0] << vec[1] << vec[2] << vec[3] << vec[4] << vec[5] << vec[6];
	});

	suite.add("zip_view__const", []() {
		const std::vector<int> vec = {0, 1, 2, 3, 4, 5, 6};
		const std::array<float, 3> arr = {
			{3.0f, 4.0f, 5.0f}
		};
		const std::vector<std::string> vecstr = {"1"s, "2"s, "3"s, "4"s};

		std::vector<std::string> result;

		for (auto [a, b, c] : utki::views::zip(vec, arr, vecstr)) {
			result.push_back(utki::cat(a, b, c));
		}

		std::vector<std::string> expected = {"031"s, "142"s, "253"s};

		tst::check_eq(result.size(), expected.size(), SL);
		tst::check(result == expected, SL)
			<< "[0] = " << result[0] << ", [1] = " << result[1] << ", [2] = " << result[2];
	});

	suite.add("zip_view__const_non_const_mix", []() {
		const std::vector<int> vec = {0, 1, 2, 3, 4, 5, 6};
		const std::array<float, 3> arr = {
			{3.0f, 4.0f, 5.0f}
		};
		std::vector<std::string> vecstr = {"1"s, "2"s, "3"s, "4"s};

		std::vector<std::string> result;

		for (const auto& [a, b, c] : utki::views::zip(vec, arr, vecstr)) {
			result.push_back(utki::cat(a, b, c));
		}

		std::vector<std::string> expected = {"031"s, "142"s, "253"s};

		tst::check_eq(result.size(), expected.size(), SL);
		tst::check(result == expected, SL)
			<< "[0] = " << result[0] << ", [1] = " << result[1] << ", [2] = " << result[2];
	});
});
} // namespace