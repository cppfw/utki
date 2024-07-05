#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/exception.hpp>
#include <utki/string.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace {
class some_unknown_error
{};
} // namespace

namespace {
const tst::set set("exception", [](tst::suite& suite) {
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

		auto actual = utki::split(s, '\n');

		auto expected_prefix = "  std::"sv;

		std::vector<std::string> expected = {
			"<std::invalid_argument>: some argument is invalid"s,
			"<(anonymous namespace)::some_unknown_error>"s,
			"<std::runtime_error>: some_runtime_error"s,
			"  std::logic_error: some logic error"s
		};

		tst::check_eq(actual.size(), expected.size(), SL);

		for (auto ei = expected.begin(), ai = actual.begin(); ei != expected.end(); ++ei, ++ai) {
			tst::check(utki::starts_with(*ai, expected_prefix), SL);
			tst::check(utki::ends_with(*ai, *ei), SL);
		}
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

	suite.add("stacked_exception_to_string", []() {
		std::string s;
		try {
			try {
				try {
					try {
						throw some_unknown_error();
					} catch (...) {
						utki::throw_with_nested(std::logic_error("some logic error"));
					}
				} catch (...) {
					utki::throw_with_nested(std::runtime_error("some_runtime_error"));
				}
			} catch (...) {
				utki::throw_with_nested(std::invalid_argument("some argument is invalid"));
			}
		} catch (utki::exception& e) {
			s = e.to_string("  "sv);
		}

		auto actual = utki::split(s, '\n');

		std::vector<std::string> expected = {
			"  std::invalid_argument: some argument is invalid"s,
			"  std::runtime_error: some_runtime_error"s,
			"  std::logic_error: some logic error"s,
			"  (anonymous namespace)::some_unknown_error"s
		};

		tst::check_eq(actual.size(), expected.size(), SL);

		for (auto ei = expected.begin(), ai = actual.begin(); ei != expected.end(); ++ei, ++ai) {
			tst::check_eq(*ai, *ei, SL);
		}
	});

	suite.add("stacked_exception_what", []() {
		std::string s;
		try {
			try {
				try {
					try {
						throw some_unknown_error();
					} catch (...) {
						utki::throw_with_nested(std::logic_error("some logic error"));
					}
				} catch (...) {
					utki::throw_with_nested(std::runtime_error("some_runtime_error"));
				}
			} catch (...) {
				utki::throw_with_nested(std::invalid_argument("some argument is invalid"));
			}
		} catch (std::exception& e) {
			s = e.what();
		}

		auto actual = utki::split(s, '\n');

		std::vector<std::string> expected = {
			"exception stack:"s,
			"- std::invalid_argument: some argument is invalid"s,
			"- std::runtime_error: some_runtime_error"s,
			"- std::logic_error: some logic error"s,
			"- (anonymous namespace)::some_unknown_error"s
		};

		tst::check_eq(actual.size(), expected.size(), SL);

		for (auto ei = expected.begin(), ai = actual.begin(); ei != expected.end(); ++ei, ++ai) {
			tst::check_eq(*ai, *ei, SL);
		}
	});
});
} // namespace
