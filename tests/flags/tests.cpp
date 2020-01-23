#include "../../src/utki/debug.hpp"
#include "../../src/utki/flags.hpp"

#include "tests.hpp"



using namespace utki;



namespace TestFlags{

enum class TestEnum{
	ZEROTH,
	FIRST,
	SECOND,
	THIRD,
	FOURTH,
	FIFTH,
	SIXTH,
	SENENTH,
	EIGHTH,
	NINETH,
	TENTH,
	ELEVENTH,
	TWELVETH,
	THIRTEENTH,
	FOURTEENTH,
	FIFTEENTH,
	SIXTEENTH,
	SEVENTEENTH,
	NINETEENTH,
	TWENTYTH,
	TWENTY_FIRST,
	TWENTY_SECOND,
	TWENTY_THIRD,
	TWENTY_FOURTH,
	TWENTY_FIFTH,
	TWENTY_SIXTH,
	TWENTY_SEVENTH,
	TWENTY_EIGHTH,
	TWENTY_NINETH,
	THIRTYTH,
	THIRTY_FIRST,
	THIRTY_SECOND,
	THIRTY_THIRD,
	THIRTY_FOURTH,
	THIRTY_FIFTH,
	THIRTY_SIXTH,
	THIRTY_SENENTH,
	THIRTY_EIGHTH,
	THIRTY_NINETH,

	ENUM_SIZE
};


void Run(){
	// basic test
	{
		utki::flags<TestEnum> fs;
		
		fs.set(TestEnum::EIGHTH, true).set(TestEnum::SECOND, true).set(TestEnum::EIGHTH, false);
		ASSERT_ALWAYS(!fs.get(TestEnum::EIGHTH))
		ASSERT_ALWAYS(fs.get(TestEnum::SECOND))
		
		
		TRACE(<< "ENUM_SIZE = " << size_t(TestEnum::ENUM_SIZE) << " sizeof(fs) = " << sizeof(fs) << " sizeof(index_t) = " << sizeof(utki::flags<TestEnum>::index_type) << std::endl)
				
		TRACE(<< "fs = " << fs << std::endl)
		
		{
			utki::flags<TestEnum> fs;
			ASSERT_ALWAYS(fs.is_clear())
			ASSERT_ALWAYS(!fs.is_set())
			
			fs.set(fs.size() - 1, true);
			ASSERT_ALWAYS(!fs.is_clear())
			ASSERT_ALWAYS(!fs.is_set())
			
			fs.clear();
			
			fs.set(TestEnum::EIGHTH, true);
			ASSERT_ALWAYS(!fs.is_clear())
			ASSERT_ALWAYS(!fs.is_set())
		}
		{
			utki::flags<TestEnum> fs(true);
			ASSERT_ALWAYS(!fs.is_clear())
			ASSERT_ALWAYS(fs.is_set())
			
			fs.set(fs.size() - 1, false);
			ASSERT_ALWAYS(!fs.is_clear())
			ASSERT_ALWAYS(!fs.is_set())
					
			fs.set();
			
			fs.set(TestEnum::EIGHTH, false);
			ASSERT_ALWAYS(!fs.is_clear())
			ASSERT_ALWAYS(!fs.is_set())
		}
	}

	// test empty initializer list
	{
		auto flags = utki::flags<TestEnum>({});

		ASSERT_ALWAYS(flags.is_clear())
	}

	// test initializer list
	{
		auto flags = utki::flags<TestEnum>({TestEnum::TWENTY_SEVENTH, TestEnum::NINETEENTH});

		ASSERT_ALWAYS(!flags.is_clear())
		ASSERT_ALWAYS(flags.get(TestEnum::TWENTY_SEVENTH))
		ASSERT_ALWAYS(flags.get(TestEnum::NINETEENTH))

		flags.clear(TestEnum::TWENTY_SEVENTH);
		ASSERT_ALWAYS(!flags.is_clear())
		ASSERT_ALWAYS(!flags.get(TestEnum::TWENTY_SEVENTH))
		ASSERT_ALWAYS(flags.get(TestEnum::NINETEENTH))
		
		flags.clear(TestEnum::NINETEENTH);
		ASSERT_ALWAYS(flags.is_clear())
		ASSERT_ALWAYS(!flags.get(TestEnum::TWENTY_SEVENTH))
		ASSERT_ALWAYS(!flags.get(TestEnum::NINETEENTH))
	}

	// test make_flags
	{
		auto flags = make_flags({TestEnum::TWENTY_SEVENTH, TestEnum::NINETEENTH});

		ASSERT_ALWAYS(!flags.is_clear())
		ASSERT_ALWAYS(flags.get(TestEnum::TWENTY_SEVENTH))
		ASSERT_ALWAYS(flags.get(TestEnum::NINETEENTH))

		flags.clear(TestEnum::TWENTY_SEVENTH);
		ASSERT_ALWAYS(!flags.is_clear())
		ASSERT_ALWAYS(!flags.get(TestEnum::TWENTY_SEVENTH))
		ASSERT_ALWAYS(flags.get(TestEnum::NINETEENTH))
		
		flags.clear(TestEnum::NINETEENTH);
		ASSERT_ALWAYS(flags.is_clear())
		ASSERT_ALWAYS(!flags.get(TestEnum::TWENTY_SEVENTH))
		ASSERT_ALWAYS(!flags.get(TestEnum::NINETEENTH))
	}

	// test twice initialized flag
	{
		auto flags = make_flags({TestEnum::NINETEENTH, TestEnum::TWENTY_SEVENTH, TestEnum::NINETEENTH});

		ASSERT_ALWAYS(!flags.is_clear())
		ASSERT_ALWAYS(flags.get(TestEnum::TWENTY_SEVENTH))
		ASSERT_ALWAYS(flags.get(TestEnum::NINETEENTH))

		flags.clear(TestEnum::TWENTY_SEVENTH);
		ASSERT_ALWAYS(!flags.is_clear())
		ASSERT_ALWAYS(!flags.get(TestEnum::TWENTY_SEVENTH))
		ASSERT_ALWAYS(flags.get(TestEnum::NINETEENTH))
		
		flags.clear(TestEnum::NINETEENTH);
		ASSERT_ALWAYS(flags.is_clear())
		ASSERT_ALWAYS(!flags.get(TestEnum::TWENTY_SEVENTH))
		ASSERT_ALWAYS(!flags.get(TestEnum::NINETEENTH))
	}
}

}
