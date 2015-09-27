#pragma once

#include "../../src/utki/Singleton.hpp"



class TestSingleton : public ting::Singleton<TestSingleton>{
public:
	int a;

	TestSingleton() :
			a(32)
	{
//		TRACE(<< "TestSingleton(): constructed, this->a = " << this->a << std::endl)
	}
	~TestSingleton()noexcept{
//		TRACE(<< "~TestSingleton(): destructed" << std::endl)
	}
};
