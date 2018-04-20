#pragma once


#include <functional>
#include <map>
#include <algorithm>

#include "debug.hpp"



namespace utki{


template<typename T_A, typename T_B> std::pair<T_B, T_A> flipPair(const std::pair<T_A, T_B> &p){
    return std::pair<T_B, T_A>(p.second, p.first);
}

template<typename T_A, typename T_B> std::map<T_B, T_A> flipMap(const std::map<T_A, T_B> &m){
    std::map<T_B, T_A> ret;
    std::transform(m.begin(), m.end(), std::inserter(ret, ret.begin()), flipPair<T_A, T_B>);
    return ret;
}

/**
 * @brief Execute a function on scope exit.
 * This class can be used to perform some function when the object of this class
 * is destroyed due to leave of the scope where it was created.
 * 
 * Example:
 * @code
 * {
 *     utki::ScopeExit scopeExit([](){
 *         std::cout << "Leaving scope" << std::endl;
 *     });
 * }
 * @endcode
 */
class ScopeExit{
	std::function<void()> f;

public:
	ScopeExit(const ScopeExit&) = delete;
	ScopeExit& operator=(const ScopeExit&) = delete;
	
	/**
	 * @brief Constructor.
	 * @param f - function to call on object destruction.
	 */
	ScopeExit(decltype(f)&& f) :
			f(std::move(f))
	{}
	
    ~ScopeExit()noexcept{
		if(this->f){
			this->f();
		}
	}
	
	/**
	 * @brief Disarm scope exit object.
	 * This function disarms the ScopeExit object, so that it will not do any action on destruction.
	 * @return the previous function which had to be executed on object's destruction.
	 */
	decltype(f) reset()noexcept{
		auto ret = std::move(this->f);
		this->f = nullptr;
		return ret;
	}
};



/**
 * @brief Get top-clamped value.
 * @param v - value to get clamped for.
 * @param top - top to clamp to.
 * @return clamped value.
 */
template <class T> inline T clampedTop(T v, const T top)noexcept{
	if(v > top){
		return top;
	}
	return v;
}



/**
 * @brief Clamp value top.
 * This inline template function can be used to clamp the top of the value.
 * Example:
 * @code
 * int a = 30;
 *
 * //Clamp to 40. Value of 'a' remains unchanged,
 * //since it is already less than 40.
 * utki::clampTop(a, 40);
 * std::cout << a << std::endl;
 *
 * //Clamp to 27. Value of 'a' is changed to 27,
 * //since it is 30 which is greater than 27.
 * utki::clampTop(a, 27);
 * std::cout << a << std::endl;
 * @endcode
 * As a result, this will print:
 * @code
 * 30
 * 27
 * @endcode
 * @param v - reference to the value which top is to be clamped.
 * @param top - value to clamp the top to.
 */
template <class T> inline void clampTop(T& v, const T top)noexcept{
	v = clampedTop(v, top);
}



/**
 * @brief Get bottom-clamped value.
 * @param v - value to get clamped for.
 * @param bottom - bottom to clamp to.
 * @return clamped value.
 */
template <class T> inline T clampedBottom(T v, const T bottom)noexcept{
	if(v < bottom){
		return bottom;
	}
	return v;
}




/**
 * @brief Clamp value bottom.
 * Usage is analogous to utki::clampTop.
 * @param v - reference to the value which bottom is to be clamped.
 * @param bottom - value to clamp the bottom to.
 */
template <class T> inline void clampBottom(T& v, const T bottom)noexcept{
	v = clampedBottom(v, bottom);
}



/**
 * @brief Get range-clamped value.
 * @param v - value to get clamped for.
 * @param bottom - value to clamp the bottom to.
 * @param top - value to clamp the top to.
 * @return clamped value.
 */
template <class T> inline T clampedRange(T v, const T bottom, const T top)noexcept{
	if(v < bottom){
		return bottom;
	}
	if(v > top){
		return top;
	}
	return v;
}



/**
 * @brief Clamp value to range.
 * Clamps given value to given range.
 * @param v - reference to the value which is to be clamped.
 * @param bottom - value to clamp the bottom to.
 * @param top - value to clamp the top to.
 */
template <class T> inline void clampRange(T& v, const T bottom, const T top)noexcept{
	v = clampedRange(v, bottom, top);
}



/**
 * @brief serialize 16 bit value, little-endian.
 * Serialize 16 bit value, less significant byte first.
 * @param value - the value.
 * @param out_buf - pointer to the 2 byte buffer where the result will be placed.
 * @return pointer to the next byte after serialized value.
 */
inline std::uint8_t* serialize16LE(std::uint16_t value, std::uint8_t* out_buf)noexcept{
	*out_buf = value & 0xff;
	++out_buf;
	*out_buf = value >> 8;
	++out_buf;
	return out_buf;
}



/**
 * @brief serialize 32 bit value, little-endian.
 * Serialize 32 bit value, less significant byte first.
 * @param value - the value.
 * @param out_buf - pointer to the 4 byte buffer where the result will be placed.
 * @return pointer to the next byte after serialized value.
 */
inline std::uint8_t* serialize32LE(std::uint32_t value, std::uint8_t* out_buf)noexcept{
	*out_buf = std::uint8_t(value & 0xff);
	++out_buf;
	value >>= 8;
	*out_buf = std::uint8_t(value & 0xff);
	++out_buf;
	value >>= 8;
	*out_buf = std::uint8_t(value & 0xff);
	++out_buf;
	value >>= 8;
	*out_buf = std::uint8_t(value & 0xff);
	++out_buf;
	return out_buf;
}



/**
 * @brief serialize 64 bit value, little-endian.
 * Serialize 64 bit value, less significant byte first.
 * @param value - the value.
 * @param out_buf - pointer to the 8 byte buffer where the result will be placed.
 * @return pointer to the next byte after serialized value.
 */
inline std::uint8_t* serialize64LE(std::uint64_t value, std::uint8_t* out_buf)noexcept{
	*out_buf = std::uint8_t(value & 0xff);
	++out_buf;
	value >>= 8;
	*out_buf = std::uint8_t(value & 0xff);
	++out_buf;
	value >>= 8;
	*out_buf = std::uint8_t(value & 0xff);
	++out_buf;
	value >>= 8;
	*out_buf = std::uint8_t(value & 0xff);
	++out_buf;
	value >>= 8;
	*out_buf = std::uint8_t(value & 0xff);
	++out_buf;
	value >>= 8;
	*out_buf = std::uint8_t(value & 0xff);
	++out_buf;
	value >>= 8;
	*out_buf = std::uint8_t(value & 0xff);
	++out_buf;
	value >>= 8;
	*out_buf = std::uint8_t(value & 0xff);
	++out_buf;
	return out_buf;
}



/**
 * @brief de-serialize 16 bit value, little-endian.
 * De-serialize 16 bit value from the sequence of bytes. Assume that less significant
 * byte goes first in the input byte sequence.
 * @param buf - pointer to buffer containing 2 bytes to convert from little-endian format.
 * @return 16 bit unsigned integer converted from little-endian byte order to native byte order.
 */
inline std::uint16_t deserialize16LE(const std::uint8_t* buf)noexcept{
	std::uint16_t ret;

	//assume little-endian
	ret = std::uint16_t(*buf);
	++buf;
	ret |= ((std::uint16_t(*buf)) << 8);

	return ret;
}



/**
 * @brief de-serialize 32 bit value, little-endian.
 * De-serialize 32 bit value from the sequence of bytes. Assume that less significant
 * byte goes first in the input byte sequence.
 * @param buf - pointer to buffer containing 4 bytes to convert from little-endian format.
 * @return 32 bit unsigned integer converted from little-endian byte order to native byte order.
 */
inline std::uint32_t deserialize32LE(const std::uint8_t* buf)noexcept{
	std::uint32_t ret;

	//assume little-endian
	ret = std::uint32_t(*buf);
	++buf;
	ret |= ((std::uint32_t(*buf)) << 8);
	++buf;
	ret |= ((std::uint32_t(*buf)) << 16);
	++buf;
	ret |= ((std::uint32_t(*buf)) << 24);

	return ret;
}



/**
 * @brief de-serialize 64 bit value, little-endian.
 * De-serialize 64 bit value from the sequence of bytes. Assume that less significant
 * byte goes first in the input byte sequence.
 * @param buf - pointer to buffer containing 8 bytes to convert from little-endian format.
 * @return 64 bit unsigned integer converted from little-endian byte order to native byte order.
 */
inline std::uint64_t deserialize64LE(const std::uint8_t* buf)noexcept{
	std::uint64_t ret;

	//assume little-endian
	ret = std::uint64_t(*buf);
	++buf;
	ret |= ((std::uint64_t(*buf)) << 8);
	++buf;
	ret |= ((std::uint64_t(*buf)) << 16);
	++buf;
	ret |= ((std::uint64_t(*buf)) << 24);
	++buf;
	ret |= ((std::uint64_t(*buf)) << 32);
	++buf;
	ret |= ((std::uint64_t(*buf)) << 40);
	++buf;
	ret |= ((std::uint64_t(*buf)) << 48);
	++buf;
	ret |= ((std::uint64_t(*buf)) << 56);

	return ret;
}




/**
 * @brief serialize 16 bit value, big-endian.
 * Serialize 16 bit value, most significant byte first.
 * @param value - the value.
 * @param out_buf - pointer to the 2 byte buffer where the result will be placed.
 * @return pointer to the next byte after serialized value.
 */
inline std::uint8_t* serialize16BE(std::uint16_t value, std::uint8_t* out_buf)noexcept{
	*out_buf = value >> 8;
	++out_buf;
	*out_buf = value & 0xff;
	++out_buf;
	return out_buf;
}



/**
 * @brief serialize 32 bit value, big-endian.
 * Serialize 32 bit value, most significant byte first.
 * @param value - the value.
 * @param out_buf - pointer to the 4 byte buffer where the result will be placed.
 * @return pointer to the next byte after serialized value.
 */
inline std::uint8_t* serialize32BE(std::uint32_t value, std::uint8_t* out_buf)noexcept{
	*out_buf = std::uint8_t((value >> 24) & 0xff);
	++out_buf;
	*out_buf = std::uint8_t((value >> 16) & 0xff);
	++out_buf;
	*out_buf = std::uint8_t((value >> 8) & 0xff);
	++out_buf;
	*out_buf = std::uint8_t(value & 0xff);
	++out_buf;
	return out_buf;
}



/**
 * @brief serialize 64 bit value, big-endian.
 * Serialize 64 bit value, most significant byte first.
 * @param value - the value.
 * @param out_buf - pointer to the 8 byte buffer where the result will be placed.
 * @return pointer to the next byte after serialized value.
 */
inline std::uint8_t* serialize64BE(std::uint64_t value, std::uint8_t* out_buf)noexcept{
	*out_buf = std::uint8_t((value >> 56) & 0xff);
	++out_buf;
	*out_buf = std::uint8_t((value >> 48) & 0xff);
	++out_buf;
	*out_buf = std::uint8_t((value >> 40) & 0xff);
	++out_buf;
	*out_buf = std::uint8_t((value >> 32) & 0xff);
	++out_buf;
	*out_buf = std::uint8_t((value >> 24) & 0xff);
	++out_buf;
	*out_buf = std::uint8_t((value >> 16) & 0xff);
	++out_buf;
	*out_buf = std::uint8_t((value >> 8) & 0xff);
	++out_buf;
	*out_buf = std::uint8_t(value & 0xff);
	++out_buf;
	return out_buf;
}



/**
 * @brief de-serialize 16 bit value, big-endian.
 * De-serialize 16 bit value from the sequence of bytes. Assume that most significant
 * byte goes first in the input byte sequence.
 * @param buf - pointer to buffer containing 2 bytes to convert from big-endian format.
 * @return 16 bit unsigned integer converted from big-endian byte order to native byte order.
 */
inline std::uint16_t deserialize16BE(const std::uint8_t* buf)noexcept{
	std::uint16_t ret;

	//assume big-endian
	ret = ((std::uint16_t(*buf)) << 8);
	++buf;
	ret |= std::uint16_t(*buf);

	return ret;
}



/**
 * @brief de-serialize 32 bit value, big-endian.
 * De-serialize 32 bit value from the sequence of bytes. Assume that most significant
 * byte goes first in the input byte sequence.
 * @param buf - pointer to buffer containing 4 bytes to convert from big-endian format.
 * @return 32 bit unsigned integer converted from big-endian byte order to native byte order.
 */
inline std::uint32_t deserialize32BE(const std::uint8_t* buf)noexcept{
	std::uint32_t ret;

	//assume big-endian
	ret = ((std::uint32_t(*buf)) << 24);
	++buf;
	ret |= ((std::uint32_t(*buf)) << 16);
	++buf;
	ret |= ((std::uint32_t(*buf)) << 8);
	++buf;
	ret |= std::uint32_t(*buf);

	return ret;
}



/**
 * @brief de-serialize 64 bit value, big-endian.
 * De-serialize 64 bit value from the sequence of bytes. Assume that most significant
 * byte goes first in the input byte sequence.
 * @param buf - pointer to buffer containing 4 bytes to convert from big-endian format.
 * @return 64 bit unsigned integer converted from big-endian byte order to native byte order.
 */
inline std::uint64_t deserialize64BE(const std::uint8_t* buf)noexcept{
	std::uint64_t ret;

	//assume big-endian
	ret = ((std::uint64_t(*buf)) << 56);
	++buf;
	ret = ((std::uint64_t(*buf)) << 48);
	++buf;
	ret = ((std::uint64_t(*buf)) << 40);
	++buf;
	ret = ((std::uint64_t(*buf)) << 32);
	++buf;
	ret = ((std::uint64_t(*buf)) << 24);
	++buf;
	ret |= ((std::uint64_t(*buf)) << 16);
	++buf;
	ret |= ((std::uint64_t(*buf)) << 8);
	++buf;
	ret |= std::uint64_t(*buf);

	return ret;
}



}
