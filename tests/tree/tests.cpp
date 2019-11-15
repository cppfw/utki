#include "../../src/utki/debug.hpp"
#include "../../src/utki/tree.hpp"

#include "tests.hpp"

using namespace utki;


namespace{
void basic(){
	{
		tree<int> t;

		t.children.push_back(35);

		ASSERT_ALWAYS(t.children.size() == 1)
	}

	{
		tree<int> t{
			{34, 45},

			{
				{78, 89, 96},
				{32, 64, 128},
				decltype(t)(42, {98, 99, 100})
			}
		};

		ASSERT_ALWAYS(t.children.size() == 2)

		ASSERT_ALWAYS(t.children[0].children.size() == 2)
		ASSERT_ALWAYS(t.children[0].children[0].value == 34)
		ASSERT_ALWAYS(t.children[0].children[1].value == 45)

		ASSERT_ALWAYS(t.children[1].children.size() == 3)
		ASSERT_ALWAYS(t.children[1].children[0].children.size() == 3)
		ASSERT_ALWAYS(t.children[1].children[1].children.size() == 3)

		ASSERT_ALWAYS(t.children[1].children[2].children.size() == 3)
		ASSERT_ALWAYS(t.children[1].children[2].value == 42)

		ASSERT_ALWAYS(t.children[1].children[2].children[0].value == 98)
		ASSERT_ALWAYS(t.children[1].children[2].children[1].value == 99)
		ASSERT_ALWAYS(t.children[1].children[2].children[2].value == 100)
	}
}

void test_move_assignment(){
	tree<int> t{
		{34, 45},

		{
			{78, 89, 96},
			{32, 64, 128},
			decltype(t)(42, {98, 99, 100})
		}
	};

	t.children[0].children = std::move(t.children[1].children);

	ASSERT_ALWAYS(t.children.size() == 2)

	ASSERT_ALWAYS(t.children[0].children.size() == 3)
	ASSERT_ALWAYS(t.children[0].children[0].children.size() == 3)
	ASSERT_ALWAYS(t.children[0].children[1].children.size() == 3)

	ASSERT_ALWAYS(t.children[0].children[2].children.size() == 3)
	ASSERT_ALWAYS(t.children[0].children[2].value == 42)

	ASSERT_ALWAYS(t.children[0].children[2].children[0].value == 98)
	ASSERT_ALWAYS(t.children[0].children[2].children[1].value == 99)
	ASSERT_ALWAYS(t.children[0].children[2].children[2].value == 100)

	ASSERT_ALWAYS(t.children[1].children.size() == 0)
}

void test_assignment(){
	tree<int> t{
		{34, 45},

		{
			{78, 89, 96},
			{32, 64, 128},
			decltype(t)(42, {98, 99, 100})
		}
	};

	t.children[0].children = t.children[1].children;

	ASSERT_ALWAYS(t.children.size() == 2)

	ASSERT_ALWAYS(t.children[0].children.size() == 3)
	ASSERT_ALWAYS(t.children[0].children[0].children.size() == 3)
	ASSERT_ALWAYS(t.children[0].children[1].children.size() == 3)

	ASSERT_ALWAYS(t.children[0].children[2].children.size() == 3)
	ASSERT_ALWAYS(t.children[0].children[2].value == 42)

	ASSERT_ALWAYS(t.children[0].children[2].children[0].value == 98)
	ASSERT_ALWAYS(t.children[0].children[2].children[1].value == 99)
	ASSERT_ALWAYS(t.children[0].children[2].children[2].value == 100)

	ASSERT_ALWAYS(t.children[1].children.size() == 3)
}

void test_comparison(){
	tree<int> t{
		{34, 45},

		{
			{78, 89, 96},
			{32, 64, 128},
			decltype(t)(42, {98, 99, 100})
		}
	};


	ASSERT_ALWAYS(t.children.size() == 2)

	ASSERT_ALWAYS(t.children[1].children.size() == 3)
	ASSERT_ALWAYS(t.children[1].children[0].children.size() == 3)
	ASSERT_ALWAYS(t.children[1].children[1].children.size() == 3)

	ASSERT_ALWAYS(t.children[1].children[2].children.size() == 3)
	ASSERT_ALWAYS(t.children[1].children[2] == 42)

	ASSERT_ALWAYS(t.children[1].children[2].children[0] == 98)
	ASSERT_ALWAYS(t.children[1].children[2].children[1] == 99)
	ASSERT_ALWAYS(t.children[1].children[2].children[2] == 100)

	ASSERT_ALWAYS(t.children[1].children.size() == 3)
}
}


void test_utki_tree(){
	basic();
	test_assignment();
	test_move_assignment();
	test_comparison();
}
