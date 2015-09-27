/**
 * @author Ivan Gagis <igagis@gmail.com>
 */

#pragma once

#include "types.hpp"
#include "debug.hpp"
#include "util.hpp"

#include <cstring>
#include <ostream>


namespace utki{

//TODO: rewrite using C++11

/**
 * @brief class representing a set of flags.
 * If you define an enumeration according to the following rules:
 * - enumeration is a 'enum class'.
 * - there is no direct assignment of values to enumeration items, i.e. values are in strict ascending order
 * - the very last item is ENUM_SIZE
 * 
 * For example:
 * @code
 * enum class MyEnum{
 *     MY_ZEROTH_ITEM,
 *     MY_FIRST_ITEM,
 *     MY_SECOND_ITEM,
 *     MY_THIRD_ITEM,
 *     ...
 *     ENUM_SIZE
 * };
 * 
 * @endcode
 * Then, the Flags can be used as follows:
 * @code
 * ting::Flags<MyEnum> fs;
 * 
 * fs.Set(MyEnum::MY_FIRST_ITEM, true).Set(MyEnum::MY_THIRD_ITEM, true);
 * 
 * if(fs.Get(MyEnum::MY_FIRST_ITEM)){
 *     //MY_FIRST_ITEM flag is set
 * }
 * 
 * if(fs.Get(MyEnum::MY_ZEROTH_ITEM)){
 *     //Will not get here, since MY_ZEROTH_ITEM flag is not set
 * }
 * 
 * @endcode
 */
template <class T_Enum> class Flags{
public:
	typedef typename utki::uint_size<sizeof(T_Enum)>::type index_t;
	
private:
	std::uint8_t flags[index_t(T_Enum::ENUM_SIZE) / 8 + 1];

public:


	/**
	 * @brief Constructor.
	 * Creates a Flags with all flags initialized to a given value.
	 * @param initialValueOfAllFlags - value to initialize all flags to.
	 */
	Flags(bool initialValueOfAllFlags = false){
		this->setAll(initialValueOfAllFlags);
	}

	/**
	 * @brief Size of the flag set.
	 * @return Number of flags in this flag set.
	 */
	index_t size()const noexcept{
		return index_t(T_Enum::ENUM_SIZE);
	}

	/**
	 * @brief Get value of a given flag.
	 * @param flag - flag to get value of.
	 * @return true if the flag is set.
	 * @return false otherwise.
	 */
	bool get(T_Enum flag)const noexcept{
		ASSERT(flag < T_Enum::ENUM_SIZE)
		return (this->flags[index_t(flag) / 8] & (1 << (index_t(flag) % 8))) != 0;
	}

	/**
	 * @brief Get value for i'th flag.
	 * Returns the value of the flag given by index.
	 * Note, the index must be less than enumeration size,
	 * otherwise the behavior is undefined.
	 * @param i - index of the flag to get value of.
	 * @return value of the flag given by index.
	 */
	bool get(index_t i)const noexcept{
		return this->get(T_Enum(i));
	}

	/**
	 * @brief Set value of a given flag.
	 * @param flag - flag to set value of.
	 * @param value - value to set.
	 * @return Reference to this Flags.
	 */
	Flags& set(T_Enum flag, bool value = true)noexcept{
		ASSERT(flag < T_Enum::ENUM_SIZE)
		if(value){
			this->flags[index_t(flag) / 8] |= (1 << (index_t(flag) % 8));
		}else{
			this->flags[index_t(flag) / 8] &= (~(1 << (index_t(flag) % 8)));
		}
		return *this;
	}

	/**
	 * @brief Clear given flag.
	 * @param flag - flag to clear.
	 * @return Reference to this Flags.
	 */
	Flags& clear(T_Enum flag)noexcept{
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
	Flags& set(index_t i, bool value = true)noexcept{
		return this->set(T_Enum(i), value);
	}

	/**
	 * @brief Clear flag given by index.
	 * Note, the index must be less than enumeration size,
	 * otherwise the behavior is undefined.
	 * @param i - index of the flag to clear.
	 * @return Reference to this Flags.
	 */
	Flags& clear(index_t i)noexcept{
		return this->set(i, false);
	}

	/**
	 * @brief Set all flags to given value.
	 * @param value - value to set all flags to.
	 * @return Reference to this Flags.
	 */
	Flags& setAll(bool value = true)noexcept{
		memset(this->flags, value ? std::uint8_t(-1) : 0, sizeof(this->flags));
		return *this;
	}

	/**
	 * @brief Check if all flags are cleared.
	 * @return true if all flags are cleared.
	 * @return false otherwise.
	 */
	bool isAllClear()const noexcept{
		ASSERT(sizeof(this->flags) > 0)
		for(size_t i = 0; i != sizeof(this->flags) - 1; ++i){
			if(this->flags[i] != 0){
				return false;
			}
		}
		for(index_t i = (this->size() / 8) * 8; i != this->size(); ++i){
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
	bool isAllSet()const noexcept{
		ASSERT(sizeof(this->flags) > 0)
		for(size_t i = 0; i != sizeof(this->flags) - 1; ++i){
			if(this->flags[i] != std::uint8_t(-1)){
				return false;
			}
		}
		for(index_t i = (this->size() / 8) * 8; i != this->size(); ++i){
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
	Flags& invert()noexcept{
		for(size_t i = 0; i != sizeof(this->flags); ++i){
			this->flags[i] = ~this->flags[i];
		}
		return *this;
	}

	/**
	 * @brief Operator NOT.
	 * @return Inverted instance of Flags.
	 */
	Flags operator~()const noexcept{
		return Flags(*this).invert();
	}

	/**
	 * @brief Operator assignment AND.
     * @param f - flags to perform AND operation with.
     * @return Reference to this Flags.
     */
	Flags& operator&=(const Flags& f)noexcept{
		for(size_t i = 0; i != sizeof(this->flags); ++i){
			this->flags[i] &= f.flags[i];
		}
		return *this;
	}
	
	/**
	 * @brief Operator AND.
     * @param f - flags to perform AND operation with.
     * @return Instance of Flags resulting from AND operation.
     */
	Flags operator&(const Flags& f)const noexcept{
		return Flags(*this).operator&=(f);
	}
	
	/**
	 * @brief Operator assignment OR.
     * @param f - flags to perform OR operation with.
     * @return Reference to this Flags.
     */
	Flags& operator|=(const Flags& f)noexcept{
		for(size_t i = 0; i != sizeof(this->flags); ++i){
			this->flags[i] |= f.flags[i];
		}
		return *this;
	}
	
	/**
	 * @brief Operator OR.
     * @param f - flags to perform OR operation with.
     * @return Instance of Flags resulting from OR operation.
     */
	Flags operator|(const Flags& f)const noexcept{
		return Flags(*this).operator|=(f);
	}
	
	/**
	 * @brief Operator assignment XOR.
     * @param f - flags to perform XOR operation with.
     * @return Reference to this Flags.
     */
	Flags& operator^=(const Flags& f)noexcept{
		for(size_t i = 0; i != sizeof(this->flags); ++i){
			this->flags[i] ^= f.flags[i];
		}
		return *this;
	}
	
	/**
	 * @brief Operator OR.
     * @param f - flags to perform OR operation with.
     * @return Instance of Flags resulting from OR operation.
     */
	Flags operator^(const Flags& f)const noexcept{
		return Flags(*this).operator^=(f);
	}
	

	friend std::ostream& operator<<(std::ostream& s, const Flags& fs){
		s << "(";

		for(index_t i = 0; i != fs.size(); ++i){
			s << (fs.get(i) ? "1" : "0");
		}

		s << ")";
		return s;
	}
};


}//~namespace
