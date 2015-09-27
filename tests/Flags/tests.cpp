#include "../../src/utki/debug.hpp"
#include "../../src/utki/Flags.hpp"

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
	TWENTY_SENENTH,
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
	utki::Flags<TestEnum> fs;
	
	fs.set(TestEnum::EIGHTH, true).set(TestEnum::SECOND, true).set(TestEnum::EIGHTH, false);
	ASSERT_ALWAYS(!fs.get(TestEnum::EIGHTH))
	ASSERT_ALWAYS(fs.get(TestEnum::SECOND))
	
	
	TRACE_ALWAYS(<< "ENUM_SIZE = " << size_t(TestEnum::ENUM_SIZE) << " sizeof(fs) = " << sizeof(fs) << " sizeof(index_t) = " << sizeof(utki::Flags<TestEnum>::index_t) << std::endl)
			
	TRACE_ALWAYS(<< "fs = " << fs << std::endl)
	
	{
		utki::Flags<TestEnum> fs;
		ASSERT_ALWAYS(fs.isAllClear())
		ASSERT_ALWAYS(!fs.isAllSet())
		
		fs.set(fs.size() - 1, true);
		ASSERT_ALWAYS(!fs.isAllClear())
		ASSERT_ALWAYS(!fs.isAllSet())
		
		fs.setAll(false);
		
		fs.set(TestEnum::EIGHTH, true);
		ASSERT_ALWAYS(!fs.isAllClear())
		ASSERT_ALWAYS(!fs.isAllSet())
	}
	{
		utki::Flags<TestEnum> fs(true);
		ASSERT_ALWAYS(!fs.isAllClear())
		ASSERT_ALWAYS(fs.isAllSet())
		
		fs.set(fs.size() - 1, false);
		ASSERT_ALWAYS(!fs.isAllClear())
		ASSERT_ALWAYS(!fs.isAllSet())
				
		fs.setAll(true);
		
		fs.set(TestEnum::EIGHTH, false);
		ASSERT_ALWAYS(!fs.isAllClear())
		ASSERT_ALWAYS(!fs.isAllSet())
	}
}

}//~namespace
