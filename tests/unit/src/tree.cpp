#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/tree.hpp>

using namespace std::string_literals;

namespace {
tst::set set("tree", [](tst::suite& suite) {
	suite.add("tree_node_default_constructor", []() {
		utki::tree<int> t;
		t.children.emplace_back(35);
		tst::check_eq(t.children.size(), size_t(1), SL);
	});

	suite.add("tree_node_value_and_initializer_list_constructor", []() {
		utki::tree<int> t(1, {34, 45});
		tst::check_eq(
			t.value,
			1,
			[&](auto& o) {
				o << "t.value = " << t.value;
			},
			SL
		);
	});

	suite.add("tree_structure_construction", []() {
		// clang-format off
		const utki::tree<int> t{
			{34, 45},
			{
				{78, 89, 96},
				{32, 64, 128},
				decltype(t)(42, {98, 99, 100})
			}
		};
		// clang-format on

		tst::check_eq(t.children.size(), size_t(2), SL);

		tst::check_eq(t.children[0].children.size(), size_t(2), SL);
		tst::check_eq(t.children[0].children[0].value, 34, SL);
		tst::check_eq(t.children[0].children[1].value, 45, SL);

		tst::check_eq(t.children[1].children.size(), size_t(3), SL);
		tst::check_eq(t.children[1].children[0].children.size(), size_t(3), SL);
		tst::check_eq(t.children[1].children[1].children.size(), size_t(3), SL);

		tst::check_eq(t.children[1].children[2].children.size(), size_t(3), SL);
		tst::check_eq(t.children[1].children[2].value, 42, SL);

		tst::check_eq(t.children[1].children[2].children[0].value, 98, SL);
		tst::check_eq(t.children[1].children[2].children[1].value, 99, SL);
		tst::check_eq(t.children[1].children[2].children[2].value, 100, SL);
	});

	suite.add("move_assignment", []() {
		// clang-format off
		utki::tree<int> t{
			{34, 45},
			{
				{78, 89, 96},
				{32, 64, 128},
				decltype(t)(42, {98, 99, 100})
			}
		};
		// clang-format on

		t.children[0].children = std::move(t.children[1].children);

		tst::check_eq(t.children.size(), size_t(2), SL);

		tst::check_eq(t.children[0].children.size(), size_t(3), SL);
		tst::check_eq(t.children[0].children[0].children.size(), size_t(3), SL);
		tst::check_eq(t.children[0].children[1].children.size(), size_t(3), SL);

		tst::check_eq(t.children[0].children[2].children.size(), size_t(3), SL);
		tst::check_eq(t.children[0].children[2].value, 42, SL);

		tst::check_eq(t.children[0].children[2].children[0].value, 98, SL);
		tst::check_eq(t.children[0].children[2].children[1].value, 99, SL);
		tst::check_eq(t.children[0].children[2].children[2].value, 100, SL);

		tst::check_eq(t.children[1].children.size(), size_t(0), SL);
	});

	suite.add("assignment", []() {
		// clang-format off
		utki::tree<int> t{
			{34, 45},
			{
				{78, 89, 96},
				{32, 64, 128},
				decltype(t)(42, {98, 99, 100})
			}
        };
		// clang-format on

		t.children[0].children = t.children[1].children;

		tst::check_eq(t.children.size(), size_t(2), SL);

		tst::check_eq(t.children[0].children.size(), size_t(3), SL);
		tst::check_eq(t.children[0].children[0].children.size(), size_t(3), SL);
		tst::check_eq(t.children[0].children[1].children.size(), size_t(3), SL);

		tst::check_eq(t.children[0].children[2].children.size(), size_t(3), SL);
		tst::check_eq(t.children[0].children[2].value, 42, SL);

		tst::check_eq(t.children[0].children[2].children[0].value, 98, SL);
		tst::check_eq(t.children[0].children[2].children[1].value, 99, SL);
		tst::check_eq(t.children[0].children[2].children[2].value, 100, SL);

		tst::check_eq(t.children[1].children.size(), size_t(3), SL);
	});

	suite.add("tree_to_value_comparison", []() {
		utki::tree<int> t{
			{34, 45},
			{{78, 89, 96}, {32, 64, 128}, decltype(t)(42, {98, 99, 100})}
        };

		tst::check_eq(t.children.size(), size_t(2), SL);
		tst::check_eq(t.children[0].children.size(), size_t(2), SL);
		tst::check_eq(
			t.children[0].children[0].value,
			34,
			[&](auto& o) {
				o << "value = " << t.children[0].value;
			},
			SL
		);

		tst::check_eq(t.children[1].children.size(), size_t(3), SL);
		tst::check_eq(t.children[1].children[0].children.size(), size_t(3), SL);
		tst::check_eq(t.children[1].children[1].children.size(), size_t(3), SL);

		tst::check_eq(t.children[1].children[2].children.size(), size_t(3), SL);
		tst::check(t.children[1].children[2] == 42, SL);

		tst::check(t.children[1].children[2].children[0] == 98, SL);
		tst::check(t.children[1].children[2].children[1] == 99, SL);
		tst::check(t.children[1].children[2].children[2] == 100, SL);

		tst::check_eq(t.children[1].children.size(), size_t(3), SL);
	});

	suite.add("tree_to_non_value_comparison", []() {
		struct wrapper {
			std::string str;

			bool operator==(const char* str) const
			{
				return this->str == str;
			}
		};

		std::string str = "hello";

		utki::tree<wrapper> t({str});

		// this should use template tree::operator==()
		tst::check(t == str.c_str(), SL);
	});

	suite.add("tree_to_tree_comparison", []() {
		utki::tree<int> t1(
			10,
			{utki::tree<int>(13, {34, 45}),
			 utki::tree<int>(
				 11,
				 {utki::tree<int>(14, {78, 89, 96}),
				  utki::tree<int>(15, {32, 64, 128}),
				  utki::tree<int>(42, {98, 99, 100})}
			 )}
		);

		utki::tree<int> t2(
			10,
			{utki::tree<int>(13, {34, 45}),
			 utki::tree<int>(
				 11,
				 {utki::tree<int>(14, {78, 89, 96}),
				  utki::tree<int>(15, {32, 64, 128}),
				  utki::tree<int>(42, {98, 99, 100})}
			 )}
		);

		tst::check_eq(t1.children[0].value, t2.children[0].value, SL);
		tst::check_eq(t1.children[0].children[0].value, t2.children[0].children[0].value, SL);
		tst::check_eq(t1.children[0].children[1].value, t2.children[0].children[1].value, SL);
		tst::check(t1 == t2, SL);
		tst::check(t1 == t1, SL);
	});

	suite.add("constructor_from_container_lvalue_lvalue", []() {
		typedef utki::tree<int> tree_int;
		tree_int::container_type children = { //
			tree_int(10),
			tree_int(20)};
		tst::check_eq(children.size(), size_t(2), SL);
		tree_int t(13, children);

		tst::check_eq(t.value, 13, SL);
		tst::check_eq(
			t.children.size(),
			size_t(2),
			[&](auto& o) {
				o << "t.children.size() = " << t.children.size();
			},
			SL
		);
		tst::check_eq(
			t.children[0].value,
			10,
			[&](auto& o) {
				o << "t.children[0].value = " << t.children[0].value;
			},
			SL
		);
		tst::check_eq(
			t.children[1].value,
			20,
			[&](auto& o) {
				o << "t.children[1].value = " << t.children[1].value;
			},
			SL
		);
	});

	suite.add("constructor_from_container_lvalue_rvalue", []() {
		typedef utki::tree<int> tree_int;
		tree_int::container_type children = { //
			tree_int(10),
			tree_int(20)};
		tst::check_eq(children.size(), size_t(2), SL);
		tree_int t(13, std::move(children));

		tst::check_eq(t.value, 13, SL);
		tst::check_eq(
			t.children.size(),
			size_t(2),
			[&](auto& o) {
				o << "t.children.size() = " << t.children.size();
			},
			SL
		);
		tst::check_eq(
			t.children[0].value,
			10,
			[&](auto& o) {
				o << "t.children[0].value = " << t.children[0].value;
			},
			SL
		);
		tst::check_eq(
			t.children[1].value,
			20,
			[&](auto& o) {
				o << "t.children[1].value = " << t.children[1].value;
			},
			SL
		);
	});

	suite.add("constructor_from_container_rvalue_rvalue", []() {
		typedef utki::tree<std::string> tree_str;
		tree_str::container_type children = { //
			tree_str("10"),
			tree_str("20")};
		tst::check_eq(children.size(), size_t(2), SL);
		std::string str("13");
		tree_str t(std::move(str), std::move(children));
		tst::check_eq(
			str.size(), // NOLINT(clang-analyzer-cplusplus.Move): intentional check that the string was moved
			size_t(0),
			SL
		);

		tst::check_eq(t.value, "13"s, SL);
		tst::check_eq(
			t.children.size(),
			size_t(2),
			[&](auto& o) {
				o << "t.children.size() = " << t.children.size();
			},
			SL
		);
		tst::check_eq(
			t.children[0].value,
			"10"s,
			[&](auto& o) {
				o << "t.children[0].value = " << t.children[0].value;
			},
			SL
		);
		tst::check_eq(
			t.children[1].value,
			"20"s,
			[&](auto& o) {
				o << "t.children[1].value = " << t.children[1].value;
			},
			SL
		);
	});

	suite.add("traversal_non_const_container", []() {
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		std::vector<int> encountered;

		auto traversal = utki::make_traversal(roots);

		tst::check_eq(
			roots.begin()->value,
			1,
			[&](auto& o) {
				o << "roots.begin()->value = " << roots.begin()->value;
			},
			SL
		);

		tst::check_eq(
			traversal.begin()->value,
			1,
			[&](auto& o) {
				o << "traversal.begin()->value = " << traversal.begin()->value;
			},
			SL
		);

		for (auto i = traversal.begin(); i != traversal.end(); ++i) {
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {1, 34, 45, 2, 3, 78, 89, 96, 4, 32, 64, 128, 42, 98, 99, 100};

		if (encountered != expected) {
			utki::log([&](auto& o) {
				o << "encountered = ";
			});
			for (auto& i : encountered) {
				utki::log([&](auto& o) {
					o << i << ", ";
				});
			}
			utki::log([&](auto& o) {
				o << std::endl;
			});
			tst::check(
				false,
				[&](auto& o) {
					o << "traversed tree is not as expected";
				},
				SL
			);
		}
	});

	suite.add("traversal_const_container", []() {
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		std::vector<int> encountered;

		for (auto& i : utki::make_traversal(roots)) {
			encountered.push_back(i.value);
		}

		std::vector<int> expected{1, 34, 45, 2, 3, 78, 89, 96, 4, 32, 64, 128, 42, 98, 99, 100};

		if (encountered != expected) {
			utki::log([&](auto& o) {
				o << "encountered = ";
			});
			for (auto& i : encountered) {
				utki::log([&](auto& o) {
					o << i << ", ";
				});
			}
			utki::log([&](auto& o) {
				o << std::endl;
			});
			tst::check(false, SL);
		}
	});

	suite.add("traversal_non_const_container_const_iterator", []() {
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		std::vector<int> encountered;

		auto traversal = utki::make_traversal(roots);

		tst::check_eq(
			roots.begin()->value,
			1,
			[&](auto& o) {
				o << "roots.begin()->value = " << roots.begin()->value;
			},
			SL
		);
		tst::check_eq(
			traversal.begin()->value,
			1,
			[&](auto& o) {
				o << "traversal.begin()->value = " << traversal.begin()->value;
			},
			SL
		);

		for (auto i = traversal.cbegin(); i != traversal.cend(); ++i) {
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {1, 34, 45, 2, 3, 78, 89, 96, 4, 32, 64, 128, 42, 98, 99, 100};

		if (encountered != expected) {
			utki::log([&](auto& o) {
				o << "encountered = ";
			});
			for (auto& i : encountered) {
				utki::log([&](auto& o) {
					o << i << ", ";
				});
			}
			utki::log([&](auto& o) {
				o << std::endl;
			});
			tst::check(
				false,
				[&](auto& o) {
					o << "traversed tree is not as expected";
				},
				SL
			);
		}
	});

	suite.add("traversal_const_container_const_iterator", []() {
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		std::vector<int> encountered;

		auto traversal = utki::make_traversal(roots);

		tst::check_eq(
			roots.begin()->value,
			1,
			[&](auto& o) {
				o << "roots.begin()->value = " << roots.begin()->value;
			},
			SL
		);
		tst::check_eq(
			traversal.begin()->value,
			1,
			[&](auto& o) {
				o << "traversal.begin()->value = " << traversal.begin()->value;
			},
			SL
		);

		for (auto i = traversal.cbegin(); i != traversal.cend(); ++i) {
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {1, 34, 45, 2, 3, 78, 89, 96, 4, 32, 64, 128, 42, 98, 99, 100};

		if (encountered != expected) {
			utki::log([&](auto& o) {
				o << "encountered = ";
			});
			for (auto& i : encountered) {
				utki::log([&](auto& o) {
					o << i << ", ";
				});
			}
			utki::log([&](auto& o) {
				o << std::endl;
			});
			tst::check(
				false,
				[&](auto& o) {
					o << "traversed tree is not as expected";
				},
				SL
			);
		}
	});

	suite.add("traversal_non_const_container_reverse_iterator", []() {
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		std::vector<int> encountered;

		auto traversal = utki::make_traversal(roots);

		tst::check_eq(
			roots.begin()->value,
			1,
			[&](auto& o) {
				o << "roots.begin()->value = " << roots.begin()->value;
			},
			SL
		);
		tst::check_eq(
			traversal.begin()->value,
			1,
			[&](auto& o) {
				o << "traversal.begin()->value = " << traversal.begin()->value;
			},
			SL
		);

		for (auto i = traversal.rbegin(); i != traversal.rend(); ++i) {
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {100, 99, 98, 42, 128, 64, 32, 4, 96, 89, 78, 3, 2, 45, 34, 1};

		if (encountered != expected) {
			utki::log([&](auto& o) {
				o << "encountered = ";
			});
			for (auto& i : encountered) {
				utki::log([&](auto& o) {
					o << i << ", ";
				});
			}
			utki::log([&](auto& o) {
				o << std::endl;
			});
			tst::check(
				false,
				[&](auto& o) {
					o << "traversed tree is not as expected";
				},
				SL
			);
		}
	});

	suite.add("traversal_const_container_reverse_iterator", []() {
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		std::vector<int> encountered;

		auto traversal = utki::make_traversal(roots);

		tst::check_eq(
			roots.begin()->value,
			1,
			[&](auto& o) {
				o << "roots.begin()->value = " << roots.begin()->value;
			},
			SL
		);
		tst::check_eq(
			traversal.begin()->value,
			1,
			[&](auto& o) {
				o << "traversal.begin()->value = " << traversal.begin()->value;
			},
			SL
		);

		for (auto i = traversal.rbegin(); i != traversal.rend(); ++i) {
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {100, 99, 98, 42, 128, 64, 32, 4, 96, 89, 78, 3, 2, 45, 34, 1};

		if (encountered != expected) {
			utki::log([&](auto& o) {
				o << "encountered = ";
			});
			for (auto& i : encountered) {
				utki::log([&](auto& o) {
					o << i << ", ";
				});
			}
			utki::log([&](auto& o) {
				o << std::endl;
			});
			tst::check(
				false,
				[&](auto& o) {
					o << "traversed tree is not as expected";
				},
				SL
			);
		}
	});

	suite.add("traversal_non_const_container_const_reverse_iterator", []() {
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		std::vector<int> encountered;

		auto traversal = utki::make_traversal(roots);

		tst::check_eq(
			roots.begin()->value,
			1,
			[&](auto& o) {
				o << "roots.begin()->value = " << roots.begin()->value;
			},
			SL
		);
		tst::check_eq(
			traversal.begin()->value,
			1,
			[&](auto& o) {
				o << "traversal.begin()->value = " << traversal.begin()->value;
			},
			SL
		);

		for (auto i = traversal.crbegin(); i != traversal.crend(); ++i) {
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {100, 99, 98, 42, 128, 64, 32, 4, 96, 89, 78, 3, 2, 45, 34, 1};

		if (encountered != expected) {
			utki::log([&](auto& o) {
				o << "encountered = ";
			});
			for (auto& i : encountered) {
				utki::log([&](auto& o) {
					o << i << ", ";
				});
			}
			utki::log([&](auto& o) {
				o << std::endl;
			});
			tst::check(
				false,
				[&](auto& o) {
					o << "traversed tree is not as expected";
				},
				SL
			);
		}
	});

	suite.add("traversal_const_container_const_reverse_iterator", []() {
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		std::vector<int> encountered;

		auto traversal = utki::make_traversal(roots);

		tst::check_eq(
			roots.begin()->value,
			1,
			[&](auto& o) {
				o << "roots.begin()->value = " << roots.begin()->value;
			},
			SL
		);
		tst::check_eq(
			traversal.begin()->value,
			1,
			[&](auto& o) {
				o << "traversal.begin()->value = " << traversal.begin()->value;
			},
			SL
		);

		for (auto i = traversal.crbegin(); i != traversal.crend(); ++i) {
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {100, 99, 98, 42, 128, 64, 32, 4, 96, 89, 78, 3, 2, 45, 34, 1};

		if (encountered != expected) {
			utki::log([&](auto& o) {
				o << "encountered = ";
			});
			for (auto& i : encountered) {
				utki::log([&](auto& o) {
					o << i << ", ";
				});
			}
			utki::log([&](auto& o) {
				o << std::endl;
			});
			tst::check(
				false,
				[&](auto& o) {
					o << "traversed tree is not as expected";
				},
				SL
			);
		}
	});

	suite.add("traversal_non_const_container_index", []() {
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		std::vector<int> encountered;

		auto traversal = utki::make_traversal(roots);

		tst::check_eq(
			roots.begin()->value,
			1,
			[&](auto& o) {
				o << "roots.begin()->value = " << roots.begin()->value;
			},
			SL
		);
		tst::check_eq(
			traversal.begin()->value,
			1,
			[&](auto& o) {
				o << "traversal.begin()->value = " << traversal.begin()->value;
			},
			SL
		);

		auto i = traversal.begin();

		++i;
		++i;
		++i;
		++i;
		++i;
		++i;

		auto index = i.index();

		std::vector<size_t> expected = {1, 0, 1};

		if (index != expected) {
			utki::log([&](auto& o) {
				o << "index = ";
			});
			for (auto& i : index) {
				utki::log([&](auto& o) {
					o << i << ", ";
				});
			}
			utki::log([&](auto& o) {
				o << std::endl;
			});
			tst::check(
				false,
				[&](auto& o) {
					o << "index is not as expected";
				},
				SL
			);
		}

		auto iter = traversal.make_iterator(index);

		tst::check(i == iter, SL);

		auto index_2 = iter.index();

		if (index_2 != expected) {
			utki::log([&](auto& o) {
				o << "index_2 = ";
			});
			for (auto& i : index_2) {
				utki::log([&](auto& o) {
					o << i << ", ";
				});
			}
			utki::log([&](auto& o) {
				o << std::endl;
			});
			tst::check(
				false,
				[&](auto& o) {
					o << "index_2 is not as expected";
				},
				SL
			);
		}
	});

	suite.add("traversal_const_container_index", []() {
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		std::vector<int> encountered;

		const auto traversal = utki::make_traversal(roots);

		tst::check_eq(
			roots.begin()->value,
			1,
			[&](auto& o) {
				o << "roots.begin()->value = " << roots.begin()->value;
			},
			SL
		);
		tst::check_eq(
			traversal.cbegin()->value,
			1,
			[&](auto& o) {
				o << "traversal.begin()->value = " << traversal.cbegin()->value;
			},
			SL
		);

		auto i = traversal.cbegin();

		++i;
		++i;
		++i;
		++i;
		++i;
		++i;

		auto index = i.index();

		std::vector<size_t> expected = {1, 0, 1};

		if (index != expected) {
			utki::log([&](auto& o) {
				o << "index = ";
			});
			for (auto& i : index) {
				utki::log([&](auto& o) {
					o << i << ", ";
				});
			}
			utki::log([&](auto& o) {
				o << std::endl;
			});
			tst::check(
				false,
				[&](auto& o) {
					o << "index is not as expected";
				},
				SL
			);
		}

		auto iter = traversal.make_const_iterator(index);

		tst::check(i == iter, SL);

		auto index_2 = iter.index();

		if (index_2 != expected) {
			utki::log([&](auto& o) {
				o << "index_2 = ";
			});
			for (auto& i : index_2) {
				utki::log([&](auto& o) {
					o << i << ", ";
				});
			}
			utki::log([&](auto& o) {
				o << std::endl;
			});
			tst::check(
				false,
				[&](auto& o) {
					o << "index_2 is not as expected";
				},
				SL
			);
		}
	});

	suite.add("traversal_check_index_is_valid", []() {
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		std::vector<int> encountered;

		const auto traversal = utki::make_traversal(roots);

		tst::check(!traversal.is_valid(utki::make_span<size_t>(nullptr)), SL);
		tst::check(traversal.is_valid({0}), SL);
		tst::check(traversal.is_valid({0, 0}), SL);
		tst::check(traversal.is_valid({0, 1}), SL);
		tst::check(!traversal.is_valid({0, 1, 0}), SL);
		tst::check(!traversal.is_valid({0, 2}), SL);
		tst::check(traversal.is_valid({1}), SL);
		tst::check(traversal.is_valid({1, 0}), SL);
		tst::check(traversal.is_valid({1, 0, 0}), SL);
		tst::check(traversal.is_valid({1, 1, 0}), SL);
		tst::check(!traversal.is_valid({1, 1, 0, 2}), SL);
		tst::check(!traversal.is_valid({3, 1}), SL);
	});

	suite.add("traversal_operator_square_brackets", []() {
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		std::vector<int> encountered;

		const auto traversal = utki::make_traversal(roots);

		tst::check_eq(traversal[{0}].value, 1, SL);
		tst::check_eq(traversal[{0, 0}].value, 34, SL);
		tst::check_eq(traversal[{0, 1}].value, 45, SL);
		tst::check_eq(traversal[{1}].value, 2, SL);
		tst::check_eq(traversal[{1, 0}].value, 3, SL);
		tst::check_eq(traversal[{1, 0, 0}].value, 78, SL);
		tst::check_eq(traversal[{1, 1, 0}].value, 32, SL);
		tst::check_eq(traversal[{1, 1, 2}].value, 128, SL);
	});

	suite.add("traversal_iterator_comparison", []() {
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		const auto traversal = utki::make_traversal(roots);

		auto iter1 = traversal.make_const_iterator({0, 1});
		auto iter2 = traversal.make_const_iterator({1, 1, 2});
		auto iter3 = traversal.make_const_iterator({1, 1, 2});

		tst::check(iter1 < iter2, SL);
		tst::check(iter2 > iter1, SL);
		tst::check(iter1 <= iter2, SL);
		tst::check(iter2 >= iter1, SL);
		tst::check(iter1 != iter3, SL);
		tst::check(iter2 == iter3, SL);
		tst::check(iter2 >= iter3, SL);
		tst::check(iter2 <= iter3, SL);
	});

	suite.add("traversal_insertion", []() {
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		auto traversal = utki::make_traversal(roots);

		auto iter = traversal.make_iterator({1, 1});

		tst::check(!iter.is_last_child(), SL);

		auto new_iter = traversal.insert(iter, tree(45));

		tst::check_eq(new_iter->value, 45, SL);
		tst::check(new_iter->children.empty(), SL);
		tst::check_eq(traversal.make_iterator({1, 1})->value, 45, SL);
		tst::check_eq(std::next(traversal.make_iterator({1, 1}))->value, 4, SL);
	});

	suite.add("traversal_insert_after", []() {
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		auto traversal = utki::make_traversal(roots);

		auto iter = traversal.make_iterator({1, 2});

		tst::check(iter.is_last_child(), SL);

		tree t(45, {46, 47});

		auto new_iter = traversal.insert_after(iter, t);

		tst::check(new_iter.is_last_child(), SL);
		tst::check_eq(new_iter->value, 45, SL);

		++new_iter;

		tst::check_eq(new_iter->value, 46, SL);
	});

	suite.add("traversal_erase_non_last", []() {
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		auto traversal = utki::make_traversal(roots);

		auto iter = traversal.make_iterator({1, 1});
		tst::check(!iter.is_last_child(), SL);

		iter = traversal.erase(iter);

		tst::check_eq(iter->value, 42, SL);
		tst::check(iter.is_last_child(), SL);
	});

	suite.add("traversal_erase_last_child", []() {
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		auto traversal = utki::make_traversal(roots);

		auto iter = traversal.make_iterator({1, 0, 2});
		tst::check(iter.is_last_child(), SL);

		iter = traversal.erase(iter);

		tst::check(!iter.is_last_child(), SL);
		tst::check_eq(iter->value, 4, SL);
	});

	suite.add("traversal_erase_very_last", []() {
		using tree = utki::tree<int>;
		tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		auto traversal = utki::make_traversal(roots);

		auto iter = traversal.make_iterator({1, 2, 2});
		tst::check(iter.is_last_child(), SL);

		iter = traversal.erase(iter);

		tst::check(!iter.is_last_child(), SL);
		tst::check(iter == traversal.end(), SL);
	});

	struct traversal_iterator_depth_fixture {
		using tree = utki::tree<int>;
		tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		decltype(utki::make_traversal(roots)) traversal = utki::make_traversal(roots);
	};

	suite.add<std::pair<std::vector<size_t>, size_t>>(
		"traversal_iterator_depth",
		{
			{       {}, size_t(0)},
			{{1, 2, 2}, size_t(3)},
			{   {1, 2}, size_t(2)}
    },
		[](const auto& p) {
			traversal_iterator_depth_fixture f;

			auto iter = f.traversal.make_iterator(p.first);
			tst::check_eq(iter.depth(), p.second, SL);
		}
	);

	suite.add("traversal_iterator_at_level", []() {
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1, {34, 45}),
			tree( //
				2,
				{//
				 tree(3, {78, 89, 96}),
				 tree(4, {32, 64, 128}),
				 tree(42, {98, 99, 100})}
			)};

		auto traversal = utki::make_traversal(roots);

		{
			auto iter = traversal.make_iterator({1, 2, 2});
			tst::check_eq(iter.depth(), size_t(3), SL);
			tst::check_eq(iter.at_level(0).value, 2, SL);
			tst::check_eq(iter.at_level(1).value, 42, SL);
			tst::check_eq(iter.at_level(2).value, 100, SL);
		}

		{
			auto iter = traversal.make_iterator({0, 1});
			tst::check_eq(iter.depth(), size_t(2), SL);
			tst::check(iter.at_level(0) == 1, SL);
			tst::check(iter.at_level(1) == 45, SL);
		}
	});
});
} // namespace
