/*
The MIT License (MIT)

utki - Utility Kit for C++.

Copyright (c) 2015-2024 Ivan Gagis <igagis@gmail.com>

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

#include <algorithm>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <unordered_map>

#include "debug.hpp"
#include "span.hpp"
#include "types.hpp"

#ifdef min
#	undef min
#endif

namespace utki {

/**
 * @brief Number of bytyes in kilobyte.
 */
constexpr auto kilobyte = 1024;

constexpr auto cm_per_inch = 2.54;
constexpr auto mm_per_inch = cm_per_inch * 10;

/**
 * @brief Hundred percent constant.
 */
[[deprecated("use std::centi::den")]] constexpr auto hundred_percent = 100;

/**
 * @brief Returns the advance of iterator.
 * Same as std::next() but only moves forward and accepts size_t
 * as number of steps to advance.
 * @param iter - iterator to get advance of.
 * @param n - number of steps to advance.
 * @return Advance of the iterator.
 */
template <typename iterator_type>
iterator_type next(iterator_type iter, size_t n)
{
	constexpr auto max_advance =
		std::numeric_limits<typename std::iterator_traits<iterator_type>::difference_type>::max();
	for (size_t num_left = n;;) {
		if (num_left > size_t(max_advance)) {
			num_left -= max_advance;
			iter = std::next(iter, max_advance);
		} else {
			return std::next(iter, num_left);
		}
	}
}

/**
 * @brief Returns n-th predecessor of iterator.
 * Same as std::prev() but only moves backwards and accepts size_t
 * as number of steps.
 * @param iter - iterator to get predecessor of.
 * @param n - number of steps to go backwards.
 * @return Predecessor of the iterator.
 */
template <typename iterator_type>
iterator_type prev(iterator_type iter, size_t n)
{
	const auto max_advance = std::numeric_limits<typename std::iterator_traits<iterator_type>::difference_type>::max();
	for (size_t num_left = n;;) {
		if (num_left > size_t(max_advance)) {
			num_left -= max_advance;
			iter = std::prev(iter, max_advance);
		} else {
			return std::prev(iter, num_left);
		}
	}
}

/**
 * @brief Offset std::index_sequence by constant value.
 * Adds constant value to each index of a given std::index_sequence type.
 * @tparam offset - index offset to add.
 * @tparam sequence_type - original std::index_sequence to add offset to.
 */
template <size_t offset, typename sequence_type>
struct offset_sequence;

template <size_t offset, size_t... indices>
struct offset_sequence<offset, std::index_sequence<indices...>> {
	using type = std::index_sequence<indices + offset...>;
};

template <size_t offset, typename sequence_type>
using offset_sequence_t = typename offset_sequence<offset, sequence_type>::type;

/**
 * @brief Not to be used directly.
 * Use skip_front<num_to_skip>(<collection>) function.
 */
template <size_t num_to_skip, typename collection_type>
struct skip_front_collection_wrapper {
	collection_type& collection;

	skip_front_collection_wrapper(collection_type& collection) :
		collection(collection)
	{}

	auto begin()
	{
		ASSERT(this->collection.size() >= num_to_skip)
		using std::begin;
		return utki::next(begin(this->collection), num_to_skip);
	}

	auto end()
	{
		using std::end;
		return end(this->collection);
	}
};

/**
 * @brief Create collection wrapper which effectively skips first elements.
 * @tparam num_to_skip - number of front elements to skip.
 * @tparam collection_type - collection type.
 * @param collection - collection to skip first elements of.
 * @return A special wrapper class which provides begin() and end() methods
 *         returning collection iterators.
 */
// TODO: rename to views::drop as in std::
template <size_t num_to_skip, typename collection_type>
auto skip_front(collection_type& collection)
{
	return skip_front_collection_wrapper<num_to_skip, collection_type>(collection);
}

namespace views {

/**
 * @brief Not to be used directly.
 * Use utki::views::reverse(<collection>) function.
 */
template <typename collection_type>
struct reverse_view {
	collection_type& collection;

	reverse_view(collection_type& collection) :
		collection(collection)
	{}

	auto begin()
	{
		using std::rbegin;
		return rbegin(this->collection);
	}

	auto end()
	{
		using std::rend;
		return rend(this->collection);
	}
};

/**
 * @brief Drop-in replacement for std::views::reverse.
 * The reverse_view returns reverse iterators for begin() and end().
 * @tparam collection_type - collection type.
 * @param collection - collection with reversed elements order.
 * @return A special wrapper class which provides begin() and end() methods
 *         returning collection iterators.
 */
template <typename collection_type>
auto reverse(collection_type& collection)
{
	return reverse_view<collection_type>(collection);
}

/**
 * @brief Drop-in replacement for std::ranges::zip_view from C++23.
 * C++17 compatible.
 */
template <typename... collection_type>
struct zip_view {
	std::tuple<collection_type&...> collection;

	size_t min_size;

	zip_view(collection_type&... collection) :
		collection(std::tuple<collection_type&...>(collection...)),
		min_size(std::min({collection.size()...}))
	{}

	class iterator
	{
		std::tuple< //
			std::conditional_t<
				std::is_const_v<collection_type>,
				typename collection_type::const_iterator,
				typename collection_type::iterator>...>
			iters;

	public:
		iterator(decltype(iters) iters) :
			iters(iters)
		{}

		bool operator!=(const iterator& i) const noexcept
		{
			return std::get<0>(iters) != std::get<0>(i.iters);
		}

		iterator& operator++() noexcept
		{
			this->iters = std::apply(
				[](auto&... i) {
					return std::make_tuple(std::next(i)...);
				},
				this->iters
			);

			return *this;
		}

		auto operator*() noexcept
		{
			return std::apply(
				[](auto&... i) {
					return std::tuple<decltype(*i)...>((*i)...);
				},
				this->iters
			);
		}
	};

	auto begin()
	{
		return iterator(
			std::apply(
				[](auto&... c) {
					return std::make_tuple([&]() {
						if constexpr (std::is_const_v<std::remove_reference_t<decltype(c)>>) {
							return c.cbegin();
						} else {
							return c.begin();
						}
					}()...);
				},
				this->collection
			)
		);
	}

	auto end()
	{
		return iterator(
			std::apply(
				[this](auto&... c) {
					return std::make_tuple(utki::next(c.begin(), this->min_size)...);
				},
				this->collection
			)
		);
	}
};

/**
 * @brief Drop-in replacement for std::views::zip from C++23.
 * C++17 compatible.
 */
template <typename... collection_type>
zip_view<collection_type...> zip(collection_type&... collection)
{
	return zip_view<collection_type...>(collection...);
}

} // namespace views

/**
 * @brief Construct std::pair with swapped components.
 * @param p - initial std::pair.
 * @return a new std::pair with swapped component.
 */
template <typename first_type, typename second_type>
std::pair<second_type, first_type> flip_pair(const std::pair<first_type, second_type>& p)
{
	return std::pair<second_type, first_type>(p.second, p.first);
}

/**
 * @brief Flip map's keys and values.
 * @param m - initial map to flip.
 * @return a new map with flipped keys and values in each element.
 */
template <typename key_type, typename value_type, typename comparator_type>
std::map<value_type, key_type> flip_map(const std::map<key_type, value_type, comparator_type>& m)
{
	std::map<value_type, key_type> ret;
	std::transform(m.begin(), m.end(), std::inserter(ret, ret.begin()), flip_pair<key_type, value_type>);
	return ret;
}

/**
 * @brief Flip map's keys and values.
 * This function overload allows supplying the new comparator type for the flipped map.
 * @param m - initial map to flip.
 * @return a new map with flipped keys and values in each element.
 */
template <typename new_comparator_type, typename key_type, typename value_type, typename comparator_type>
std::map<value_type, key_type, new_comparator_type> flip_map(const std::map<key_type, value_type, comparator_type>& m)
{
	std::map<value_type, key_type, new_comparator_type> ret;
	std::transform(m.begin(), m.end(), std::inserter(ret, ret.begin()), flip_pair<key_type, value_type>);
	return ret;
}

/**
 * @brief Check if container contains value.
 * Uses operator==().
 * Also works with std::map and std::unordered_map, in which case it checks
 * for containment of a key.
 *
 * @tparam container_type - type of the container.
 * @tparam value_type - type of the value.
 * @param container - container to check for containing the value.
 * @param value - value to check for.
 * @return true if the container contains the value.
 * @return false otherwise.
 */
template <typename container_type, typename value_type>
bool contains(const container_type& container, const value_type& value)
{
	if constexpr (//
		utki::is_specialization_of_v<std::map, container_type> ||
		utki::is_specialization_of_v<std::unordered_map, container_type>
	)
	{
		return container.find(value) != container.cend();
	} else {
		using std::cbegin;
		using std::cend;
		return std::find(cbegin(container), cend(container), value) != cend(container);
	}
}

/**
 * @brief Execute a function on scope exit.
 * This class can be used to perform some function when the object of this class
 * is destroyed due to leave of the scope where it was created.
 *
 * Example:
 * @code
 * {
 *     utki::scope_exit some_scope_exit([](){
 *         std::cout << "Leaving scope" << std::endl;
 *     });
 * }
 * @endcode
 */
class scope_exit
{
	std::function<void()> f;

public:
	scope_exit(const scope_exit&) = delete;
	scope_exit& operator=(const scope_exit&) = delete;

	scope_exit(scope_exit&&) = delete;
	scope_exit& operator=(scope_exit&&) = delete;

	/**
	 * @brief Constructor.
	 * @param f - function to call on object destruction.
	 */
	scope_exit(decltype(f)&& f) :
		f(std::move(f))
	{}

	~scope_exit() noexcept
	{
		if (this->f) {
			try {
				this->f();
				// destructor must not throw
				// NOLINTNEXTLINE(bugprone-empty-catch)
			} catch (...) {
				// ignore
			}
		}
	}

	/**
	 * @brief Disarm scope exit object.
	 * This function disarms the scope_exit object, so that it will not do any action on destruction.
	 * @return the previous function which had to be executed on object's destruction.
	 */
	decltype(f) release() noexcept
	{
		auto ret = std::move(this->f);
		this->f = nullptr;
		return ret;
	}

	[[deprecated("use scope_exit::release()")]] //
	decltype(f) reset() noexcept
	{
		return this->release();
	}
};

/**
 * @brief Serialize unsigned integral value, little-endian.
 * @param value - value to serialize.
 * @param out_buf - output buffer.
 * @return Pointer to the next byte after serialized value.
 */
template <typename unsigned_type>
uint8_t* serialize_le(unsigned_type value, uint8_t* out_buf) noexcept
{
	static_assert(std::is_integral_v<unsigned_type>, "serialized type must be integral");
	static_assert(std::is_unsigned_v<unsigned_type>, "serialized type must be unsigned");

	unsigned index = 0;
	auto span = utki::make_span(out_buf, sizeof(value));
	for (auto& b : span) {
		b = uint8_t((value >> (byte_bits * index)) & byte_mask);
		++index;
	}

	return &*span.end();
}

/**
 * @brief serialize 16 bit value, little-endian.
 * Serialize 16 bit value, less significant byte first.
 * @param value - the value.
 * @param out_buf - pointer to the 2 byte buffer where the result will be placed.
 * @return pointer to the next byte after serialized value.
 */
inline uint8_t* serialize16le(uint16_t value, uint8_t* out_buf) noexcept
{
	return serialize_le(value, out_buf);
}

/**
 * @brief serialize 32 bit value, little-endian.
 * Serialize 32 bit value, less significant byte first.
 * @param value - the value.
 * @param out_buf - pointer to the 4 byte buffer where the result will be placed.
 * @return pointer to the next byte after serialized value.
 */
inline uint8_t* serialize32le(uint32_t value, uint8_t* out_buf) noexcept
{
	return serialize_le(value, out_buf);
}

/**
 * @brief serialize 64 bit value, little-endian.
 * Serialize 64 bit value, less significant byte first.
 * @param value - the value.
 * @param out_buf - pointer to the 8 byte buffer where the result will be placed.
 * @return pointer to the next byte after serialized value.
 */
inline uint8_t* serialize64le(uint64_t value, uint8_t* out_buf) noexcept
{
	return serialize_le(value, out_buf);
}

/**
 * @brief Deserialize unsigned integral value, little-endian.
 * @param buf - array of bytes to deserialize.
 * @return Deserialized value.
 */
template <typename unsigned_type>
unsigned_type deserialize_le(const uint8_t* buf) noexcept
{
	static_assert(std::is_integral_v<unsigned_type>, "deserialized type must be integral");
	static_assert(std::is_unsigned_v<unsigned_type>, "deserialized type must be unsigned");

	unsigned_type ret = 0;

	unsigned index = 0;

	for (auto b : utki::make_span(buf, sizeof(ret))) {
		ret |= (decltype(ret)(b) << (byte_bits * index));
		++index;
	}

	return ret;
}

/**
 * @brief de-serialize 16 bit value, little-endian.
 * De-serialize 16 bit value from the sequence of bytes. Assume that less significant
 * byte goes first in the input byte sequence.
 * @param buf - pointer to buffer containing 2 bytes to convert from little-endian format.
 * @return 16 bit unsigned integer converted from little-endian byte order to native byte order.
 */
inline uint16_t deserialize16le(const uint8_t* buf) noexcept
{
	return deserialize_le<uint16_t>(buf);
}

/**
 * @brief de-serialize 32 bit value, little-endian.
 * De-serialize 32 bit value from the sequence of bytes. Assume that less significant
 * byte goes first in the input byte sequence.
 * @param buf - pointer to buffer containing 4 bytes to convert from little-endian format.
 * @return 32 bit unsigned integer converted from little-endian byte order to native byte order.
 */
inline uint32_t deserialize32le(const uint8_t* buf) noexcept
{
	return deserialize_le<uint32_t>(buf);
}

/**
 * @brief de-serialize 64 bit value, little-endian.
 * De-serialize 64 bit value from the sequence of bytes. Assume that less significant
 * byte goes first in the input byte sequence.
 * @param buf - pointer to buffer containing 8 bytes to convert from little-endian format.
 * @return 64 bit unsigned integer converted from little-endian byte order to native byte order.
 */
inline uint64_t deserialize64le(const uint8_t* buf) noexcept
{
	return deserialize_le<uint64_t>(buf);
}

/**
 * @brief Serialize float to IEEE 754, little-endian.
 * @param value - flaot value to serialize.
 * @param out_buf - pointer to the 4 byte buffer where the result will be placed.
 * @return pointer to the next byte after serialized value.
 */
inline uint8_t* serialize_float_le(float value, uint8_t* out_buf) noexcept
{
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	auto p = reinterpret_cast<uint8_t*>(&value);
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	return serialize32le(*reinterpret_cast<uint32_t*>(p), out_buf);
}

/**
 * @brief De-serialize IEEE 754 single precision floating point value, little-endian.
 * @param buf - pointer to buffer containing 4 bytes to convert from IEEE 754 little-endian format.
 * @return floating point value.
 */
inline float deserialize_float_le(const uint8_t* buf) noexcept
{
	auto val = deserialize32le(buf);
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	auto p = reinterpret_cast<uint8_t*>(&val);
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	return *reinterpret_cast<float*>(p);
}

/**
 * @brief Serialize unsigned integral value, big-endian.
 * @param value - value to serialize.
 * @param out_buf - output buffer.
 * @return Pointer to the next byte after serialized value.
 */
template <typename unsigned_type>
uint8_t* serialize_be(unsigned_type value, uint8_t* out_buf) noexcept
{
	static_assert(std::is_integral_v<unsigned_type>, "serialized type must be integral");
	static_assert(std::is_unsigned_v<unsigned_type>, "serialized type must be unsigned");

	unsigned index = sizeof(value);
	auto span = utki::make_span(out_buf, sizeof(value));
	for (auto& b : span) {
		ASSERT(index > 0)
		--index;
		auto num_bits_to_shift = byte_bits * index;
		ASSERT(num_bits_to_shift <= sizeof(value) * byte_bits)
		// NOLINTNEXTLINE(clang-analyzer-core.UndefinedBinaryOperatorResult, clang-analyzer-core.BitwiseShift)
		b = uint8_t((value >> num_bits_to_shift) & byte_mask);
	}

	return &*span.end();
}

/**
 * @brief serialize 16 bit value, big-endian.
 * Serialize 16 bit value, most significant byte first.
 * @param value - the value.
 * @param out_buf - pointer to the 2 byte buffer where the result will be placed.
 * @return pointer to the next byte after serialized value.
 */
inline uint8_t* serialize16be(uint16_t value, uint8_t* out_buf) noexcept
{
	return serialize_be(value, out_buf);
}

/**
 * @brief serialize 32 bit value, big-endian.
 * Serialize 32 bit value, most significant byte first.
 * @param value - the value.
 * @param out_buf - pointer to the 4 byte buffer where the result will be placed.
 * @return pointer to the next byte after serialized value.
 */
inline uint8_t* serialize32be(uint32_t value, uint8_t* out_buf) noexcept
{
	return serialize_be(value, out_buf);
}

/**
 * @brief serialize 64 bit value, big-endian.
 * Serialize 64 bit value, most significant byte first.
 * @param value - the value.
 * @param out_buf - pointer to the 8 byte buffer where the result will be placed.
 * @return pointer to the next byte after serialized value.
 */
inline uint8_t* serialize64be(uint64_t value, uint8_t* out_buf) noexcept
{
	return serialize_be(value, out_buf);
}

/**
 * @brief Deserialize unsigned integral value, big-endian.
 * @param buf - array of bytes to deserialize.
 * @return Deserialized value.
 */
template <typename unsigned_type>
unsigned_type deserialize_be(const uint8_t* buf) noexcept
{
	static_assert(std::is_integral_v<unsigned_type>, "deserialized type must be integral");
	static_assert(std::is_unsigned_v<unsigned_type>, "deserialized type must be unsigned");

	unsigned_type ret = 0;

	unsigned index = sizeof(ret);

	for (auto b : utki::make_span(buf, sizeof(ret))) {
		--index;
		ASSERT(index <= sizeof(ret))
		// NOLINTNEXTLINE(clang-analyzer-core.BitwiseShift)
		ret |= (decltype(ret)(b) << (byte_bits * index));
	}

	return ret;
}

/**
 * @brief de-serialize 16 bit value, big-endian.
 * De-serialize 16 bit value from the sequence of bytes. Assume that most significant
 * byte goes first in the input byte sequence.
 * @param buf - pointer to buffer containing 2 bytes to convert from big-endian format.
 * @return 16 bit unsigned integer converted from big-endian byte order to native byte order.
 */
inline uint16_t deserialize16be(const uint8_t* buf) noexcept
{
	return deserialize_be<uint16_t>(buf);
}

/**
 * @brief de-serialize 32 bit value, big-endian.
 * De-serialize 32 bit value from the sequence of bytes. Assume that most significant
 * byte goes first in the input byte sequence.
 * @param buf - pointer to buffer containing 4 bytes to convert from big-endian format.
 * @return 32 bit unsigned integer converted from big-endian byte order to native byte order.
 */
inline uint32_t deserialize32be(const uint8_t* buf) noexcept
{
	return deserialize_be<uint32_t>(buf);
}

/**
 * @brief de-serialize 64 bit value, big-endian.
 * De-serialize 64 bit value from the sequence of bytes. Assume that most significant
 * byte goes first in the input byte sequence.
 * @param buf - pointer to buffer containing 4 bytes to convert from big-endian format.
 * @return 64 bit unsigned integer converted from big-endian byte order to native byte order.
 */
inline uint64_t deserialize64be(const uint8_t* buf) noexcept
{
	return deserialize_be<uint64_t>(buf);
}

/**
 * @brief Serialize float to IEEE 754, big-endian.
 * @param value - flaot value to serialize.
 * @param out_buf - pointer to the 4 byte buffer where the result will be placed.
 * @return pointer to the next byte after serialized value.
 */
inline uint8_t* serialize_float_be(float value, uint8_t* out_buf) noexcept
{
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	auto p = reinterpret_cast<uint8_t*>(&value);
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	return serialize32be(*reinterpret_cast<uint32_t*>(p), out_buf);
}

/**
 * @brief De-serialize IEEE 754 single precision floating point value, big-endian.
 * @param buf - pointer to buffer containing 4 bytes to convert from IEEE 754 big-endian format.
 * @return floating point value.
 */
inline float deserialize_float_be(const uint8_t* buf) noexcept
{
	auto val = deserialize32be(buf);
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	auto p = reinterpret_cast<uint8_t*>(&val);
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	return *reinterpret_cast<float*>(p);
}

/**
 * @brief Check if stderr is terminal or file/pipe.
 * @return true in case stderr outputs to terminal.
 * @return false in case stderr outputs to file or pipe.
 */
bool is_terminal_cerr();

// TODO: remove deprecated stuff
[[deprecated("use is_terminal_cerr()")]] //
inline bool is_cerr_terminal()
{
	return is_terminal_cerr();
}

/**
 * @brief Check if stdout is terminal or file/pipe.
 * @return true in case stdout outputs to terminal.
 * @return false in case stdout outputs to file or pipe.
 */
bool is_terminal_cout();

// TODO: remove deprecated stuff
[[deprecated("use is_terminal_cout()")]] //
inline bool is_cout_terminal()
{
	return is_terminal_cout();
}

/**
 * @brief Check if stdin is terminal or pipe.
 * @return true in case stdin inputs from terminal.
 * @return false in case stdin inputs from pipe.
 */
bool is_terminal_cin();

// TODO: remove deprecated stuff
[[deprecated("use is_terminal_cin()")]] //
inline bool is_cin_terminal()
{
	return is_terminal_cin();
}

/**
 * @brief Version duplet.
 */
struct version_duplet {
	/**
	 * @brief Major version.
	 */
	uint16_t major;

	/**
	 * @brief Minor version.
	 */
	uint16_t minor;

	/**
	 * @brief Combine major and minor versions into one number.
	 */
	constexpr uint32_t to_uint32_t() const noexcept
	{
		return uint32_t(uint32_t(this->major) << sizeof(uint16_t) * utki::byte_bits) | uint32_t(this->minor);
	}

	friend std::ostream& operator<<(std::ostream& o, const version_duplet& vd)
	{
		return o << vd.major << '.' << vd.minor;
	}
};

/**
 * @brief Version triplet.
 * Version of the form "MAJOR.MINOR.REVISON".
 */
struct version_triplet {
	/**
	 * @brief Version duplet.
	 * Major and minor versions.
	 */
	version_duplet duplet;

	/**
	 * @brief Revision.
	 */
	uint32_t revision;

	/**
	 * @brief Parse version triplet from string.
	 * The input string is supposed to contain version triplet at its beginning.
	 * Parsing will stop on the first character right after the version triplet, or at the end of the string.
	 * @param str - string to parse.
	 * @return Parsed version triplet.
	 * @throw std::invalid_argument in case input string does not represent a version triplet.
	 */
	version_triplet from(std::string_view str);
	// TODO: implement

	// TODO: add stream output operator

	// TODO: add unit tests

	// TODO: move out to version.hpp
};

} // namespace utki
