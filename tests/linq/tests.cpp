#include "../../src/utki/linq.hpp"
#include "../../src/utki/debug.hpp"

#include "tests.hpp"

#include <vector>

namespace{
void test_select(){
#if __cplusplus >= 201703L
	// test select(&&)
	{
		std::vector<std::pair<int, std::string>> in = {{
			{13, "13"},
			{14, "14"},
			{1, "1"}
		}};

		auto out = utki::linq(std::move(in)).select(
				[](auto v){
					TRACE(<< "v.second = " << v.second << std::endl)
					return std::make_pair(std::move(v.second), 13.4f);
				}
			).get();

		static_assert(std::is_same<decltype(out), std::vector<std::pair<std::string, float>>>::value, "not same");

		TRACE(<< "select done" << std::endl)

		decltype(out) expected = {{
			{"13", 13.4f},
			{"14", 13.4f},
			{"1", 13.4f}
		}};

		ASSERT_ALWAYS(out == expected)
		ASSERT_ALWAYS(in.empty())
	}

	// test select(const&)
	{
		std::vector<std::pair<int, std::string>> in = {{
			{13, "13"},
			{14, "14"},
			{1, "1"}
		}};

		auto out = utki::select(
				in,
				[](auto v){
					return std::make_pair(v.second, 13.4f);
				}
			);

		decltype(out) expected = {{
			{"13", 13.4f},
			{"14", 13.4f},
			{"1", 13.4f}
		}};

		ASSERT_ALWAYS(out == expected)
		ASSERT_ALWAYS(!in[0].second.empty())
	}
#endif
}
}

void test_utki_linq(){
	test_select();
}
