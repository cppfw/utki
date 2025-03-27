#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/config.hpp>
#include <utki/string.hpp>
#include <utki/variant.hpp>

namespace {
const tst::set set("variant", [](tst::suite& suite) {
#if CFG_COMPILER != CFG_COMPILER_MSVC || CFG_COMPILER_MSVC_TOOLS_V >= 142
	suite.add("index_of", []() {
		using variant_type = std::variant<int, const char*, std::string, std::pair<bool, int>>;

		std::array<variant_type, std::variant_size_v<variant_type>> variants = {
			{std::string("hello world!"), int(123), "const char*", std::make_pair(true, 45)}
		};

		tst::check_eq(variants.size(), size_t(4), SL);

		std::vector<std::string> res;

		for (auto& v : variants) {
			switch (v.index()) {
				case utki::index_of<int, decltype(variants)::value_type>::value:
					res.emplace_back("int");
					break;
				case utki::index_of<std::pair<bool, int>, decltype(variants)::value_type>::value:
					res.emplace_back("pair");
					break;
				case utki::index_of_v<std::string, decltype(variants)::value_type>:
					res.emplace_back("string");
					break;
				case utki::index_of_v<const char*, decltype(variants)::value_type>:
					res.emplace_back("const char*");
					break;
				default:
					break;
			}
		}

#	ifdef DEBUG
		for (auto& r : res) {
			utki::log([&](auto& o) {
				o << "r = " << r << std::endl;
			});
		}
#	endif

		std::vector<std::string> expected = {
			{"string", "int", "const char*", "pair"}
		};

		tst::check(res == expected, SL);
	});
#endif // ~ non-MSVC compiler or MSVC compiler tools >= v142

	suite.add("overloaded", []() {
		using variant_type = std::variant<char, int, const char*, std::string, std::pair<bool, int>>;

		std::array<variant_type, std::variant_size_v<variant_type>> variants = {
			{std::string("hello world!"), int(123), 'a', "const char*", std::make_pair(true, 45)}
		};

		tst::check_eq(variants.size(), size_t(5), SL);

		std::vector<std::string> res;

		for (auto& v : variants) {
			std::visit(
				utki::overloaded{
					[&](auto v) {
						res.emplace_back("auto");
					},
					[&](const std::string s) {
						res.emplace_back(std::string("std::string(): ").append(s));
					},
					[&](int v) {
						res.emplace_back(utki::cat("int = ", v));
					}
				},
				v
			);
		}

#ifdef DEBUG
		for (auto& r : res) {
			utki::log([&](auto& o) {
				o << "r = " << r << std::endl;
			});
		}
#endif

		std::vector<std::string> expected = {
			{"std::string(): hello world!", "int = 123", "auto", "auto", "auto"}
		};

		tst::check(res == expected, SL);
	});
});
} // namespace