/**
 * @file types.hpp
 * @author Ivan Gagis <igagis@gmail.com>
 * @brief General type utility definitions.
 */

#pragma once

#include <cstdint>
#include <cstddef>


namespace utki{



template <size_t type_size> struct uint_size{typedef std::uint64_t type;};
template <> struct uint_size<1>{typedef std::uint8_t type;};
template <> struct uint_size<2>{typedef std::uint16_t type;};
template <> struct uint_size<3>{typedef std::uint32_t type;};
template <> struct uint_size<4>{typedef std::uint32_t type;};

template <size_t type_size> struct int_size{typedef std::int64_t type;};
template <> struct int_size<1>{typedef std::int8_t type;};
template <> struct int_size<2>{typedef std::int16_t type;};
template <> struct int_size<3>{typedef std::int32_t type;};
template <> struct int_size<4>{typedef std::int32_t type;};



}//~namespace ting
