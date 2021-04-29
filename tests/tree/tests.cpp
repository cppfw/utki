#include "../../src/utki/tree.hpp"
#include "../../src/utki/debug.hpp"

#include "tests.hpp"


namespace{
void basic(){
	{
		utki::tree<int> t;

		t.children.push_back(35);

		utki::assert(t.children.size() == 1, SL);
	}

	{
		utki::tree<int> t(1,{34, 45});
		utki::assert(t.value == 1, [&](auto&o){o << "t.value = " << t.value;}, SL);
	}

	{
		const utki::tree<int> t{
			{34, 45},

			{
				{78, 89, 96},
				{32, 64, 128},
				decltype(t)(42, {98, 99, 100})
			}
		};

		utki::assert(t.children.size() == 2, SL);

		utki::assert(t.children[0].children.size() == 2, SL);
		utki::assert(t.children[0].children[0].value == 34, SL);
		utki::assert(t.children[0].children[1].value == 45, SL);

		utki::assert(t.children[1].children.size() == 3, SL);
		utki::assert(t.children[1].children[0].children.size() == 3, SL);
		utki::assert(t.children[1].children[1].children.size() == 3, SL);

		utki::assert(t.children[1].children[2].children.size() == 3, SL);
		utki::assert(t.children[1].children[2].value == 42, SL);

		utki::assert(t.children[1].children[2].children[0].value == 98, SL);
		utki::assert(t.children[1].children[2].children[1].value == 99, SL);
		utki::assert(t.children[1].children[2].children[2].value == 100, SL);
	}
}

void test_move_assignment(){
	utki::tree<int> t{
		{34, 45},

		{
			{78, 89, 96},
			{32, 64, 128},
			decltype(t)(42, {98, 99, 100})
		}
	};

	t.children[0].children = std::move(t.children[1].children);

	utki::assert(t.children.size() == 2, SL);

	utki::assert(t.children[0].children.size() == 3, SL);
	utki::assert(t.children[0].children[0].children.size() == 3, SL);
	utki::assert(t.children[0].children[1].children.size() == 3, SL);

	utki::assert(t.children[0].children[2].children.size() == 3, SL);
	utki::assert(t.children[0].children[2].value == 42, SL);

	utki::assert(t.children[0].children[2].children[0].value == 98, SL);
	utki::assert(t.children[0].children[2].children[1].value == 99, SL);
	utki::assert(t.children[0].children[2].children[2].value == 100, SL);

	utki::assert(t.children[1].children.size() == 0, SL);
}

void test_assignment(){
	utki::tree<int> t{
		{34, 45},

		{
			{78, 89, 96},
			{32, 64, 128},
			decltype(t)(42, {98, 99, 100})
		}
	};

	t.children[0].children = t.children[1].children;

	utki::assert(t.children.size() == 2, SL);

	utki::assert(t.children[0].children.size() == 3, SL);
	utki::assert(t.children[0].children[0].children.size() == 3, SL);
	utki::assert(t.children[0].children[1].children.size() == 3, SL);

	utki::assert(t.children[0].children[2].children.size() == 3, SL);
	utki::assert(t.children[0].children[2].value == 42, SL);

	utki::assert(t.children[0].children[2].children[0].value == 98, SL);
	utki::assert(t.children[0].children[2].children[1].value == 99, SL);
	utki::assert(t.children[0].children[2].children[2].value == 100, SL);

	utki::assert(t.children[1].children.size() == 3, SL);
}

void test_comparison(){
	// test tree to value comparison
	{
		utki::tree<int> t{
			{34, 45},

			{
				{78, 89, 96},
				{32, 64, 128},
				decltype(t)(42, {98, 99, 100})
			}
		};


		utki::assert(t.children.size() == 2, SL);
		utki::assert(t.children[0].children.size() == 2, SL);
		utki::assert(t.children[0].children[0].value == 34, [&](auto&o){o << "value = " << t.children[0].value;}, SL);

		utki::assert(t.children[1].children.size() == 3, SL);
		utki::assert(t.children[1].children[0].children.size() == 3, SL);
		utki::assert(t.children[1].children[1].children.size() == 3, SL);

		utki::assert(t.children[1].children[2].children.size() == 3, SL);
		utki::assert(t.children[1].children[2] == 42, SL);

		utki::assert(t.children[1].children[2].children[0] == 98, SL);
		utki::assert(t.children[1].children[2].children[1] == 99, SL);
		utki::assert(t.children[1].children[2].children[2] == 100, SL);

		utki::assert(t.children[1].children.size() == 3, SL);
	}

	// test tree to tree comparison
	{
		utki::tree<int> t1(10, {
			decltype(t1)(13, {34, 45}),

			decltype(t1)(11, {
				decltype(t1)(14, {78, 89, 96}),
				decltype(t1)(15, {32, 64, 128}),
				decltype(t1)(42, {98, 99, 100})
			})
		});

		utki::tree<int> t2(10, {
			decltype(t2)(13, {34, 45}),

			decltype(t2)(11, {
				decltype(t2)(14, {78, 89, 96}),
				decltype(t2)(15, {32, 64, 128}),
				decltype(t2)(42, {98, 99, 100})
			})
		});

		utki::assert(t1.children[0].value == t2.children[0].value, SL);
		utki::assert(t1.children[0].children[0].value == t2.children[0].children[0].value, SL);
		utki::assert(t1.children[0].children[1].value == t2.children[0].children[1].value, SL);
		utki::assert(t1 == t2, SL);
		utki::assert(t1 == t1, SL);
	}
}

void test_constructors_from_container(){
	// lvalue, lvalue
	{
		typedef utki::tree<int> tree_int;
		tree_int::container_type children = {
			tree_int(10),
			tree_int(20)
		};
		utki::assert(children.size() == 2, SL);
		tree_int t(13, children);

		utki::assert(t.value == 13, SL);
		utki::assert(t.children.size() == 2, [&](auto&o){o << "t.children.size() = " << t.children.size();}, SL);
		utki::assert(t.children[0].value == 10, [&](auto&o){o << "t.children[0].value = " << t.children[0].value;}, SL);
		utki::assert(t.children[1].value == 20, [&](auto&o){o << "t.children[1].value = " << t.children[1].value;}, SL);
	}

	// lvalue, rvalue
	{
		typedef utki::tree<int> tree_int;
		tree_int::container_type children = {
			tree_int(10),
			tree_int(20)
		};
		utki::assert(children.size() == 2, SL);
		tree_int t(13, std::move(children));

		utki::assert(t.value == 13, SL);
		utki::assert(t.children.size() == 2, [&](auto&o){o << "t.children.size() = " << t.children.size();}, SL);
		utki::assert(t.children[0].value == 10, [&](auto&o){o << "t.children[0].value = " << t.children[0].value;}, SL);
		utki::assert(t.children[1].value == 20, [&](auto&o){o << "t.children[1].value = " << t.children[1].value;}, SL);
	}

	// rvalue, rvalue
	{
		typedef utki::tree<std::string> tree_str;
		tree_str::container_type children = {
			tree_str("10"),
			tree_str("20")
		};
		utki::assert(children.size() == 2, SL);
		std::string str("13");
		tree_str t(std::move(str), std::move(children));
		utki::assert(str.length() == 0, SL);

		utki::assert(t.value == "13", SL);
		utki::assert(t.children.size() == 2, [&](auto&o){o << "t.children.size() = " << t.children.size();}, SL);
		utki::assert(t.children[0].value == "10", [&](auto&o){o << "t.children[0].value = " << t.children[0].value;}, SL);
		utki::assert(t.children[1].value == "20", [&](auto&o){o << "t.children[1].value = " << t.children[1].value;}, SL);
	}
}

void test_traversal(){
	// non-const container
	{
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		auto traversal = utki::make_traversal(roots);

		utki::assert(roots.begin()->value == 1, [&](auto&o){o << "roots.begin()->value = " << roots.begin()->value;}, SL);

		utki::assert(traversal.begin()->value == 1, [&](auto&o){o << "traversal.begin()->value = " << traversal.begin()->value;}, SL);

		for(auto i = traversal.begin(); i != traversal.end(); ++i){
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {
			1, 34, 45, 2, 3, 78, 89, 96, 4, 32, 64, 128, 42, 98, 99, 100
		};

		if(encountered != expected){
			utki::log([&](auto&o){o << "encountered = ";});
			for(auto& i : encountered){
				utki::log([&](auto&o){o << i << ", ";});
			}
			utki::log([&](auto&o){o << std::endl;});
			utki::assert(false, [&](auto&o){o << "traversed tree is not as expected";}, SL);
		}
	}

	// const container
	{
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		for(auto& i : utki::make_traversal(roots)){
			encountered.push_back(i.value);
		}

		std::vector<int> expected{
			1, 34, 45, 2, 3, 78, 89, 96, 4, 32, 64, 128, 42, 98, 99, 100
		};

		if(encountered != expected){
			utki::log([&](auto&o){o << "encountered = ";});
			for(auto& i : encountered){
				utki::log([&](auto&o){o << i << ", ";});
			}
			utki::log([&](auto&o){o << std::endl;});
			utki::assert(false, SL);
		}
	}

	// non-const container, const_iterator
	{
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		auto traversal = utki::make_traversal(roots);

		utki::assert(roots.begin()->value == 1, [&](auto&o){o << "roots.begin()->value = " << roots.begin()->value;}, SL);
		utki::assert(traversal.begin()->value == 1, [&](auto&o){o << "traversal.begin()->value = " << traversal.begin()->value;}, SL);

		for(auto i = traversal.cbegin(); i != traversal.cend(); ++i){
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {
			1, 34, 45, 2, 3, 78, 89, 96, 4, 32, 64, 128, 42, 98, 99, 100
		};

		if(encountered != expected){
			utki::log([&](auto&o){o << "encountered = ";});
			for(auto& i : encountered){
				utki::log([&](auto&o){o << i << ", ";});
			}
			utki::log([&](auto&o){o << std::endl;});
			utki::assert(false, [&](auto&o){o << "traversed tree is not as expected";}, SL);
		}
	}

	// const container, const_iterator
	{
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		auto traversal = utki::make_traversal(roots);

		utki::assert(roots.begin()->value == 1, [&](auto&o){o << "roots.begin()->value = " << roots.begin()->value;}, SL);
		utki::assert(traversal.begin()->value == 1, [&](auto&o){o << "traversal.begin()->value = " << traversal.begin()->value;}, SL);

		for(auto i = traversal.cbegin(); i != traversal.cend(); ++i){
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {
			1, 34, 45, 2, 3, 78, 89, 96, 4, 32, 64, 128, 42, 98, 99, 100
		};

		if(encountered != expected){
			utki::log([&](auto&o){o << "encountered = ";});
			for(auto& i : encountered){
				utki::log([&](auto&o){o << i << ", ";});
			}
			utki::log([&](auto&o){o << std::endl;});
			utki::assert(false, [&](auto&o){o << "traversed tree is not as expected";}, SL);
		}
	}

	// non-const container, reverse_iterator
	{
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		auto traversal = utki::make_traversal(roots);

		utki::assert(roots.begin()->value == 1, [&](auto&o){o << "roots.begin()->value = " << roots.begin()->value;}, SL);
		utki::assert(traversal.begin()->value == 1, [&](auto&o){o << "traversal.begin()->value = " << traversal.begin()->value;}, SL);

		for(auto i = traversal.rbegin(); i != traversal.rend(); ++i){
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {
			100, 99, 98, 42, 128, 64, 32, 4, 96, 89, 78, 3, 2, 45, 34, 1
		};

		if(encountered != expected){
			utki::log([&](auto&o){o << "encountered = ";});
			for(auto& i : encountered){
				utki::log([&](auto&o){o << i << ", ";});
			}
			utki::log([&](auto&o){o << std::endl;});
			utki::assert(false, [&](auto&o){o << "traversed tree is not as expected";}, SL);
		}
	}

	// const container, reverse_iterator
	{
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		auto traversal = utki::make_traversal(roots);

		utki::assert(roots.begin()->value == 1, [&](auto&o){o << "roots.begin()->value = " << roots.begin()->value;}, SL);
		utki::assert(traversal.begin()->value == 1, [&](auto&o){o << "traversal.begin()->value = " << traversal.begin()->value;}, SL);

		for(auto i = traversal.rbegin(); i != traversal.rend(); ++i){
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {
			100, 99, 98, 42, 128, 64, 32, 4, 96, 89, 78, 3, 2, 45, 34, 1
		};

		if(encountered != expected){
			utki::log([&](auto&o){o << "encountered = ";});
			for(auto& i : encountered){
				utki::log([&](auto&o){o << i << ", ";});
			}
			utki::log([&](auto&o){o << std::endl;});
			utki::assert(false, [&](auto&o){o << "traversed tree is not as expected";}, SL);
		}
	}

	// non-const container, const_reverse_iterator
	{
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		auto traversal = utki::make_traversal(roots);

		utki::assert(roots.begin()->value == 1, [&](auto&o){o << "roots.begin()->value = " << roots.begin()->value;}, SL);
		utki::assert(traversal.begin()->value == 1, [&](auto&o){o << "traversal.begin()->value = " << traversal.begin()->value;}, SL);

		for(auto i = traversal.crbegin(); i != traversal.crend(); ++i){
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {
			100, 99, 98, 42, 128, 64, 32, 4, 96, 89, 78, 3, 2, 45, 34, 1
		};

		if(encountered != expected){
			utki::log([&](auto&o){o << "encountered = ";});
			for(auto& i : encountered){
				utki::log([&](auto&o){o << i << ", ";});
			}
			utki::log([&](auto&o){o << std::endl;});
			utki::assert(false, [&](auto&o){o << "traversed tree is not as expected";}, SL);
		}
	}

	// const container, const_reverse_iterator
	{
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		auto traversal = utki::make_traversal(roots);

		utki::assert(roots.begin()->value == 1, [&](auto&o){o << "roots.begin()->value = " << roots.begin()->value;}, SL);
		utki::assert(traversal.begin()->value == 1, [&](auto&o){o << "traversal.begin()->value = " << traversal.begin()->value;}, SL);

		for(auto i = traversal.crbegin(); i != traversal.crend(); ++i){
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {
			100, 99, 98, 42, 128, 64, 32, 4, 96, 89, 78, 3, 2, 45, 34, 1
		};

		if(encountered != expected){
			utki::log([&](auto&o){o << "encountered = ";});
			for(auto& i : encountered){
				utki::log([&](auto&o){o << i << ", ";});
			}
			utki::log([&](auto&o){o << std::endl;});
			utki::assert(false, [&](auto&o){o << "traversed tree is not as expected";}, SL);
		}
	}

	// non-const container, index
	{
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		auto traversal = utki::make_traversal(roots);

		utki::assert(roots.begin()->value == 1, [&](auto&o){o << "roots.begin()->value = " << roots.begin()->value;}, SL);
		utki::assert(traversal.begin()->value == 1, [&](auto&o){o << "traversal.begin()->value = " << traversal.begin()->value;}, SL);

		auto i = traversal.begin();

		++i;
		++i;
		++i;
		++i;
		++i;
		++i;

		auto index = i.index();

		std::vector<size_t> expected = {
			1, 0, 1
		};

		if(index != expected){
			utki::log([&](auto&o){o << "index = ";});
			for(auto& i : index){
				utki::log([&](auto&o){o << i << ", ";});
			}
			utki::log([&](auto&o){o << std::endl;});
			utki::assert(false, [&](auto&o){o << "index is not as expected";}, SL);
		}

		auto iter = traversal.make_iterator(index);

		utki::assert(i == iter, SL);

		auto index_2 = iter.index();

		if(index_2 != expected){
			utki::log([&](auto&o){o << "index_2 = ";});
			for(auto& i : index_2){
				utki::log([&](auto&o){o << i << ", ";});
			}
			utki::log([&](auto&o){o << std::endl;});
			utki::assert(false, [&](auto&o){o << "index_2 is not as expected";}, SL);
		}
	}

	// const container, index
	{
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		const auto traversal = utki::make_traversal(roots);

		utki::assert(roots.begin()->value == 1, [&](auto&o){o << "roots.begin()->value = " << roots.begin()->value;}, SL);
		utki::assert(traversal.cbegin()->value == 1, [&](auto&o){o << "traversal.begin()->value = " << traversal.cbegin()->value;}, SL);

		auto i = traversal.cbegin();

		++i;
		++i;
		++i;
		++i;
		++i;
		++i;

		auto index = i.index();

		std::vector<size_t> expected = {
			1, 0, 1
		};

		if(index != expected){
			utki::log([&](auto&o){o << "index = ";});
			for(auto& i : index){
				utki::log([&](auto&o){o << i << ", ";});
			}
			utki::log([&](auto&o){o << std::endl;});
			utki::assert(false, [&](auto&o){o << "index is not as expected";}, SL);
		}

		auto iter = traversal.make_const_iterator(index);

		utki::assert(i == iter, SL);

		auto index_2 = iter.index();

		if(index_2 != expected){
			utki::log([&](auto&o){o << "index_2 = ";});
			for(auto& i : index_2){
				utki::log([&](auto&o){o << i << ", ";});
			}
			utki::log([&](auto&o){o << std::endl;});
			utki::assert(false, [&](auto&o){o << "index_2 is not as expected";}, SL);
		}
	}

	// check if index is valid
	{
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		const auto traversal = utki::make_traversal(roots);

		utki::assert(!traversal.is_valid(utki::make_span<size_t>(nullptr)), SL);
		utki::assert(traversal.is_valid({0}), SL);
		utki::assert(traversal.is_valid({0, 0}), SL);
		utki::assert(traversal.is_valid({0, 1}), SL);
		utki::assert(!traversal.is_valid({0, 1, 0}), SL);
		utki::assert(!traversal.is_valid({0, 2}), SL);
		utki::assert(traversal.is_valid({1}), SL);
		utki::assert(traversal.is_valid({1, 0}), SL);
		utki::assert(traversal.is_valid({1, 0, 0}), SL);
		utki::assert(traversal.is_valid({1, 1, 0}), SL);
		utki::assert(!traversal.is_valid({1, 1, 0, 2}), SL);
		utki::assert(!traversal.is_valid({3, 1}), SL);
	}

	// test operator[]
	{
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		const auto traversal = utki::make_traversal(roots);

		utki::assert(traversal[{0}].value == 1, SL);
		utki::assert((traversal[{0, 0}].value) == 34, SL);
		utki::assert((traversal[{0, 1}].value) == 45, SL);
		utki::assert((traversal[{1}].value) == 2, SL);
		utki::assert((traversal[{1, 0}].value) == 3, SL);
		utki::assert((traversal[{1, 0, 0}].value) == 78, SL);
		utki::assert((traversal[{1, 1, 0}].value) == 32, SL);
		utki::assert((traversal[{1, 1, 2}].value) == 128, SL);
	}

	// iterator comparison
	{
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		const auto traversal = utki::make_traversal(roots);

		auto iter1 = traversal.make_const_iterator({0, 1});
		auto iter2 = traversal.make_const_iterator({1, 1, 2});
		auto iter3 = traversal.make_const_iterator({1, 1, 2});

		utki::assert(iter1 < iter2, SL);
		utki::assert(iter2 > iter1, SL);
		utki::assert(iter1 <= iter2, SL);
		utki::assert(iter2 >= iter1, SL);
		utki::assert(iter1 != iter3, SL);
		utki::assert(iter2 == iter3, SL);
		utki::assert(iter2 >= iter3, SL);
		utki::assert(iter2 <= iter3, SL);
	}

	// insertion
	{
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		auto traversal = utki::make_traversal(roots);

		auto iter = traversal.make_iterator({1, 1});

		utki::assert(!iter.is_last_child(), SL);

		auto new_iter = traversal.insert(iter, tree(45));

		utki::assert(new_iter->value == 45, SL);
		utki::assert(new_iter->children.empty(), SL);
		utki::assert(traversal.make_iterator({1, 1})->value == 45, SL);
		utki::assert(std::next(traversal.make_iterator({1, 1}))->value == 4, SL);
	}

	// insert after
	{
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		auto traversal = utki::make_traversal(roots);

		auto iter = traversal.make_iterator({1, 2});

		utki::assert(iter.is_last_child(), SL);

		tree t(45, {46, 47});

		auto new_iter = traversal.insert_after(iter, t);

		utki::assert(new_iter.is_last_child(), SL);
		utki::assert(new_iter->value == 45, SL);

		++new_iter;

		utki::assert(new_iter->value == 46, SL);
	}

	// erase non-last
	{
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		auto traversal = utki::make_traversal(roots);

		auto iter = traversal.make_iterator({1, 1});
		utki::assert(!iter.is_last_child(), SL);

		iter = traversal.erase(iter);

		utki::assert(iter->value == 42, SL);
		utki::assert(iter.is_last_child(), SL);
	}

	// erase last child
	{
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		auto traversal = utki::make_traversal(roots);

		auto iter = traversal.make_iterator({1, 0, 2});
		utki::assert(iter.is_last_child(), SL);

		iter = traversal.erase(iter);

		utki::assert(!iter.is_last_child(), SL);
		utki::assert(iter->value == 4, SL);
	}

	// erase very last
	{
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		auto traversal = utki::make_traversal(roots);

		auto iter = traversal.make_iterator({1, 2, 2});
		utki::assert(iter.is_last_child(), SL);

		iter = traversal.erase(iter);

		utki::assert(!iter.is_last_child(), SL);
		utki::assert(iter == traversal.end(), SL);
	}
}
}

void test_utki_tree(){
	basic();
	test_assignment();
	test_move_assignment();
	test_comparison();
	test_constructors_from_container();
	test_traversal();
}
