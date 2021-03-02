#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>

#if __cplusplus >= 201703L
#	include <variant>
#endif

namespace utki{

template <size_t type_size> struct uint_size;
template <> struct uint_size<1>{typedef uint8_t type;};
template <> struct uint_size<2>{typedef uint16_t type;};
template <> struct uint_size<3>{typedef std::uint32_t type;};
template <> struct uint_size<4>{typedef std::uint32_t type;};
template <> struct uint_size<5>{typedef std::uint64_t type;};
template <> struct uint_size<6>{typedef std::uint64_t type;};
template <> struct uint_size<7>{typedef std::uint64_t type;};
template <> struct uint_size<8>{typedef std::uint64_t type;};

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
template <typename T> struct remove_constptr{
	typedef typename std::remove_const<typename std::remove_pointer<T>::type>::type type;
};

template <typename T> struct remove_const_pointer{
	typedef typename std::remove_const<typename std::remove_pointer<T>::type>::type type;
};

// TODO: deprecated, remove
template <typename T> struct remove_constref{
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

#if __cplusplus >= 201703L
template <typename> struct tag { };
template <typename T, typename V> struct get_index;

/**
 * @brief Get variant's alternative index by its type in compile time.
 */
template <typename T, typename... Ts> struct get_index<T, std::variant<Ts...>> :
		std::integral_constant<size_t, std::variant<tag<Ts>...>(tag<T>()).index()>
{};

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
 * @param T - type to get T::type from.
 * Defines 'type' member which is same as T::type in case T::type is defined, or void type otherwise.
 */
template <class T> struct type_or_void<T, std::void_t<typename T::type>>{
	using type = typename T::type;
};

#endif

}
