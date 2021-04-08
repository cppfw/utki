#include "../../src/utki/linq.hpp"
#include "../../src/utki/debug.hpp"

#include "tests.hpp"

#include <vector>
#include <list>
#include <map>

#if __cplusplus >= 201703L
namespace{
void test_select(){
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
		std::map<int, const std::string> in = {{
			{13, "13"},
			{14, "14"},
			{1, "1"}
		}};

		auto out = utki::linq(in).select(
				[](auto v, auto i){
					auto r = std::make_pair(std::move(v.second), i);
					ASSERT_ALWAYS(!v.second.empty())
					return r;
				}
			).get();

		// since original container is a map, the order of elements is sorted by map key
		decltype(out) expected = {{
			{"1", 0},
			{"13", 1},
			{"14", 2}
		}};

		utki::assert(out == expected, [&](auto&o){o << "out.size() = " << out.size() << std::endl;}, SL);
		utki::assert(!in.empty(), SL);
		utki::assert(!in.begin()->second.empty(), SL);
	}

	// test linq(const &).select()
	{
		const std::list<std::pair<int, const std::string>> in = {{
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
		ASSERT_ALWAYS(!in.front().second.empty())
	}
}
}

namespace{
void test_group_by(){
	// test linq(&&).group_by()
	{
		std::vector<std::pair<int, std::string>> in = {{
			{13, "13"},
			{14, "14"},
			{1, "1"},
			{132, "13"},
			{15, "15"},
			{133, "13"},
			{11, "1"}
		}};

		auto out = utki::linq(std::move(in)).select(
				[](auto v, auto i){
					auto r = std::make_tuple(std::move(v.second), v.first, i);
					ASSERT_ALWAYS(v.second.empty())
					return r;
				}
			).group_by(
				[](const auto& v)->const std::string&{
					return std::get<0>(v);
				}
			).get();

		decltype(out) expected = {{
			{"1", {
				{"1", 1, 2}, {"1", 11, 6}
			} },
			{"13", {
				{"13", 13, 0}, {"13", 132, 3}, {"13", 133, 5}
			} },
			{"14", {
				{"14", 14, 1}
			} },
			{"15", {
				{"15", 15, 4}
			} }
		}};

		ASSERT_ALWAYS(!out.empty())
		ASSERT_ALWAYS(out == expected)
		ASSERT_ALWAYS(in.empty())
	}
}
}

namespace{
void test_where(){
	// test linq(const &).where()
	{
		const std::list<std::pair<int, const std::string>> in = {{
			{13, "13"},
			{14, "14"},
			{1, "1"},
			{13, "13_2"},
			{3, "3"},
			{13, "13_3"}
		}};

		auto out = utki::linq(in).where([](const decltype(in)::value_type& v){return v.first == 13;}).get();

		decltype(out) expected = {{
			{13, "13"},
			{13, "13_2"},
			{13, "13_3"}
		}};

		ASSERT_ALWAYS(out == expected)
		ASSERT_ALWAYS(!in.empty())
		ASSERT_ALWAYS(!in.front().second.empty())
	}

	// test linq(const &).group_by().where()
	{
		const std::list<std::pair<int, const std::string>> in = {{
			{13, "13"},
			{14, "14"},
			{1, "1"},
			{13, "13_2"},
			{3, "3"},
			{3, "3_2"}
		}};

		auto out = utki::linq(in).group_by([](const auto& v){return v.first;}).where([](const auto& v){return v.second.size() == 2;}).get();

		ASSERT_ALWAYS(!in.empty())
		ASSERT_ALWAYS(!in.front().second.empty())

		utki::assert(out.size() == 2, [&](auto&o){o << "out.size() = " << out.size();}, SL);
		utki::assert(out.begin()->first == 3, [&](auto&o){o << "out.begin()->first = " << out.begin()->first;}, SL);
		utki::assert(std::next(out.begin())->first == 13, [&](auto&o){o << "std::next(out.begin())->first = " << std::next(out.begin())->first;}, SL);
		utki::assert(out.begin()->second.size() == 2, [&](auto&o){o << "out.begin()->second.size() = " << out.begin()->second.size();}, SL);
	}
}
}

namespace{
void test_order_by(){
	// test linq(&&).order_by()
	{
		std::vector<std::pair<int, std::string>> in = {{
			{13, "13"},
			{14, "hgb"},
			{14, "asdh"},
			{5, "asd"},
			{1, "1"}
		}};

		auto out = utki::linq(std::move(in)).order_by(
				[](const auto& v) -> const int&{
					return v.first;
				}
			).get();

		static_assert(std::is_same<decltype(out), decltype(in)>::value, "not same");

		decltype(out) expected = {{
			{1, "1"},
			{5, "asd"},
			{13, "13"},
			{14, "hgb"},
			{14, "asdh"}
		}};

#ifdef DEBUG
		for(auto& e : out){
			LOG([&](auto&o){o << "e = " << e.first << " " << e.second << std::endl;})
		}
#endif

		ASSERT_ALWAYS(out == expected)
		ASSERT_ALWAYS(in.empty())
	}

	// test linq(&).order_by()
	{
		std::vector<std::pair<int, std::string>> in = {{
			{13, "13"},
			{14, "hgb"},
			{14, "asdh"},
			{5, "asd"},
			{1, "1"}
		}};

		auto out = utki::linq(in).order_by(
				[](const auto& v) -> const int&{
					return v.first;
				}
			).get();

		static_assert(std::is_same<decltype(out), decltype(in)>::value, "not same");

		decltype(out) expected = {{
			{1, "1"},
			{5, "asd"},
			{13, "13"},
			{14, "hgb"},
			{14, "asdh"}
		}};

#ifdef DEBUG
		for(auto& e : out){
			LOG([&](auto&o){o << "e = " << e.first << " " << e.second << std::endl;})
		}
#endif

		ASSERT_ALWAYS(out == expected)
		ASSERT_ALWAYS(!in.empty())
		ASSERT_ALWAYS(!in.begin()->second.empty())
	}

	// test linq(const &).order_by()
	{
		const std::vector<std::pair<int, std::string>> in = {{
			{13, "13"},
			{14, "hgb"},
			{14, "asdh"},
			{5, "asd"},
			{1, "1"}
		}};

		auto out = utki::linq(in).order_by(
				[](const auto& v) -> const int&{
					return v.first;
				}
			).get();

		static_assert(std::is_same<decltype(out), std::remove_const<decltype(in)>::type>::value, "not same");

		decltype(out) expected = {{
			{1, "1"},
			{5, "asd"},
			{13, "13"},
			{14, "hgb"},
			{14, "asdh"}
		}};

#ifdef DEBUG
		for(auto& e : out){
			LOG([&](auto&o){o << "e = " << e.first << " " << e.second << std::endl;})
		}
#endif

		ASSERT_ALWAYS(out == expected)
		ASSERT_ALWAYS(!in.empty())
		ASSERT_ALWAYS(!in.begin()->second.empty())
	}
}
}

void test_utki_linq(){
	test_select();
	test_group_by();
	test_where();
	test_order_by();
}
#else
void test_utki_linq(){}
#endif
