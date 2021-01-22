#include "../../src/utki/linq.hpp"
#include "../../src/utki/debug.hpp"

#include "tests.hpp"

#include <vector>

namespace{
void test_select(){
#if __cplusplus >= 201703L
	// test linq(&&).select()
	{
		std::vector<std::pair<int, std::string>> in = {{
			{13, "13"},
			{14, "14"},
			{1, "1"}
		}};

		auto out = utki::linq(std::move(in)).select(
				[](auto v){
					auto r = std::make_pair(std::move(v.second), 13.4f);
					ASSERT_ALWAYS(v.second.empty())
					return r;
				}
			).get();

		static_assert(std::is_same<decltype(out), std::vector<std::pair<std::string, float>>>::value, "not same");

		decltype(out) expected = {{
			{"13", 13.4f},
			{"14", 13.4f},
			{"1", 13.4f}
		}};

		ASSERT_ALWAYS(out == expected)
		ASSERT_ALWAYS(in.empty())
	}

	// test linq(&).select()
	{
		std::vector<std::pair<int, const std::string>> in = {{
			{13, "13"},
			{14, "14"},
			{1, "1"}
		}};

		auto out = utki::linq(in).select(
				[](auto v){
					auto r = std::make_pair(std::move(v.second), 13.4f);
					ASSERT_ALWAYS(!v.second.empty())
					return r;
				}
			).get();

		decltype(out) expected = {{
			{"13", 13.4f},
			{"14", 13.4f},
			{"1", 13.4f}
		}};

		ASSERT_ALWAYS(out == expected)
		ASSERT_ALWAYS(!in.empty())
		ASSERT_ALWAYS(!in[0].second.empty())
	}

	// test linq(const &).select()
	{
		const std::vector<std::pair<int, const std::string>> in = {{
			{13, "13"},
			{14, "14"},
			{1, "1"}
		}};

		auto out = utki::linq(in).select(
				[](auto v){
					auto r = std::make_pair(std::move(v.second), 13.4f);
					ASSERT_ALWAYS(!v.second.empty())
					return r;
				}
			).get();

		decltype(out) expected = {{
			{"13", 13.4f},
			{"14", 13.4f},
			{"1", 13.4f}
		}};

		ASSERT_ALWAYS(out == expected)
		ASSERT_ALWAYS(!in.empty())
		ASSERT_ALWAYS(!in[0].second.empty())
	}
#endif
}
}

void test_utki_linq(){
	test_select();
}
