#pragma once

#include "types.hpp"
#include "debug.hpp"
#include "util.hpp"

#include <cstring>
#include <ostream>
#include <array>


namespace utki{

/**
 * @brief class representing a set of flags.
 * If you define an enumeration according to the following rules:
 * - enumeration is a 'enum class'.
 * - there is no direct assignment of values to enumeration items, i.e. values are in strict ascending order
 * - the very last item is ENUM_SIZE
 * 
 * For example:
 * @code
 * enum class my_enum{
 *     zeroth_item,
 *     first_item,
 *     second_item,
 *     third_item,
 *     ...
 *     ENUM_SIZE
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
template <class T_Enum> class flags{
	static_assert(int(T_Enum::ENUM_SIZE) >= 0, "enumeration must define ENUM_SIZE item");
	static_assert(unsigned(T_Enum::ENUM_SIZE) > 0, "enumeration must define at least one item");
public:
	typedef typename utki::uint_size<sizeof(T_Enum)>::type index_type;
	
private:
	std::array<uint8_t, index_type(T_Enum::ENUM_SIZE) / 8 + 1> f;

public:

	/**
	 * @brief Constructor.
	 * All flags are initialized to the same given value.
	 * @param initial_value - value which initializes all flags.
	 */
	flags(bool initial_value = false){
		this->set(initial_value);
	}

	/**
	 * @brief Constructor.
	 * @param initially_set_flags - list of flags which will be set initially.
	 */
	flags(std::initializer_list<T_Enum> initially_set_flags){
		std::fill(this->f.begin(), this->f.end(), 0);
		for(auto v : initially_set_flags){
			this->set(v);
		}
	}

	/**
	 * @brief Size of the flag set.
	 * @return Number of flags in this flag set.
	 */
	index_type size()const noexcept{
		return index_type(T_Enum::ENUM_SIZE);
	}

	/**
	 * @brief Get value of a given flag.
	 * @param flag - flag to get value of.
	 * @return true if the flag is set.
	 * @return false otherwise.
	 */
	bool get(T_Enum flag)const noexcept{
		ASSERT(flag < T_Enum::ENUM_SIZE)
		return (this->f[index_type(flag) / 8] & (1 << (index_type(flag) % 8))) != 0;
	}

	/**
	 * @brief Get value for i'th flag.
	 * Returns the value of the flag given by index.
	 * Note, the index must be less than enumeration size,
	 * otherwise the behavior is undefined.
	 * @param i - index of the flag to get value of.
	 * @return value of the flag given by index.
	 */
	bool get(index_type i)const noexcept{
		return this->get(T_Enum(i));
	}

	/**
	 * @brief Set value of a given flag.
	 * @param flag - flag to set value of.
	 * @param value - value to set.
	 * @return Reference to this Flags.
	 */
	flags& set(T_Enum flag, bool value = true)noexcept{
		ASSERT(flag < T_Enum::ENUM_SIZE)
		if(value){
			this->f[index_type(flag) / 8] |= (1 << (index_type(flag) % 8));
		}else{
			this->f[index_type(flag) / 8] &= (~(1 << (index_type(flag) % 8)));
		}
		return *this;
	}

	/**
	 * @brief Clear given flag.
	 * @param flag - flag to clear.
	 * @return Reference to this Flags.
	 */
	flags& clear(T_Enum flag)noexcept{
		return this->set(flag, false);
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
	flags& set(index_type i, bool value = true)noexcept{
		return this->set(T_Enum(i), value);
	}

	/**
	 * @brief Clear flag given by index.
	 * Note, the index must be less than enumeration size,
	 * otherwise the behavior is undefined.
	 * @param i - index of the flag to clear.
	 * @return Reference to this Flags.
	 */
	flags& clear(index_type i)noexcept{
		return this->set(i, false);
	}

	/**
	 * @brief Set all flags to given value.
	 * @param value - value to set all flags to.
	 * @return Reference to this Flags.
	 */
	flags& set(bool value = true)noexcept{
		std::fill(this->f.begin(), this->f.end(), value ? uint8_t(-1) : 0);
		return *this;
	}

	/**
	 * @brief Clear all flags.
	 * @return Reference to this Flags.
	 */
	flags& clear()noexcept{
		return this->set(false);
	}

	/**
	 * @brief Check if all flags are cleared.
	 * @return true if all flags are cleared.
	 * @return false otherwise.
	 */
	bool is_clear()const noexcept{
		ASSERT_INFO(this->f.size() != 0, "given flags enumeration is empty")
		for(size_t i = 0; i != this->f.size() - 1; ++i){
			if(this->f[i] != 0){
				return false;
			}
		}
		for(index_type i = (this->size() / 8) * 8; i != this->size(); ++i){
			if(this->get(i)){
				return false;
			}
		}
		return true;
	}

	/**
	 * @brief Check if all flags are set.
	 * @return true if all flags are set.
	 * @return false otherwise.
	 */
	bool is_set()const noexcept{
		ASSERT_INFO(this->f.size() != 0, "given flags enumeration is empty")
		for(size_t i = 0; i != this->f.size() - 1; ++i){
			if(this->f[i] != uint8_t(-1)){
				return false;
			}
		}
		for(index_type i = (this->size() / 8) * 8; i != this->size(); ++i){
			if(!this->get(i)){
				return false;
			}
		}
		return true;
	}

	/**
	 * @brief Inverts all the flags.
	 * @return Reference to this Flags.
	 */
	flags& invert()noexcept{
		for(size_t i = 0; i != this->f.size(); ++i){
			this->f[i] = ~this->f[i];
		}
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
		for(size_t i = 0; i != this->f.size(); ++i){
			this->f[i] &= f.f[i];
		}
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
		for(size_t i = 0; i != this->f.size(); ++i){
			this->f[i] |= f.f[i];
		}
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
		for(size_t i = 0; i != this->f.size(); ++i){
			this->f[i] ^= f.f[i];
		}
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

		for(index_type i = 0; i != fs.size(); ++i){
			s << (fs.get(i) ? "1" : "0");
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
