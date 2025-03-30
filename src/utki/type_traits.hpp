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
#include <cstdint>
#include <limits>
#include <memory>
#include <variant>

#include "config.hpp"

namespace utki {

[[deprecated("use utki::byte_bits")]] constexpr auto num_bits_in_byte = 8;
constexpr auto byte_bits = 8;
static_assert(std::numeric_limits<uint8_t>::digits == byte_bits, "system with non-8-bit bytes is not supported");
[[deprecated("use utki::nibble_bits")]] constexpr auto num_bits_in_nibble = byte_bits / 2;
constexpr auto nibble_bits = byte_bits / 2;

constexpr auto lower_nibble_mask = 0xf;
constexpr auto upper_nibble_mask = 0xf0;
constexpr auto byte_mask = 0xff;
constexpr auto bit_0_mask = 0x01;
constexpr auto bit_1_mask = 0x02;
constexpr auto bit_2_mask = 0x04;
constexpr auto bit_3_mask = 0x08;
constexpr auto bit_4_mask = 0x10;
constexpr auto bit_5_mask = 0x20;
constexpr auto bit_6_mask = 0x40;
constexpr auto bit_7_mask = 0x80;

/**
 * @brief Smallest unsigned integer type to hold specified number of bytes.
 * @tparam type_size - number of bytes to store.
 */
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

/**
 * @brief Smallest signed integer type to hold specified number of bytes.
 * @tparam type_size - number of bytes to store.
 */
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
using remove_const_pointer_t = typename remove_const_pointer<in_type>::type;

// test remove_const_pointer_t
static_assert(std::is_same_v<remove_const_pointer_t<const char*>, char>, "remove_const_pointer_t test failed");

/**
 * @brief Drop-in replacement for std::remove_cvref from C++20.
 */
template <typename in_type>
struct remove_const_reference {
	using type = typename std::remove_const_t<typename std::remove_reference_t<in_type>>;
};

template <typename in_type>
using remove_const_reference_t = typename remove_const_reference<in_type>::type;

// test remove_const_reference_t
static_assert(std::is_same_v<remove_const_reference_t<const char&>, char>, "remove_const_reference_t test failed");

/**
 * @brief Dummy class.
 * This class is supposed to be used as template parameter when there
 * is a desire to avoid function overload matching.
 * For example, see utki::span class implementation.
 */
// TODO: is needed?
class dummy_class
{};

/**
 * @brief Check if owner_type::type is defined.
 * Defines bool 'value' which is true if owner_type::type is defined and false otherwise.
 * @tparam owner_type - type to check.
 */
template <class owner_type, class = void>
struct is_type_defined : std::false_type {};

// clang-format off

template <class owner_type>
struct is_type_defined<owner_type, std::void_t<typename owner_type::type>> : std::true_type{};

// clang-format on

/**
 * @brief Get type or void.
 * @tparam owner_type - type to get owner_type::type from.
 * Defines 'type' member which is same as owner_type::type in case owner_type::type is defined, or void type otherwise.
 */
template <class owner_type, class = void>
struct type_or_void {
	using type = void;
};

template <class owner_type>
struct type_or_void<owner_type, std::void_t<typename owner_type::type>> {
	using type = typename owner_type::type;
};

/**
 * @brief Type alias for type_or_void.
 */
template <typename owner_type>
using type_or_void_t = typename type_or_void<owner_type>::type;

/**
 * @brief Check if given type is a specialization of given template.
 * @tparam template_templ - template to check for specialization of.
 * @tparam checked_type - type to check for specialization of given template.
 */
template <template <typename...> class template_templ, typename checked_type>
struct is_specialization_of : std::false_type {};

// clang-format off
template <template <typename...> class template_templ, typename... args_type>
struct is_specialization_of<template_templ, template_templ<args_type...>> : std::true_type{};

// clang-format on

/**
 * @brief Variable alias for is_specialization_of.
 */
template <template <typename...> class template_templ, typename checked_type>
constexpr static bool is_specialization_of_v = is_specialization_of<template_templ, checked_type>::value;

/**
 * @brief Drop-in replacement for std::is_scoped_enum from C++23.
 * Check if enumeration type is a scoped enum.
 * C++17 compatible.
 * 
 * Example:
 * @code
 * enum unscoped{a ,b, c};
 * enum class scoped{a, b, c};
 * 
 * static_assert(!is_scoped_enum<unscoped>::value)
 * static_assert(is_scoped_enum<scoped>::value)
 * @endcode
 * 
 * @tparam enum_type - enum to check for being scoped.
 */
template <typename enum_type, bool = std::is_enum_v<enum_type>>
struct is_scoped_enum : std::false_type {};

template <typename enum_type>
struct is_scoped_enum<enum_type, true> :
	std::bool_constant<!std::is_convertible_v<enum_type, typename std::underlying_type_t<enum_type>>> {};

/**
 * @brief Variable alias for is_scoped_enum.
 */
template <typename enum_type>
constexpr bool is_scoped_enum_v = is_scoped_enum<enum_type>::value;

} // namespace utki
