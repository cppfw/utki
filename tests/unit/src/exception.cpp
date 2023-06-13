#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/exception.hpp>

using namespace std::string_literals;

namespace {
const tst::set set("exception", [](tst::suite& suite) {
	suite.add("exception_message_should_be_stored_in_the_exception_object", [] {
		try {
			throw utki::exception("hello world!");
		} catch (const std::exception& e) {
			tst::check_eq("hello world!"s, std::string(e.what()), SL);
		}
	});
});
} // namespace
