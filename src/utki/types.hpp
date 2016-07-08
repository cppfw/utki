/**
 * @file types.hpp
 * @author Ivan Gagis <igagis@gmail.com>
 * @brief General type utility definitions.
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>


namespace utki{



template <size_t type_size> struct uint_size;
template <> struct uint_size<1>{typedef std::uint8_t type;};
template <> struct uint_size<2>{typedef std::uint16_t type;};
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



template <typename T> struct remove_constptr{
	typedef typename std::remove_const<typename std::remove_pointer<T>::type>::type type;
};


/**
 * @brief Cast pointer to pointer-to-const.
 * @param p - pointer to cast.
 * @return Pointer to const.
 */
template <class T> inline const T* makePtrToConst(T* p){
	return const_cast<const T*>(p);
}


}
