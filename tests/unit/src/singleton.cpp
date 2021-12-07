#include <tst/set.hpp>
#include <tst/check.hpp>

#include <utki/config.hpp>
#include <utki/singleton.hpp>

#if M_COMPILER != M_COMPILER_MSVC

namespace{
class test_singleton : public utki::singleton<test_singleton>{
public:
	int a = 13;
};
}

namespace{
tst::set set("singleton", [](tst::suite& suite){
	suite.add(
		"only_one_singleton_instance_can_exist_at_a_time",
		tst::flag::no_parallel,
		[]{
			test_singleton sing1;

			try{
				test_singleton sing2;
				tst::check(false, SL) << "creating second singleton object should throw";
			}catch(std::logic_error&){}
		}
	);
});
}
#endif // ~msvc compiler
