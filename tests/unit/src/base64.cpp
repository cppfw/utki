#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/base64.hpp>

using namespace std::string_view_literals;

namespace {
const tst::set set("base64", [](tst::suite& suite) {
	suite.add<std::pair<std::string_view, std::vector<uint8_t>>>( //
		"samples",
		{
			{				""sv,														   {}},
			{            "SA=="sv,														{'H'}},
			{			  "SA"sv,														{'H'}},
			{            "SGU="sv,												   {'H', 'e'}},
			{			 "SGU"sv,												   {'H', 'e'}},
			{            "SGVs"sv,											  {'H', 'e', 'l'}},
			{        "SGVsbA=="sv,										 {'H', 'e', 'l', 'l'}},
			{          "SGVsbA"sv,										 {'H', 'e', 'l', 'l'}},
			{        "SGVsbG8="sv,									{'H', 'e', 'l', 'l', 'o'}},
			{         "SGVsbG8"sv,									{'H', 'e', 'l', 'l', 'o'}},
			{        "SGVsbG8g"sv,							   {'H', 'e', 'l', 'l', 'o', ' '}},
			{    "SGVsbG8gdw=="sv,                          {'H', 'e', 'l', 'l', 'o', ' ', 'w'}},
			{      "SGVsbG8gdw"sv,                          {'H', 'e', 'l', 'l', 'o', ' ', 'w'}},
			{    "SGVsbG8gd28="sv,                     {'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o'}},
			{     "SGVsbG8gd28"sv,                     {'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o'}},
			{    "SGVsbG8gd29y"sv,                {'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r'}},
			{"SGVsbG8gd29ybGQh"sv, {'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'}}
    },
		[](const auto& p) {
			auto res = utki::base64_decode(p.first);
			tst::check(res == p.second, SL);
		}
	);
});
} // namespace
