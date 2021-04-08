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

	enum_size
};


void Run(){
	// basic test
	{
		utki::flags<TestEnum> fs;
		
		fs.set(TestEnum::EIGHTH, true).set(TestEnum::SECOND, true).set(TestEnum::EIGHTH, false);
		utki::assert(!fs.get(TestEnum::EIGHTH), SL);
		utki::assert(fs.get(TestEnum::SECOND), SL);
		
		
		TRACE(<< "enum_size = " << size_t(TestEnum::enum_size) << " sizeof(fs) = " << sizeof(fs) << " sizeof(index_t) = " << sizeof(utki::flags<TestEnum>::index_type) << std::endl)
				
		TRACE(<< "fs = " << fs << std::endl)
		
		{
			utki::flags<TestEnum> fs;
			utki::assert(fs.is_clear(), SL);
			utki::assert(!fs.is_set(), SL);
			
			fs.set(fs.size() - 1, true);
			utki::assert(!fs.is_clear(), SL);
			utki::assert(!fs.is_set(), SL);
			
			fs.clear();
			
			fs.set(TestEnum::EIGHTH, true);
			utki::assert(!fs.is_clear(), SL);
			utki::assert(!fs.is_set(), SL);
		}
		{
			utki::flags<TestEnum> fs(true);
			utki::assert(!fs.is_clear(), SL);
			utki::assert(fs.is_set(), SL);
			
			fs.set(fs.size() - 1, false);
			utki::assert(!fs.is_clear(), SL);
			utki::assert(!fs.is_set(), SL);
					
			fs.set();
			
			fs.set(TestEnum::EIGHTH, false);
			utki::assert(!fs.is_clear(), SL);
			utki::assert(!fs.is_set(), SL);
		}
	}

	// test empty initializer list
	{
		auto flags = utki::flags<TestEnum>({});

		utki::assert(flags.is_clear(), SL);
	}

	// test initializer list
	{
		auto flags = utki::flags<TestEnum>({TestEnum::TWENTY_SEVENTH, TestEnum::NINETEENTH});

		utki::assert(!flags.is_clear(), SL);
		utki::assert(flags.get(TestEnum::TWENTY_SEVENTH), SL);
		utki::assert(flags.get(TestEnum::NINETEENTH), SL);

		flags.clear(TestEnum::TWENTY_SEVENTH);
		utki::assert(!flags.is_clear(), SL);
		utki::assert(!flags.get(TestEnum::TWENTY_SEVENTH), SL);
		utki::assert(flags.get(TestEnum::NINETEENTH), SL);
		
		flags.clear(TestEnum::NINETEENTH);
		utki::assert(flags.is_clear(), SL);
		utki::assert(!flags.get(TestEnum::TWENTY_SEVENTH), SL);
		utki::assert(!flags.get(TestEnum::NINETEENTH), SL);
	}

	// test make_flags
	{
		auto flags = make_flags({TestEnum::TWENTY_SEVENTH, TestEnum::NINETEENTH});

		utki::assert(!flags.is_clear(), SL);
		utki::assert(flags.get(TestEnum::TWENTY_SEVENTH), SL);
		utki::assert(flags.get(TestEnum::NINETEENTH), SL);

		flags.clear(TestEnum::TWENTY_SEVENTH);
		utki::assert(!flags.is_clear(), SL);
		utki::assert(!flags.get(TestEnum::TWENTY_SEVENTH), SL);
		utki::assert(flags.get(TestEnum::NINETEENTH), SL);
		
		flags.clear(TestEnum::NINETEENTH);
		utki::assert(flags.is_clear(), SL);
		utki::assert(!flags.get(TestEnum::TWENTY_SEVENTH), SL);
		utki::assert(!flags.get(TestEnum::NINETEENTH), SL);
	}

	// test twice initialized flag
	{
		auto flags = make_flags({TestEnum::NINETEENTH, TestEnum::TWENTY_SEVENTH, TestEnum::NINETEENTH});

		utki::assert(!flags.is_clear(), SL);
		utki::assert(flags.get(TestEnum::TWENTY_SEVENTH), SL);
		utki::assert(flags.get(TestEnum::NINETEENTH), SL);

		flags.clear(TestEnum::TWENTY_SEVENTH);
		utki::assert(!flags.is_clear(), SL);
		utki::assert(!flags.get(TestEnum::TWENTY_SEVENTH), SL);
		utki::assert(flags.get(TestEnum::NINETEENTH), SL);
		
		flags.clear(TestEnum::NINETEENTH);
		utki::assert(flags.is_clear(), SL);
		utki::assert(!flags.get(TestEnum::TWENTY_SEVENTH), SL);
		utki::assert(!flags.get(TestEnum::NINETEENTH), SL);
	}
}

}
