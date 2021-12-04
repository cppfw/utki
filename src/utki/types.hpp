/*
The MIT License (MIT)

Copyright (c) 2015-2021 Ivan Gagis <igagis@gmail.com>

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

#include "config.hpp"

#include <cstdint>
#include <cstddef>
#include <memory>

#if M_CPP >= 17
#	include <variant>
#endif

namespace utki{

template <size_t type_size> struct uint_size;
template <> struct uint_size<1>{typedef uint8_t type;};
template <> struct uint_size<2>{typedef uint16_t type;};
template <> struct uint_size<3>{typedef uint32_t type;};
template <> struct uint_size<4>{typedef uint32_t type;};
template <> struct uint_size<5>{typedef uint64_t type;};
template <> struct uint_size<6>{typedef uint64_t type;};
template <> struct uint_size<7>{typedef uint64_t type;};
template <> struct uint_size<8>{typedef uint64_t type;};

template <size_t type_size> struct int_size;
template <> struct int_size<1>{typedef std::int8_t type;};
template <> struct int_size<2>{typedef std::int16_t type;};
template <> struct int_size<3>{typedef std::int32_t type;};
template <> struct int_size<4>{typedef std::int32_t type;};
template <> struct int_size<5>{typedef std::int64_t type;};
template <> struct int_size<6>{typedef std::int64_t type;};
template <> struct int_size<7>{typedef std::int64_t type;};
template <> struct int_size<8>{typedef std::int64_t type;};

// TODO: deprecated, remove
template <typename T>
struct remove_constptr{
	[[deprecated("use utki::remove_const_pointer")]]
	typedef typename std::remove_const<typename std::remove_pointer<T>::type>::type type;
};

template <typename T> struct remove_const_pointer{
	typedef typename std::remove_const<typename std::remove_pointer<T>::type>::type type;
};

// TODO: deprecated, remove
template <typename T> struct remove_constref{
	[[deprecated("use utki::remove_const_reference")]]
	typedef typename std::remove_const<typename std::remove_reference<T>::type>::type type;
};

template <typename T> struct remove_const_reference{
	typedef typename std::remove_const<typename std::remove_reference<T>::type>::type type;
};

/**
 * @brief Cast pointer to pointer-to-const.
 * @param p - pointer to cast.
 * @return Pointer to const.
 */
template <class T> inline const T* make_pointer_to_const(T* p){
	return const_cast<const T*>(p);
}

/**
 * @brief Dummy class.
 * This class is supposed to be used as template parameter when there
 * is a desire to avoid function overload matching.
 * For example, see utki::span class implementation.
 */
class dummy_class{};

template <typename> struct tag { };
template <typename T, typename V> struct get_index;

// MSVC compiler prior to tools v142 doesn't compile this
#if M_CPP >= 17 && (M_COMPILER != M_COMPILER_MSVC || M_COMPILER_MSVC_TOOLS_V >= 142)
/**
 * @brief Get variant's alternative index by its type in compile time.
 */
template <typename T, typename... Ts> struct get_index<T, std::variant<Ts...>> :
		std::integral_constant<size_t, std::variant<tag<Ts>...>(tag<T>()).index()>
{};
#endif

template <class T, class = void> struct is_type_defined : std::false_type{};
/**
 * @brief Check if T::type is defined.
 * Defines bool 'value' which is true if T::type is defined and false otherwise.
 */
template <class T> struct is_type_defined<T, std::void_t<typename T::type>> : std::true_type{};

template <class T, class = void> struct type_or_void{
	using type = void;
};

/**
 * @brief Get type or void.
 * @tparam T - type to get T::type from.
 * Defines 'type' member which is same as T::type in case T::type is defined, or void type otherwise.
 */
template <class T> struct type_or_void<T, std::void_t<typename T::type>>{
	using type = typename T::type;
};

}
