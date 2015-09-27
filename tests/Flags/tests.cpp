#include "../../src/ting/debug.hpp"
#include "../../src/ting/Flags.hpp"

#include "tests.hpp"



using namespace ting;



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
	ting::Flags<TestEnum> fs;
	
	fs.SetTo(TestEnum::EIGHTH, true).SetTo(TestEnum::SECOND, true).SetTo(TestEnum::EIGHTH, false);
	ASSERT_ALWAYS(!fs.Get(TestEnum::EIGHTH))
	ASSERT_ALWAYS(fs.Get(TestEnum::SECOND))
	
	
	TRACE_ALWAYS(<< "ENUM_SIZE = " << size_t(TestEnum::ENUM_SIZE) << " sizeof(fs) = " << sizeof(fs) << " sizeof(index_t) = " << sizeof(ting::Flags<TestEnum>::index_t) << std::endl)
			
	TRACE_ALWAYS(<< "fs = " << fs << std::endl)
	
	{
		ting::Flags<TestEnum> fs;
		ASSERT_ALWAYS(fs.IsAllClear())
		ASSERT_ALWAYS(!fs.IsAllSet())
		
		fs.SetTo(fs.Size() - 1, true);
		ASSERT_ALWAYS(!fs.IsAllClear())
		ASSERT_ALWAYS(!fs.IsAllSet())
		
		fs.SetAllTo(false);
		
		fs.SetTo(TestEnum::EIGHTH, true);
		ASSERT_ALWAYS(!fs.IsAllClear())
		ASSERT_ALWAYS(!fs.IsAllSet())
	}
	{
		ting::Flags<TestEnum> fs(true);
		ASSERT_ALWAYS(!fs.IsAllClear())
		ASSERT_ALWAYS(fs.IsAllSet())
		
		fs.SetTo(fs.Size() - 1, false);
		ASSERT_ALWAYS(!fs.IsAllClear())
		ASSERT_ALWAYS(!fs.IsAllSet())
				
		fs.SetAllTo(true);
		
		fs.SetTo(TestEnum::EIGHTH, false);
		ASSERT_ALWAYS(!fs.IsAllClear())
		ASSERT_ALWAYS(!fs.IsAllSet())
	}
}

}//~namespace
