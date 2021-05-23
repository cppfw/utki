#pragma once

#include "debug.hpp"

#include <ostream>
#include <bitset>

namespace utki{

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
template <class enum_type> class flags : public std::bitset<size_t(enum_type::enum_size)>{
	static_assert(int(enum_type::enum_size) >= 0, "enumeration must define enum_size item");
	static_assert(unsigned(enum_type::enum_size) > 0, "enumeration must define at least one item");

public:
	typedef std::bitset<size_t(enum_type::enum_size)> bitset_type;

	/**
	 * @brief Constructor.
	 * All flags are initialized to the same given value.
	 * @param initial_value - value which initializes all flags.
	 */
	flags(bool initial_value = false){
		if(initial_value){
			this->bitset_type::set();
		}
	}

	/**
	 * @brief Constructor.
	 * @param initially_set_flags - list of flags which will be set initially.
	 */
	flags(std::initializer_list<enum_type> initially_set_flags){
		for(auto v : initially_set_flags){
			this->bitset_type::set(size_t(v));
		}
	}

	using bitset_type::operator[];

	constexpr bool operator[](enum_type flag)const{
		return this->bitset_type::operator[](size_t(flag));
	}

	typename bitset_type::reference operator[](enum_type flag){
		return this->bitset_type::operator[](size_t(flag));
	}

	/**
	 * @brief Get value of a given flag.
	 * @param flag - flag to get value of.
	 * @return true if the flag is set.
	 * @return false otherwise.
	 */
	bool get(enum_type flag)const noexcept{
		ASSERT(flag < enum_type::enum_size)
		return this->bitset_type::operator[](size_t(flag));
	}

	/**
	 * @brief Get value for i'th flag.
	 * Returns the value of the flag given by index.
	 * Note, the index must be less than enumeration size,
	 * otherwise the behavior is undefined.
	 * @param i - index of the flag to get value of.
	 * @return value of the flag given by index.
	 */
	bool get(size_t i)const noexcept{
		return this->bitset_type::test(i);
	}

	/**
	 * @brief Set value of a given flag.
	 * @param flag - flag to set value of.
	 * @param value - value to set.
	 * @return Reference to this Flags.
	 */
	flags& set(enum_type flag, bool value = true)noexcept{
		ASSERT(flag < enum_type::enum_size)
		this->bitset_type::set(size_t(flag), value);
		return *this;
	}

	/**
	 * @brief Set value of an i'th flag.
	 * Sets the value of the flag given by index.
	 * Note, the index must be less than enumeration size,
	 * otherwise the behavior is undefined.
	 * @param i - index of the flag to set value of.
	 * @param value - value to set.
	 * @return Reference to this Flags.
	 */
	flags& set(size_t i, bool value = true)noexcept{
		this->bitset_type::set(i, value);
		return *this;
	}

	/**
	 * @brief Set all flags to given value.
	 * @param value - value to set all flags to.
	 * @return Reference to this Flags.
	 */
	flags& set(bool value = true)noexcept{
		if(value){
			this->bitset_type::set();
		}else{
			this->bitset_type::reset();
		}
		return *this;
	}

	/**
	 * @brief Clear given flag.
	 * @param flag - flag to clear.
	 * @return Reference to this Flags.
	 */
	flags& clear(enum_type flag)noexcept{
		this->bitset_type::reset(size_t(flag));
		return *this;
	}

	/**
	 * @brief Clear flag given by index.
	 * Note, the index must be less than enumeration size,
	 * otherwise the behavior is undefined.
	 * @param i - index of the flag to clear.
	 * @return Reference to this Flags.
	 */
	flags& clear(size_t i)noexcept{
		return this->bitset_type::set(i, false);
	}

	/**
	 * @brief Clear all flags.
	 * @return Reference to this Flags.
	 */
	flags& clear()noexcept{
		this->reset();
		return *this;
	}

	/**
	 * @brief Check if all flags are cleared.
	 * @return true if all flags are cleared.
	 * @return false otherwise.
	 */
	bool is_clear()const noexcept{
		return this->none();
	}

	/**
	 * @brief Check if all flags are set.
	 * @return true if all flags are set.
	 * @return false otherwise.
	 */
	bool is_set()const noexcept{
		return this->all();
	}

	/**
	 * @brief Inverts all the flags.
	 * @return Reference to this Flags.
	 */
	flags& invert()noexcept{
		this->flip();
		return *this;
	}

	/**
	 * @brief Operator NOT.
	 * @return Inverted instance of Flags.
	 */
	flags operator~()const noexcept{
		return flags(*this).invert();
	}

	/**
	 * @brief Operator assignment AND.
	 * @param f - flags to perform AND operation with.
	 * @return Reference to this Flags.
	 */
	flags& operator&=(const flags& f)noexcept{
		this->bitset_type::operator&=(f);
		return *this;
	}
	
	/**
	 * @brief Operator AND.
	 * @param f - flags to perform AND operation with.
	 * @return Instance of Flags resulting from AND operation.
	 */
	flags operator&(const flags& f)const noexcept{
		return flags(*this).operator&=(f);
	}
	
	/**
	 * @brief Operator assignment OR.
	 * @param f - flags to perform OR operation with.
	 * @return Reference to this Flags.
	 */
	flags& operator|=(const flags& f)noexcept{
		this->bitset_type::operator|=(f);
		return *this;
	}
	
	/**
	 * @brief Operator OR.
	 * @param f - flags to perform OR operation with.
	 * @return Instance of Flags resulting from OR operation.
	 */
	flags operator|(const flags& f)const noexcept{
		return flags(*this).operator|=(f);
	}
	
	/**
	 * @brief Operator assignment XOR.
	 * @param f - flags to perform XOR operation with.
	 * @return Reference to this Flags.
	 */
	flags& operator^=(const flags& f)noexcept{
		this->bitset_type::operator^=(f);
		return *this;
	}
	
	/**
	 * @brief Operator OR.
	 * @param f - flags to perform OR operation with.
	 * @return Instance of Flags resulting from OR operation.
	 */
	flags operator^(const flags& f)const noexcept{
		return flags(*this).operator^=(f);
	}

	friend std::ostream& operator<<(std::ostream& s, const flags& fs){
		s << "(";

		for(size_t i = 0; i != fs.size(); ++i){
			s << (fs[i] ? "1" : "0");
		}

		s << ")";
		return s;
	}
};

/**
 * @brief Construct flags object.
 * @param initially_set_flags - list of flags which will be set initially.
 * @return newly created flags object.
 */
template <class T> flags<T> make_flags(std::initializer_list<T> initially_set_flags){
	return flags<T>(initially_set_flags);
}

}
