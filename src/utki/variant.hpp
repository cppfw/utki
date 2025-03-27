/*
The MIT License (MIT)

utki - Utility Kit for C++.

Copyright (c) 2015-2025 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* ================ LICENSE END ================ */

#pragma once

#include <cstddef>
#include <variant>

#include "config.hpp"

namespace utki {

template <typename>
struct internal_helper_tag {};

/**
 * @brief Get variant's alternative index by its type in compile time.
 * @tparam indexed_type - type to get index of.
 * @tparam variant_type - std::variant type to get index from.
 */
template <typename indexed_type, typename variant_type>
struct index_of;

// MSVC compiler prior to tools v142 doesn't compile this
#if CFG_COMPILER != CFG_COMPILER_MSVC || CFG_COMPILER_MSVC_TOOLS_V >= 142
template <typename indexed_type, typename... variant_item_type>
struct index_of<indexed_type, std::variant<variant_item_type...>> :
	std::integral_constant<
		size_t, //
		std::variant<internal_helper_tag<variant_item_type>...>(internal_helper_tag<indexed_type>()).index() //
		> //
{};
#endif

/**
 * @brief Value alias for index_of.
 */
template <typename indexed_type, typename variant_type>
constexpr size_t index_of_v = index_of<indexed_type, variant_type>::value;

/**
 * @brief Functor overloads holder.
 * This is a helper template to be used along with std::visit.
 * Example:
 * @code
 * std::variant<int, bool, double, std::string> var;
 *
 * // ...
 *
 * std::visit(
 *     overloaded{
 *         [](auto arg) {
 *             // template argument overload
 *         },
 *         [](double arg) {
 *              std::cout << "variant stored value is double: " << arg;
 *         },
 *         [](const std::string& arg) {
 *              std::cout << "variant stored value is std::string: " arg;
 *         }
 *     },
 *     var
 * );
 * @endcode
 */
template <class... functor_type>
struct overloaded : functor_type... {
	using functor_type::operator()...;
};

#if CFG_CPP < 20
// explicit deduction guide (not needed as of C++20)
template <class... functor_type>
overloaded(functor_type...) -> overloaded<functor_type...>;
#endif

} // namespace utki
