#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/exception.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace {
class some_unknown_error
{};
} // namespace

namespace {
const tst::set set("exception", [](tst::suite& suite) {
	suite.add("exception_message_should_be_stored_in_the_exception_object", [] {
		try {
			throw utki::exception("hello world!");
		} catch (const std::exception& e) {
			tst::check_eq("hello world!"s, std::string(e.what()), SL);
		}
	});

	suite.add("to_string__std_exception", []() {
		std::string s;
		try {
			throw std::logic_error("some logic error");
		} catch (std::exception& e) {
			s = utki::to_string(e);
		}

		tst::check_eq(s, "std::logic_error: some logic error"s, SL);
	});

	suite.add("to_string__std_nested_exception", []() {
		std::string s;
		try {
			try {
				try {
					try {
						throw std::logic_error("some logic error");
					} catch (...) {
						std::throw_with_nested(std::runtime_error("some_runtime_error"));
					}
				} catch (...) {
					std::throw_with_nested(some_unknown_error());
				}
			} catch (...) {
				std::throw_with_nested(std::invalid_argument("some argument is invalid"));
			}
		} catch (std::exception& e) {
			s = utki::to_string(e, "  "sv);
		}

		tst::check_eq( //
			s,
			"  std::_Nested_exception<std::invalid_argument>: some argument is invalid\n"
			"  std::_Nested_exception<(anonymous namespace)::some_unknown_error>\n"
			"  std::_Nested_exception<std::runtime_error>: some_runtime_error\n"
			"  std::logic_error: some logic error"s,
			SL
		);
	});

	suite.add("current_exception_to_string", []() {
		std::string s;

		try {
			throw some_unknown_error();
		} catch (...) {
			s = utki::current_exception_to_string("  "sv);
		}

		tst::check_eq(s, "  (anonymous namespace)::some_unknown_error"s, SL);
	});
});
} // namespace
