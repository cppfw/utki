#include <tst/set.hpp>
#include <tst/check.hpp>

#include <utki/util.hpp>
#include <utki/types.hpp>

#include <variant>
#include <array>

namespace{
tst::set set("util", [](tst::suite& suite){
	suite.add(
		"serialization_16_bit",
		[](){
			for(uint32_t i = 0; i <= uint16_t(-1); ++i){
				std::array<uint8_t, sizeof(uint16_t)> buf;
				utki::serialize16le(uint16_t(i), &*buf.begin());

				tst::check_eq(buf[0], uint8_t(i & 0xff), SL);
				tst::check_eq(buf[1], uint8_t((i >> 8) & 0xff), SL);

				uint16_t res = utki::deserialize16le(&*buf.begin());
				tst::check_eq(res, uint16_t(i), SL);
			}
		}
	);

	suite.add(
		"serialization_32_bit",
		[](){
			for(uint64_t i = 0; i <= uint32_t(-1); i += 513177){ // increment by 513177, because if increment by 1 it takes too long to run the test
				std::array<uint8_t, sizeof(uint32_t)> buf;
				utki::serialize32le(uint32_t(i), &*buf.begin());

				tst::check_eq(buf[0], uint8_t(i & 0xff), SL);
				tst::check_eq(buf[1], uint8_t((i >> 8) & 0xff), SL);
				tst::check_eq(buf[2], uint8_t((i >> 16) & 0xff), SL);
				tst::check_eq(buf[3], uint8_t((i >> 24) & 0xff), SL);

				uint32_t res = utki::deserialize32le(&*buf.begin());
				tst::check_eq(res, uint32_t(i), SL);
			}
		}
	);

	suite.add(
		"serialization_64_bit",
		[](){
			uint64_t val = 0xabcdef87bcde67af;
			std::array<uint8_t, sizeof(uint64_t)> buf;
			utki::serialize64le(val, buf.data());

			tst::check_eq(buf[0], uint8_t(val & 0xff), SL);
			tst::check_eq(buf[1], uint8_t((val >> 8) & 0xff), SL);
			tst::check_eq(buf[2], uint8_t((val >> 16) & 0xff), SL);
			tst::check_eq(buf[3], uint8_t((val >> 24) & 0xff), SL);
			tst::check_eq(buf[4], uint8_t((val >> 32) & 0xff), SL);
			tst::check_eq(buf[5], uint8_t((val >> 40) & 0xff), SL);
			tst::check_eq(buf[6], uint8_t((val >> 48) & 0xff), SL);
			tst::check_eq(buf[7], uint8_t((val >> 56) & 0xff), SL);

			uint64_t res = utki::deserialize64le(buf.data());
			tst::check_eq(res, val, SL);
		}
	);

	suite.add(
		"scope_exit",
		[](){
			bool flag = false;
			{
				utki::scope_exit se([&flag](){
					flag = true;
				});
			}
			tst::check(flag, SL);
		}
	);

	suite.add(
		"scope_exit_release",
		[](){
			bool flag = false;
			{
				utki::scope_exit se([&flag](){
					flag = true;
				});
				
				se.release();
			}
			tst::check(!flag, SL);
		}
	);

	suite.add(
		"flip_map",
		[](){
			std::map<int, std::string> m = {
				{10, "10"},
				{13, "13"},
				{42, "42"}
			};
			
			auto fm = utki::flip_map(m);
			
			tst::check_eq(fm.size(), size_t(3), SL);
			tst::check_eq(fm["10"], 10, SL);
			tst::check_eq(fm["13"], 13, SL);
			tst::check_eq(fm["42"], 42, SL);
		}
	);

#if M_CPP >= 17 && (M_COMPILER != M_COMPILER_MSVC || M_COMPILER_MSVC_TOOLS_V >= 142)
	suite.add(
		"variant_get_index",
		[](){
			typedef std::variant<int, const char*, std::string, std::pair<bool, int>> variant_type;

			std::array<variant_type, std::variant_size<variant_type>::value> variants = {{
				std::string("hello world!"),
				int(123),
				"const char*",
				std::make_pair(true, 45)
			}};

			tst::check_eq(variants.size(), size_t(4), SL);

			std::vector<std::string> res;

			for(auto& v : variants){
				switch(v.index()){
					case utki::get_index<int, decltype(variants)::value_type>::value:
						res.push_back("int");
						break;
					case utki::get_index<std::pair<bool, int>, decltype(variants)::value_type>::value:
						res.push_back("pair");
						break;
					case utki::get_index<std::string, decltype(variants)::value_type>::value:
						res.push_back("string");
						break;
					case utki::get_index<const char*, decltype(variants)::value_type>::value:
						res.push_back("const char*");
						break;
					default:
						break;
				}
			}

#ifdef DEBUG
			for(auto& r : res){
				utki::log([&](auto&o){o << "r = " << r << std::endl;});
			}
#endif

			std::vector<std::string> expected = {{"string", "int", "const char*", "pair"}};

			tst::check(res == expected, SL);
		}
	);
#endif // ~ non-MSVC compiler or MSVC compiler tools >= v142

});
}
