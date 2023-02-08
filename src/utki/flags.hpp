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

#include <bitset>
#include <ostream>

#include "debug.hpp"

namespace utki {

/**
 * @brief class representing a set of flags.
 * If you define an enumeration according to the following rules:
 * - enumeration is a 'enum class'.
 * - there is no direct assignment of values to enumeration items, i.e. values are in strict ascending order
 * - the very last item is enum_size
 *
 * For example:
 * @code
 * enum class my_enum{
 *     zeroth_item,
 *     first_item,
 *     second_item,
 *     third_item,
 *     ...
 *     enum_size
 * };
 *
 * @endcode
 * Then, the 'flags' can be used as follows:
 * @code
 * utki::flags<my_enum> fs;
 *
 * fs.set(my_enum::first_item, true).set(my_enum::third_item, true);
 *
 * if(fs.get(my_enum::first_item)){
 *     //first_item flag is set
 * }
 *
 * if(fs.get(my_enum::zeroth_item)){
 *     //Will not get here, since zeroth_item flag is not set
 * }
 *
 * @endcode
 */
template <class enumeration_type>
class flags
{
	static_assert(std::is_enum<enumeration_type>::value, "template parameter must be an enumaration type");

public:
	using enum_type = enumeration_type;

private:
	static_assert(int(enum_type::enum_size) >= 0, "enumeration must define enum_size item");
	static_assert(unsigned(enum_type::enum_size) > 0, "enumeration must define at least one item");

	std::bitset<size_t(enum_type::enum_size)> bitset;

public:
	/**
	 * @brief Constructor.
	 * All flags are initialized to the same given value.
	 * @param initial_value - value which initializes all flags.
	 */
	flags(bool initial_value = false)
	{
		if (initial_value) {
			this->bitset.set();
		}
	}

	/**
	 * @brief Constructor.
	 * @param initially_set_flags - list of flags which will be set initially.
	 */
	flags(std::initializer_list<enum_type> initially_set_flags)
	{
		for (auto v : initially_set_flags) {
			this->bitset.set(size_t(v));
		}
	}

	/**
	 * @brief Constructor.
	 * Constructs a flags instance with only one flag set initially.
	 * @param e - flag to set.
	 */
	flags(enum_type e) :
		flags({e})
	{}

	constexpr size_t size() const noexcept
	{
		return this->bitset.size();
	}

	constexpr bool operator[](enum_type flag) const
	{
		return this->bitset[size_t(flag)];
	}

	typename std::bitset<size_t(enum_type::enum_size)>::reference operator[](enum_type flag)
	{
		return this->bitset[size_t(flag)];
	}

	/**
	 * @brief Get value of a given flag.
	 * @param flag - flag to get value of.
	 * @return true if the flag is set.
	 * @return false otherwise.
	 */
	bool get(enum_type flag) const noexcept
	{
		ASSERT(flag < enum_type::enum_size)
		return this->bitset[size_t(flag)];
	}

	/**
	 * @brief Set value of a given flag.
	 * @param flag - flag to set value of.
	 * @param value - value to set.
	 * @return Reference to this Flags.
	 */
	flags& set(enum_type flag, bool value = true) noexcept
	{
		ASSERT(flag < enum_type::enum_size)
		// flag index cannot exceed the bitset size since
		// it is taken from enum, so just wrap it into a
		// try-catch and ignore the exception to silence linter
		try {
			this->bitset.set(size_t(flag), value);
		} catch (...) {
			ASSERT(false)
		}
		return *this;
	}

	/**
	 * @brief Set all flags to given value.
	 * @param value - value to set all flags to.
	 * @return Reference to this Flags.
	 */
	flags& set(bool value = true) noexcept
	{
		if (value) {
			this->bitset.set();
		} else {
			this->bitset.reset();
		}
		return *this;
	}

	/**
	 * @brief Clear given flag.
	 * @param flag - flag to clear.
	 * @return Reference to this Flags.
	 */
	flags& clear(enum_type flag) noexcept
	{
		ASSERT(flag < enum_type::enum_size)
		// flag index cannot exceed the bitset size since
		// it is taken from enum, so just wrap it into a
		// try-catch and ignore the exception to silence linter
		try {
			this->bitset.reset(size_t(flag));
		} catch (...) {
			ASSERT(false)
		}
		return *this;
	}

	/**
	 * @brief Clear all flags.
	 * @return Reference to this Flags.
	 */
	flags& clear() noexcept
	{
		this->bitset.reset();
		return *this;
	}

	/**
	 * @brief Check if all flags are cleared.
	 * @return true if all flags are cleared.
	 * @return false otherwise.
	 */
	bool is_clear() const noexcept
	{
		return this->bitset.none();
	}

	/**
	 * @brief Check if all flags are set.
	 * @return true if all flags are set.
	 * @return false otherwise.
	 */
	bool is_set() const noexcept
	{
		return this->bitset.all();
	}

	/**
	 * @brief Inverts all the flags.
	 * @return Reference to this Flags.
	 */
	flags& invert() noexcept
	{
		this->bitset.flip();
		return *this;
	}

	/**
	 * @brief Operator NOT EQUALS.
	 * @param f - other set of flags to compare this one to.
	 * @return false if flags set/unset are matching.
	 * @return true otherwise.
	 */
	bool operator!=(const flags& f) const
	{
		return this->bitset != f.bitset;
	}

	/**
	 * @brief Operator LOGIC EQUALS.
	 * @param f - other set of flags to compare this one to.
	 * @return true if flags set/unset are matching.
	 * @return false otherwise.
	 */
	bool operator==(const flags& f) const
	{
		return this->bitset == f.bitset;
	}

	/**
	 * @brief Operator NOT.
	 * @return Inverted instance of Flags.
	 */
	flags operator~() const noexcept
	{
		return flags(*this).invert();
	}

	/**
	 * @brief Operator assignment AND.
	 * @param f - flags to perform AND operation with.
	 * @return Reference to this Flags.
	 */
	flags& operator&=(const flags& f) noexcept
	{
		this->bitset &= f.bitset;
		return *this;
	}

	/**
	 * @brief Operator AND.
	 * @param f - flags to perform AND operation with.
	 * @return Instance of Flags resulting from AND operation.
	 */
	flags operator&(const flags& f) const noexcept
	{
		return flags(*this).operator&=(f);
	}

	/**
	 * @brief Operator assignment OR.
	 * @param f - flags to perform OR operation with.
	 * @return Reference to this Flags.
	 */
	flags& operator|=(const flags& f) noexcept
	{
		this->bitset |= f.bitset;
		return *this;
	}

	/**
	 * @brief Operator OR.
	 * @param f - flags to perform OR operation with.
	 * @return Instance of Flags resulting from OR operation.
	 */
	flags operator|(const flags& f) const noexcept
	{
		return flags(*this).operator|=(f);
	}

	flags operator|(enum_type e) const noexcept
	{
		return flags(*this).set(e);
	}

	/**
	 * @brief Operator assignment XOR.
	 * @param f - flags to perform XOR operation with.
	 * @return Reference to this Flags.
	 */
	flags& operator^=(const flags& f) noexcept
	{
		this->bitset ^= f.bitset;
		return *this;
	}

	/**
	 * @brief Operator OR.
	 * @param f - flags to perform OR operation with.
	 * @return Instance of Flags resulting from OR operation.
	 */
	flags operator^(const flags& f) const noexcept
	{
		return flags(*this).operator^=(f);
	}

	friend std::ostream& operator<<(std::ostream& o, const flags& f)
	{
		return o << f.bitset;
	}
};

/**
 * @brief Construct flags object.
 * @param initially_set_flags - list of flags which will be set initially.
 * @return newly created flags object.
 */
template <class enum_type>
flags<enum_type> make_flags(std::initializer_list<enum_type> initially_set_flags)
{
	return flags<enum_type>(initially_set_flags);
}

template <class enum_type>
struct is_flaggable : public std::false_type {};

} // namespace utki

template <class enum_type>
typename std::enable_if<
	std::is_enum<enum_type>::value
		// TODO: add std::is_scoped_enum check when C++'23 is widely available
		// check that enum has enum_size item. Note, that this is the only way to do it for MSVC compiler.
		&& enum_type::enum_size == enum_type::enum_size,
	utki::flags<enum_type>>::type
operator|(enum_type e1, enum_type e2)
{
	return {e1, e2};
}
