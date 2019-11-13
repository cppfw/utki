#include "../../src/utki/debug.hpp"
#include "../../src/utki/tree.hpp"

#include "tests.hpp"

using namespace utki;


namespace{
void basic(){
	{
		tree<int> t;

		t.push_back(35);

		ASSERT_ALWAYS(t.size() == 1)
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

		ASSERT_ALWAYS(t.size() == 2)

		ASSERT_ALWAYS(t[0].size() == 2)
		ASSERT_ALWAYS(t[0][0].leaf() == 34)
		ASSERT_ALWAYS(t[0][1].leaf() == 45)

		ASSERT_ALWAYS(t[1].size() == 3)
		ASSERT_ALWAYS(t[1][0].size() == 3)
		ASSERT_ALWAYS(t[1][1].size() == 3)

		ASSERT_ALWAYS(t[1][2].size() == 3)
		ASSERT_ALWAYS(t[1][2].leaf() == 42)

		ASSERT_ALWAYS(t[1][2][0].leaf() == 98)
		ASSERT_ALWAYS(t[1][2][1].leaf() == 99)
		ASSERT_ALWAYS(t[1][2][2].leaf() == 100)
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

	t[0].branches() = std::move(t[1].branches());

	ASSERT_ALWAYS(t.size() == 2)

	ASSERT_ALWAYS(t[0].size() == 3)
	ASSERT_ALWAYS(t[0][0].size() == 3)
	ASSERT_ALWAYS(t[0][1].size() == 3)

	ASSERT_ALWAYS(t[0][2].size() == 3)
	ASSERT_ALWAYS(t[0][2].leaf() == 42)

	ASSERT_ALWAYS(t[0][2][0].leaf() == 98)
	ASSERT_ALWAYS(t[0][2][1].leaf() == 99)
	ASSERT_ALWAYS(t[0][2][2].leaf() == 100)

	ASSERT_ALWAYS(t[1].size() == 0)
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

	t[0].branches() = t[1].branches();

	ASSERT_ALWAYS(t.size() == 2)

	ASSERT_ALWAYS(t[0].size() == 3)
	ASSERT_ALWAYS(t[0][0].size() == 3)
	ASSERT_ALWAYS(t[0][1].size() == 3)

	ASSERT_ALWAYS(t[0][2].size() == 3)
	ASSERT_ALWAYS(t[0][2].leaf() == 42)

	ASSERT_ALWAYS(t[0][2][0].leaf() == 98)
	ASSERT_ALWAYS(t[0][2][1].leaf() == 99)
	ASSERT_ALWAYS(t[0][2][2].leaf() == 100)

	ASSERT_ALWAYS(t[1].size() == 3)
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


	ASSERT_ALWAYS(t.size() == 2)

	ASSERT_ALWAYS(t[1].size() == 3)
	ASSERT_ALWAYS(t[1][0].size() == 3)
	ASSERT_ALWAYS(t[1][1].size() == 3)

	ASSERT_ALWAYS(t[1][2].size() == 3)
	ASSERT_ALWAYS(t[1][2] == 42)

	ASSERT_ALWAYS(t[1][2][0] == 98)
	ASSERT_ALWAYS(t[1][2][1] == 99)
	ASSERT_ALWAYS(t[1][2][2] == 100)

	ASSERT_ALWAYS(t[1].size() == 3)
}
}


void test_utki_tree(){
	basic();
	test_assignment();
	test_move_assignment();
	test_comparison();
}
