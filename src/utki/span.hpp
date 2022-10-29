/*
The MIT License (MIT)

Copyright (c) 2015-2022 Ivan Gagis <igagis@gmail.com>

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

#include <array>
#include <cstring>
#include <limits>
#include <ostream>
#include <vector>

#ifdef DEBUG
#	include <iostream>
#endif

#include "debug.hpp"
#include "types.hpp"
#include "util.hpp"

// undefine the max macro in case it is defined (e.g. by windows.h)
#ifdef max
#	undef max
#endif

namespace utki {

/**
 * @brief span template class.
 * This class is a wrapper of continuous memory span, it encapsulates pointer to memory block and size of that memory
 * block. It does not own the memory. This is a replacement of std::span when C++'20 is not available.
 */
template <class T>
class span final
{
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type* iterator;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef std::reverse_iterator<iterator> reverse_iterator;

private:
	pointer buf = nullptr;
	size_type buf_size = 0;

public:
	span(const span&) = default;
	span& operator=(const span&) = default;

	/**
	 * @brief Create a span object.
	 * Creates a span object which wraps given memory buffer of specified size.
	 * Note, the memory will not be freed upon this Buffer object destruction,
	 * Buffer does not own the memory.
	 * @param p - pointer to the memory buffer.
	 * @param s - size of the memory buffer.
	 */
	span(pointer p, size_type s) noexcept :
		buf(p),
		buf_size(s)
	{}

	span() noexcept = default;

	/**
	 * @brief Constructor for automatic conversion from nullptr.
	 */
	span(std::nullptr_t) noexcept {}

	/**
	 * @brief Constructor for automatic conversion to span<const T> or other convertible type.
	 * @param sp - span to convert.
	 */
	template < //
		typename TT,
		typename std::enable_if_t< //
			std::is_convertible_v< // note pointers to array, this is because span points to an array of objects
				TT (*)[], // NOLINT(modernize-avoid-c-arrays)
				T (*)[]>, // NOLINT(modernize-avoid-c-arrays)
			bool> = true>
	constexpr span(const span<TT>& sp) noexcept :
		buf(sp.data()),
		buf_size(sp.size())
	{}

	template <size_t array_size>
	span(std::array<typename std::remove_const<T>::type, array_size>& a) :
		span(a.data(), a.size())
	{}

	template <size_t array_size>
	span(const std::array<typename std::remove_const<T>::type, array_size>& a) :
		span(a.data(), a.size())
	{}

	span(std::vector<typename std::remove_const<T>::type>& v) :
		span(v.data(), v.size())
	{}

	template < //
		typename TT,
		typename std::enable_if_t< //
			std::is_convertible_v< //
				TT (*)[], // NOLINT(modernize-avoid-c-arrays)
				const T (*)[]>, // NOLINT(modernize-avoid-c-arrays)
			bool> = true>
	span(const std::vector<TT>& v) :
		span(v.data(), v.size())
	{}

	span(std::basic_string<typename std::remove_const<T>::type>& v) :
		span(v.data(), v.size())
	{}

	template < //
		typename TT,
		typename std::enable_if_t< //
			std::is_convertible_v< //
				TT (*)[], // NOLINT(modernize-avoid-c-arrays)
				const T (*)[]>, // NOLINT(modernize-avoid-c-arrays)
			bool> = true>
	span(const std::basic_string<TT>& v) :
		span(v.data(), v.size())
	{}

	template < //
		typename TT,
		typename std::enable_if_t< //
			std::is_convertible_v< //
				TT (*)[], // NOLINT(modernize-avoid-c-arrays)
				const T (*)[]>, // NOLINT(modernize-avoid-c-arrays)
			bool> = true>
	span(std::basic_string_view<TT> v) :
		span(v.data(), v.size())
	{}

	template < //
		typename TT,
		typename std::enable_if_t< //
			std::is_convertible_v< //
				TT (*)[], // NOLINT(modernize-avoid-c-arrays)
				const char (*)[]>, // NOLINT(modernize-avoid-c-arrays)
			bool> = true>
	span(TT* v) :
		span(v, strlen(v))
	{}

	/**
	 * @brief get span size.
	 * @return number of elements in the span.
	 */
	size_type size() const noexcept
	{
		return this->buf_size;
	}

	/**
	 * @brief Check if span is empty.
	 * @return true if the span is empty.
	 * @return false if the span is not empty.
	 */
	bool empty() const noexcept
	{
		return this->size() == 0;
	}

	/**
	 * @brief get size of span in bytes.
	 * @return size of array in bytes.
	 */
	size_type size_bytes() const noexcept
	{
		return this->size() * sizeof(value_type);
	}

	/**
	 * @brief access specified element of the span.
	 * Const version of operator[].
	 * @param i - element index.
	 * @return reference to i'th element of the span.
	 */
	const_reference operator[](size_type i) const noexcept
	{
		ASSERT(i < this->size())
		return this->buf[i];
	}

	/**
	 * @brief access specified element of the span.
	 * @param i - element index.
	 * @return reference to i'th element of the span.
	 */
	reference operator[](size_type i) noexcept
	{
		ASSERT(i < this->size(), [&](auto& o) {
			o << "operator[](" << i << "): index out of bounds";
		})
		return this->buf[i];
	}

	/**
	 * @brief get iterator to first element of the span.
	 * @return iterator to first element of the span.
	 */
	iterator begin() const noexcept
	{
		return this->buf;
	}

	/**
	 * @brief get const iterator of the first element of the span.
	 * @return const iterator of the first element of the span.
	 */
	iterator cbegin() const noexcept
	{
		return this->buf;
	}

	/**
	 * @brief get iterator to "after last" element of the span.
	 * @return iterator to "after last" element of the span.
	 */
	iterator end() const noexcept
	{
		return this->buf + this->buf_size;
	}

	/**
	 * @brief get const iterator to "after last" element of the span.
	 * @return const iterator to "after last" element of the span.
	 */
	iterator cend() const noexcept
	{
		return this->buf + this->buf_size;
	}

	reverse_iterator crbegin() const noexcept
	{
		return reverse_iterator(this->end());
	}

	reverse_iterator crend() const noexcept
	{
		return reverse_iterator(this->begin());
	}

	reverse_iterator rbegin() const noexcept
	{
		return reverse_iterator(this->end());
	}

	reverse_iterator rend() const noexcept
	{
		return reverse_iterator(this->begin());
	}

	pointer data() const noexcept
	{
		return this->buf;
	}

	pointer end_pointer() const noexcept
	{
		return this->data() + this->size();
	}

	value_type& front() noexcept
	{
		ASSERT(!this->empty())
		return this->operator[](0);
	}

	const value_type& front() const noexcept
	{
		ASSERT(!this->empty())
		return this->operator[](0);
	}

	value_type& back() noexcept
	{
		ASSERT(!this->empty())
		return this->operator[](this->size() - 1);
	}

	const value_type& back() const noexcept
	{
		ASSERT(!this->empty())
		return this->operator[](this->size() - 1);
	}

	span subspan( //
		size_type offset,
		size_type count = std::numeric_limits<size_type>::max()
	) const noexcept
	{
		pointer new_p = offset > this->size() ? this->end() : this->data() + offset;
		ASSERT(new_p <= this->end())
		size_type max_new_s = this->end() - new_p;
		size_type new_s = count > max_new_s ? max_new_s : count;
		ASSERT(new_p + new_s <= this->end())
		return span(new_p, new_s);
	}

	/**
	 * @brief Checks if pointer points somewhere within the span.
	 * @param p - pointer to check.
	 * @return true - if pointer passed as argument points somewhere within the span.
	 * @return false otherwise.
	 */
	bool overlaps(const_pointer p) const noexcept
	{
		return this->begin() <= p && p <= (this->end() - 1);
	}

	friend std::ostream& operator<<(std::ostream& s, const span<T>& buf)
	{
		for (auto& e : buf) {
			s << e;
		}
		return s;
	}

	constexpr bool operator==(const span<const T>& s) const noexcept
	{
		return std::equal(this->begin(), this->end(), s.begin(), s.end());
	}

	constexpr bool operator!=(const span<const T>& s) const noexcept
	{
		return !this->operator==(s);
	}

	constexpr bool operator<(const span<const T>& s) const noexcept
	{
		return std::lexicographical_compare(this->begin(), this->end(), s.begin(), s.end());
	}

	constexpr bool operator>=(const span<const T>& s) const noexcept
	{
		return !this->operator<(s);
	}

	constexpr bool operator>(const span<const T>& s) const noexcept
	{
		return s.operator<(*this);
	}

	constexpr bool operator<=(const span<const T>& s) const noexcept
	{
		return !this->operator>(s);
	}
};

template <class T>
inline utki::span<T> make_span(std::nullptr_t)
{
	return utki::span<T>(nullptr);
}

template <class T>
inline utki::span<T> make_span(T* buf, size_t size)
{
	return utki::span<T>(buf, size);
}

template <class T, std::size_t array_size>
inline utki::span<T> make_span(std::array<T, array_size>& a)
{
	return make_span(a.size() == 0 ? nullptr : a.data(), a.size());
}

template <class T, std::size_t array_size>
inline utki::span<const T> make_span(const std::array<T, array_size>& a)
{
	return make_span(a.size() == 0 ? nullptr : a.data(), a.size());
}

template <class T>
inline utki::span<T> make_span(std::vector<T>& v)
{
	return make_span(v.size() == 0 ? nullptr : v.data(), v.size());
}

template <class T>
inline utki::span<const T> make_span(const std::vector<T>& v)
{
	return make_span(v.size() == 0 ? nullptr : v.data(), v.size());
}

/**
 * @brief Make span representing contents of a basic_string.
 * @param s - string to make the span from.
 * @return span of the string contents.
 */
template <class T>
inline utki::span<const T> make_span(const std::basic_string<T>& s)
{
	return make_span(s.size() == 0 ? nullptr : s.data(), s.size());
}

template <class T>
inline utki::span<T> make_span(std::basic_string<T>& s)
{
	return make_span(s.size() == 0 ? nullptr : s.data(), s.size());
}

template <class T>
inline utki::span<const T> make_span(std::basic_string_view<T> s)
{
	return make_span(s.size() == 0 ? nullptr : s.data(), s.size());
}

/**
 * @brief Make span from zero-terminated string.
 * @param str - zero-terminated string to make span from.
 * @return span representing contents of the string.
 */
inline utki::span<const char> make_span(const char* str)
{
	return make_span(str, strlen(str));
}

template <class value_type>
std::vector<typename std::remove_const<value_type>::type> make_vector(span<value_type> s)
{
	std::vector<typename std::remove_const<value_type>::type> ret;
	ret.reserve(s.size());
	std::copy(s.begin(), s.end(), std::back_inserter(ret));
	return ret;
}

} // namespace utki
