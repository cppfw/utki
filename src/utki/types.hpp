/**
 * @file types.hpp
 * @author Ivan Gagis <igagis@gmail.com>
 * @brief General type utility definitions.
 */

#pragma once

#include <cstdint>
#include <cstddef>


namespace utki{



template <size_t type_size> struct UnsignedTypeForSize{typedef std::uint64_t Type;};
template <> struct UnsignedTypeForSize<1>{typedef std::uint8_t Type;};
template <> struct UnsignedTypeForSize<2>{typedef std::uint16_t Type;};
template <> struct UnsignedTypeForSize<3>{typedef std::uint32_t Type;};
template <> struct UnsignedTypeForSize<4>{typedef std::uint32_t Type;};

template <size_t type_size> struct SignedTypeForSize{typedef std::int64_t Type;};
template <> struct SignedTypeForSize<1>{typedef std::int8_t Type;};
template <> struct SignedTypeForSize<2>{typedef std::int16_t Type;};
template <> struct SignedTypeForSize<3>{typedef std::int32_t Type;};
template <> struct SignedTypeForSize<4>{typedef std::int32_t Type;};



}//~namespace ting
