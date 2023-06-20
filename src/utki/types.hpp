/*
The MIT License (MIT)

Copyright (c) 2015-2023 Ivan Gagis <igagis@gmail.com>

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
#include <cstdint>
#include <memory>
#include <variant>

#include "config.hpp"

namespace utki {

constexpr auto num_bits_in_byte = 8;
constexpr auto num_bits_in_nibble = num_bits_in_byte / 2;

constexpr auto lower_nibble_mask = 0xf;
constexpr auto upper_nibble_mask = 0xf0;
constexpr auto byte_mask = 0xff;

template <size_t type_size>
struct uint_size;

template <>
struct uint_size<1> {
	using type = uint8_t;
};

template <>
struct uint_size<2> {
	using type = uint16_t;
};

template <>
struct uint_size<3> {
	using type = uint32_t;
};

template <>
struct uint_size<4> {
	using type = uint32_t;
};

template <>
// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
struct uint_size<5> {
	using type = uint64_t;
};

template <>
// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
struct uint_size<6> {
	using type = uint64_t;
};

template <>
// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
struct uint_size<7> {
	using type = uint64_t;
};

template <>
// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
struct uint_size<8> {
	using type = uint64_t;
};

template <size_t type_size>
struct int_size;

template <>
struct int_size<1> {
	using type = int8_t;
};

template <>
struct int_size<2> {
	using type = int16_t;
};

template <>
struct int_size<3> {
	using type = int32_t;
};

template <>
struct int_size<4> {
	using type = int32_t;
};

template <>
// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
struct int_size<5> {
	using type = int64_t;
};

template <>
// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
struct int_size<6> {
	using type = int64_t;
};

template <>
// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
struct int_size<7> {
	using type = int64_t;
};

template <>
// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
struct int_size<8> {
	using type = int64_t;
};

template <typename in_type>
struct remove_const_pointer {
	using type = typename std::remove_const_t<typename std::remove_pointer_t<in_type>>;
};

template <typename in_type>
struct remove_const_reference {
	using type = typename std::remove_const_t<typename std::remove_reference_t<in_type>>;
};

/**
 * @brief Cast pointer to pointer-to-const.
 * @param p - pointer to cast.
 * @return Pointer to const.
 */
template <class object_type>
inline const object_type* make_pointer_to_const(object_type* p)
{
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
	return const_cast<const object_type*>(p);
}

/**
 * @brief Dummy class.
 * This class is supposed to be used as template parameter when there
 * is a desire to avoid function overload matching.
 * For example, see utki::span class implementation.
 */
class dummy_class
{};

template <typename>
struct tag {};

template <typename, typename>
struct get_index;

// MSVC compiler prior to tools v142 doesn't compile this
#if CFG_COMPILER != CFG_COMPILER_MSVC || CFG_COMPILER_MSVC_TOOLS_V >= 142
/**
 * @brief Get variant's alternative index by its type in compile time.
 */
template <typename index_type, typename... variant_type>
struct get_index<index_type, std::variant<variant_type...>> :
	std::integral_constant<size_t, std::variant<tag<variant_type>...>(tag<index_type>()).index()> {};
#endif

template <class, class = void>
struct is_type_defined : std::false_type {};

/**
 * @brief Check if owner_type::type is defined.
 * Defines bool 'value' which is true if owner_type::type is defined and false otherwise.
 */
template <class owner_type>
struct is_type_defined<owner_type, std::void_t<typename owner_type::type>> : std::true_type {};

template <class, class = void>
struct type_or_void {
	using type = void;
};

/**
 * @brief Get type or void.
 * @tparam owner_type - type to get owner_type::type from.
 * Defines 'type' member which is same as owner_type::type in case owner_type::type is defined, or void type otherwise.
 */
template <class owner_type>
struct type_or_void<owner_type, std::void_t<typename owner_type::type>> {
	using type = typename owner_type::type;
};

/**
 * @brief Type alias for type_or_void.
 */
template <typename owner_type>
using type_or_void_t = typename type_or_void<owner_type>::type;

} // namespace utki
