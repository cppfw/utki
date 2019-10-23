#include "../../src/utki/debug.hpp"
#include "../../src/utki/tree.hpp"

#include "tests.hpp"

#include <array>

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
		ASSERT_ALWAYS(t[0][0].get() == 34)
		ASSERT_ALWAYS(t[0][1].get() == 45)

		ASSERT_ALWAYS(t[1].size() == 3)
		ASSERT_ALWAYS(t[1][0].size() == 3)
		ASSERT_ALWAYS(t[1][1].size() == 3)

		ASSERT_ALWAYS(t[1][2].size() == 3)
		ASSERT_ALWAYS(t[1][2].get() == 42)

		ASSERT_ALWAYS(t[1][2][0].get() == 98)
		ASSERT_ALWAYS(t[1][2][1].get() == 99)
		ASSERT_ALWAYS(t[1][2][2].get() == 100)
	}
}
}


void test_utki_tree(){
	basic();
}