/**
 * @author Ivan Gagis <igagis@gmail.com>
 */

#pragma once

#include "types.hpp"
#include "debug.hpp"
#include "util.hpp"

#include <cstring>


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
	typedef typename utki::UnsignedTypeForSize<sizeof(T_Enum)>::Type index_t;
	
private:
	std::uint8_t flags[index_t(T_Enum::ENUM_SIZE) / 8 + 1];

public:


	/**
	 * @brief Constructor.
	 * Creates a Flags with all flags initialized to a given value.
	 * @param initialValueOfAllFlags - value to initialize all flags to.
	 */
	Flags(bool initialValueOfAllFlags = false){
		this->SetAllTo(initialValueOfAllFlags);
	}

	/**
	 * @brief Size of the flag set.
	 * @return Number of flags in this flag set.
	 */
	index_t Size()const noexcept{
		return index_t(T_Enum::ENUM_SIZE);
	}

	/**
	 * @brief Get value of a given flag.
	 * @param flag - flag to get value of.
	 * @return true if the flag is set.
	 * @return false otherwise.
	 */
	bool Get(T_Enum flag)const noexcept{
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
	bool Get(index_t i)const noexcept{
		return this->Get(T_Enum(i));
	}

	/**
	 * @brief Set value of a given flag.
	 * @param flag - flag to set value of.
	 * @param value - value to set.
	 * @return Reference to this Flags.
	 */
	Flags& SetTo(T_Enum flag, bool value)noexcept{
		ASSERT(flag < T_Enum::ENUM_SIZE)
		if(value){
			this->flags[index_t(flag) / 8] |= (1 << (index_t(flag) % 8));
		}else{
			this->flags[index_t(flag) / 8] &= (~(1 << (index_t(flag) % 8)));
		}
		return *this;
	}

	/**
	 * @brief Set given flag.
	 * @param flag - flag to set.
	 * @return Reference to this Flags.
	 */
	Flags& Set(T_Enum flag)noexcept{
		return this->SetTo(flag, true);
	}

	/**
	 * @brief Clear given flag.
	 * @param flag - flag to clear.
	 * @return Reference to this Flags.
	 */
	Flags& Clear(T_Enum flag)noexcept{
		return this->SetTo(flag, false);
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
	Flags& SetTo(index_t i, bool value)noexcept{
		return this->SetTo(T_Enum(i), value);
	}

	/**
	 * @brief Set flag given by index.
	 * Note, the index must be less than enumeration size,
	 * otherwise the behavior is undefined.
	 * @param i - index of the flag to set.
	 * @return Reference to this Flags.
	 */
	Flags& Set(index_t i)noexcept{
		return this->SetTo(i, true);
	}

	/**
	 * @brief Clear flag given by index.
	 * Note, the index must be less than enumeration size,
	 * otherwise the behavior is undefined.
	 * @param i - index of the flag to clear.
	 * @return Reference to this Flags.
	 */
	Flags& Clear(index_t i)noexcept{
		return this->SetTo(i, false);
	}

	/**
	 * @brief Set all flags to given value.
	 * @param value - value to set all flags to.
	 * @return Reference to this Flags.
	 */
	Flags& SetAllTo(bool value)noexcept{
		memset(this->flags, value ? std::uint8_t(-1) : 0, sizeof(this->flags));
		return *this;
	}

	/**
	 * @brief Check if all flags are cleared.
	 * @return true if all flags are cleared.
	 * @return false otherwise.
	 */
	bool IsAllClear()const noexcept{
		ASSERT(sizeof(this->flags) > 0)
		for(size_t i = 0; i != sizeof(this->flags) - 1; ++i){
			if(this->flags[i] != 0){
				return false;
			}
		}
		for(index_t i = (this->Size() / 8) * 8; i != this->Size(); ++i){
			if(this->Get(i)){
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
	bool IsAllSet()const noexcept{
		ASSERT(sizeof(this->flags) > 0)
		for(size_t i = 0; i != sizeof(this->flags) - 1; ++i){
			if(this->flags[i] != std::uint8_t(-1)){
				return false;
			}
		}
		for(index_t i = (this->Size() / 8) * 8; i != this->Size(); ++i){
			if(!this->Get(i)){
				return false;
			}
		}
		return true;
	}

	/**
	 * @brief Inverts all the flags.
	 * @return Reference to this Flags.
	 */
	Flags& Invert()noexcept{
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
		return Flags(*this).Invert();
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
	
#ifdef DEBUG
	friend std::ostream& operator<<(std::ostream& s, const Flags& fs){
		s << "(";

		for(index_t i = 0; i != fs.Size(); ++i){
			s << (fs.Get(i) ? "1" : "0");
		}

		s << ")";
		return s;
	}
#endif
};


}//~namespace
