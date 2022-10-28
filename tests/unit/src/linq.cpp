#include <list>

#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/config.hpp>
#include <utki/linq.hpp>

#if CFG_COMPILER != CFG_COMPILER_MSVC || CFG_COMPILER_MSVC_TOOLS_V >= 142

namespace {
tst::set set("linq", [](tst::suite& suite) {
	suite.add("linq_from_rvalue_select", []() {
		std::vector<std::pair<int, std::string>> in = {
			{13, "13"},
			{14, "14"},
			{ 1,  "1"}
        };

		auto out = utki::linq(std::move(in))
					   .select([](auto v) {
						   auto r = std::make_pair(std::move(v.second), 13.4f);
						   tst::check(v.second.empty(), SL);
						   return r;
					   })
					   .get();

		static_assert( //
			std::is_same<decltype(out), std::vector<std::pair<std::string, float>>>::value,
			"not same"
		);

		decltype(out) expected = {
			{"13", 13.4f},
			{"14", 13.4f},
			{ "1", 13.4f}
        };

		tst::check(out == expected, SL);
		tst::check(in.empty(), SL);
	});

	suite.add("linq_from_lvalue_select", []() {
		std::map<int, const std::string> in = {
			{13, "13"},
			{14, "14"},
			{ 1,  "1"}
        };

		auto out = utki::linq(in)
					   .select([](auto v, auto i) {
						   auto r = std::make_pair(std::move(v.second), i);
						   tst::check(!v.second.empty(), SL);
						   return r;
					   })
					   .get();

		// since original container is a map, the order of elements is sorted by map key
		decltype(out) expected = {
			{ "1", 0},
			{"13", 1},
			{"14", 2}
        };

		tst::check(
			out == expected,
			[&](auto& o) {
				o << "out.size() = " << out.size() << std::endl;
			},
			SL
		);
		tst::check(!in.empty(), SL);
		tst::check(!in.begin()->second.empty(), SL);
	});

	suite.add("linq_from_const_lvalue_select", []() {
		const std::list<std::pair<int, const std::string>> in = {
			{13, "13"},
			{14, "14"},
			{ 1,  "1"}
        };

		auto out = utki::linq(in)
					   .select([](auto v) {
						   auto r = std::make_pair(std::move(v.second), 13.4f);
						   tst::check(!v.second.empty(), SL);
						   return r;
					   })
					   .get();

		decltype(out) expected = {
			{"13", 13.4f},
			{"14", 13.4f},
			{ "1", 13.4f}
        };

		tst::check(out == expected, SL);
		tst::check(!in.empty(), SL);
		tst::check(!in.front().second.empty(), SL);
	});

	suite.add("linq_from_rvalue_group_by", []() {
		std::vector<std::pair<int, std::string>> in = {
			{ 13, "13"},
			{ 14, "14"},
			{  1,  "1"},
			{132, "13"},
			{ 15, "15"},
			{133, "13"},
			{ 11,  "1"}
        };

		auto out = utki::linq(std::move(in))
					   .select([](auto v, auto i) {
						   auto r = std::make_tuple(std::move(v.second), v.first, i);
						   tst::check(v.second.empty(), SL);
						   return r;
					   })
					   .group_by([](const auto& v) -> const std::string& { //
						   return std::get<0>(v);
					   })
					   .get();

		// clang-format off
		decltype(out) expected = {
			{ 
				"1",
				{
					{"1", 1, 2},
					{"1", 11, 6}
				}
			},
			{
				"13",
				{
					{"13", 13, 0},
					{"13", 132, 3},
					{"13", 133, 5}
				}
			},
			{
				"14",
				{
					{"14", 14, 1}
				}
			},
			{
				"15",
				{
					{"15", 15, 4}
				}
			}
		};
		// clang-format on

		tst::check(!out.empty(), SL);
		tst::check(out == expected, SL);
		tst::check(in.empty(), SL);
	});

	suite.add("linq_from_const_lvalue_where", []() {
		const std::list<std::pair<int, const std::string>> in = {
			{13,   "13"},
			{14,   "14"},
			{ 1,    "1"},
			{13, "13_2"},
			{ 3,    "3"},
			{13, "13_3"}
        };

		auto out = utki::linq(in) //
					   .where([](const decltype(in)::value_type& v) { //
						   return v.first == 13;
					   })
					   .get();

		decltype(out) expected = {
			{13,   "13"},
			{13, "13_2"},
			{13, "13_3"}
        };

		tst::check(out == expected, SL);
		tst::check(!in.empty(), SL);
		tst::check(!in.front().second.empty(), SL);
	});

	suite.add("linq_from_const_lvalue_group_by_where", []() {
		const std::list<std::pair<int, const std::string>> in = {
			{13,   "13"},
			{14,   "14"},
			{ 1,    "1"},
			{13, "13_2"},
			{ 3,    "3"},
			{ 3,  "3_2"}
        };

		auto out = utki::linq(in) //
					   .group_by([](const auto& v) { //
						   return v.first;
					   })
					   .where([](const auto& v) { //
						   return v.second.size() == 2;
					   })
					   .get();

		tst::check(!in.empty(), SL);
		tst::check(!in.front().second.empty(), SL);

		tst::check(
			out.size() == 2,
			[&](auto& o) {
				o << "out.size() = " << out.size();
			},
			SL
		);
		tst::check(
			out.begin()->first == 3,
			[&](auto& o) {
				o << "out.begin()->first = " << out.begin()->first;
			},
			SL
		);
		tst::check(
			std::next(out.begin())->first == 13,
			[&](auto& o) {
				o << "std::next(out.begin())->first = " << std::next(out.begin())->first;
			},
			SL
		);
		tst::check(
			out.begin()->second.size() == 2,
			[&](auto& o) {
				o << "out.begin()->second.size() = " << out.begin()->second.size();
			},
			SL
		);
	});

	suite.add("linq_from_rvalue_order_by", []() {
		std::vector<std::pair<int, std::string>> in = {
			{13,   "13"},
			{14,  "hgb"},
			{14, "asdh"},
			{ 5,  "asd"},
			{ 1,    "1"}
        };

		auto out = utki::linq(std::move(in)) //
					   .order_by([](const auto& v) -> const int& { //
						   return v.first;
					   })
					   .get();

		static_assert( //
			std::is_same<decltype(out), decltype(in)>::value,
			"not same"
		);

		decltype(out) expected = {
			{ 1,    "1"},
			{ 5,  "asd"},
			{13,   "13"},
			{14,  "hgb"},
			{14, "asdh"}
        };

#	ifdef DEBUG
		for (auto& e : out) {
			LOG([&](auto& o) { //
				o << "e = " << e.first << " " << e.second << std::endl;
			})
		}
#	endif

		tst::check(out == expected, SL);
		tst::check(in.empty(), SL);
	});

	suite.add("linq_from_lvalue_order_by", []() {
		std::vector<std::pair<int, std::string>> in = {
			{13,   "13"},
			{14,  "hgb"},
			{14, "asdh"},
			{ 5,  "asd"},
			{ 1,    "1"}
        };

		auto out = utki::linq(in) //
					   .order_by([](const auto& v) -> const int& { //
						   return v.first;
					   })
					   .get();

		static_assert( //
			std::is_same<decltype(out), decltype(in)>::value,
			"not same"
		);

		decltype(out) expected = {
			{ 1,    "1"},
			{ 5,  "asd"},
			{13,   "13"},
			{14,  "hgb"},
			{14, "asdh"}
        };

#	ifdef DEBUG
		for (auto& e : out) {
			LOG([&](auto& o) { //
				o << "e = " << e.first << " " << e.second << std::endl;
			})
		}
#	endif

		tst::check(out == expected, SL);
		tst::check(!in.empty(), SL);
		tst::check(!in.begin()->second.empty(), SL);
	});

	suite.add("linq_from_const_lvalue_order_by", []() {
		const std::vector<std::pair<int, std::string>> in = {
			{13,   "13"},
			{14,  "hgb"},
			{14, "asdh"},
			{ 5,  "asd"},
			{ 1,    "1"}
        };

		auto out = utki::linq(in) //
					   .order_by([](const auto& v) -> const int& { //
						   return v.first;
					   })
					   .get();

		static_assert( //
			std::is_same<decltype(out), std::remove_const<decltype(in)>::type>::value,
			"not same"
		);

		decltype(out) expected = {
			{ 1,    "1"},
			{ 5,  "asd"},
			{13,   "13"},
			{14,  "hgb"},
			{14, "asdh"}
        };

#	ifdef DEBUG
		for (auto& e : out) {
			LOG([&](auto& o) { //
				o << "e = " << e.first << " " << e.second << std::endl;
			})
		}
#	endif

		tst::check(out == expected, SL);
		tst::check(!in.empty(), SL);
		tst::check(!in.begin()->second.empty(), SL);
	});

	suite.add(
		"select_shared_ptr",
		[]() {
			struct test_struct {
				bool& destroyed;
				test_struct(bool& destroyed) :
					destroyed(destroyed)
				{}
				~test_struct()
				{
					this->destroyed = true;
				}
			};

			bool destroyed = false;

			struct wrapper {
				std::shared_ptr<test_struct> ts;
				int dummy;

				~wrapper() = default;

				[[nodiscard]]
				std::shared_ptr<test_struct> get_ts() const
				{
					return this->ts;
				}
			};

			std::vector<wrapper> vec{
				{std::make_shared<test_struct>(destroyed), 10},
				{std::make_shared<test_struct>(destroyed), 13}
            };

			auto res = utki::linq(vec) //
						   .select([](const auto& i) -> std::shared_ptr<const test_struct> { //
							   return i.get_ts();
						   })
						   .get();

			vec.clear();

			tst::check(!destroyed, SL);

			res.clear();

			tst::check(destroyed, SL);
		} // NOLINT(clang-analyzer-cplusplus.NewDeleteLeaks): clang-tidy sees potential memory leak here, for some
		  // reason...
	);
});
} // namespace

#endif // ~ non-MSVC compiler or MSVC compiler tools >= v142
