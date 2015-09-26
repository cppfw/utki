/* The MIT License:

Copyright (c) 2008-2014 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */

// Home page: http://ting.googlecode.com



/**
 * @file types.hpp
 * @author Ivan Gagis <igagis@gmail.com>
 * @brief General type utility definitions.
 */

#pragma once

#include <cstdint>
#include <cstddef>


namespace ting{



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
