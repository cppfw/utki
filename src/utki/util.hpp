/**
 * @file utils.hpp
 * @author Ivan Gagis <igagis@gmail.com>
 * @brief Utility functions and classes.
 */

#pragma once

//TODO: is it needed?
//#if M_COMPILER == M_COMPILER_MSVC
//#	pragma warning(disable:4290)
//#endif

#include <vector>
#include <functional>

#include "debug.hpp"
#include "types.hpp"
#include "config.hpp"



namespace utki{


class ScopeExit{
	std::function<void()> f;

public:
	ScopeExit(const ScopeExit&) = delete;
	ScopeExit& operator=(const ScopeExit&) = delete;
	
	ScopeExit(decltype(f)&& f) :
			f(std::move(f))
	{}
	
    ~ScopeExit(){
		if(this->f){
			this->f();
		}
	}
};



/**
 * @brief Get top-clamped value.
 * @param v - value to get clamped for.
 * @param top - top to clamp to.
 * @return clamped value.
 */
template <class T> inline T ClampedTop(T v, const T top)noexcept{
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
 * ting::ClampTop(a, 40);
 * std::cout << a << std::endl;
 *
 * //Clamp to 27. Value of 'a' is changed to 27,
 * //since it is 30 which is greater than 27.
 * ting::ClampTop(a, 27);
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
template <class T> inline void ClampTop(T& v, const T top)noexcept{
	v = ClampedTop(v, top);
}



/**
 * @brief Get bottom-clamped value.
 * @param v - value to get clamped for.
 * @param bottom - bottom to clamp to.
 * @return clamped value.
 */
template <class T> inline T ClampedBottom(T v, const T bottom)noexcept{
	if(v < bottom){
		return bottom;
	}
	return v;
}




/**
 * @brief Clamp value bottom.
 * Usage is analogous to ting::ClampTop.
 * @param v - reference to the value which bottom is to be clamped.
 * @param bottom - value to clamp the bottom to.
 */
template <class T> inline void ClampBottom(T& v, const T bottom)noexcept{
	v = ClampedBottom(v, bottom);
}



/**
 * @brief Get range-clamped value.
 * @param v - value to get clamped for.
 * @param bottom - value to clamp the bottom to.
 * @param top - value to clamp the top to.
 * @return clamped value.
 */
template <class T> inline T ClampedRange(T v, const T bottom, const T top)noexcept{
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
template <class T> inline void ClampRange(T& v, const T bottom, const T top)noexcept{
	v = ClampedRange(v, bottom, top);
}



/**
 * @brief serialize 16 bit value, little-endian.
 * Serialize 16 bit value, less significant byte first.
 * @param value - the value.
 * @param out_buf - pointer to the 2 byte buffer where the result will be placed.
 */
inline void Serialize16LE(std::uint16_t value, std::uint8_t* out_buf)noexcept{
	out_buf[0] = value & 0xff;
	out_buf[1] = value >> 8;
}



/**
 * @brief serialize 32 bit value, little-endian.
 * Serialize 32 bit value, less significant byte first.
 * @param value - the value.
 * @param out_buf - pointer to the 4 byte buffer where the result will be placed.
 */
inline void Serialize32LE(std::uint32_t value, std::uint8_t* out_buf)noexcept{
	*out_buf = std::uint8_t(value & 0xff);
	++out_buf;
	*out_buf = std::uint8_t((value >> 8) & 0xff);
	++out_buf;
	*out_buf = std::uint8_t((value >> 16) & 0xff);
	++out_buf;
	*out_buf = std::uint8_t((value >> 24) & 0xff);
}



/**
 * @brief de-serialize 16 bit value, little-endian.
 * De-serialize 16 bit value from the sequence of bytes. Assume that less significant
 * byte goes first in the input byte sequence.
 * @param buf - pointer to buffer containing 2 bytes to convert from little-endian format.
 * @return 16 bit unsigned integer converted from little-endian byte order to native byte order.
 */
inline std::uint16_t Deserialize16LE(const std::uint8_t* buf)noexcept{
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
inline std::uint32_t Deserialize32LE(const std::uint8_t* buf)noexcept{
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
 * @brief serialize 16 bit value, big-endian.
 * Serialize 16 bit value, most significant byte first.
 * @param value - the value.
 * @param out_buf - pointer to the 2 byte buffer where the result will be placed.
 */
inline void Serialize16BE(std::uint16_t value, std::uint8_t* out_buf)noexcept{
	out_buf[0] = value >> 8;
	out_buf[1] = value & 0xff;
}



/**
 * @brief serialize 32 bit value, big-endian.
 * Serialize 32 bit value, most significant byte first.
 * @param value - the value.
 * @param out_buf - pointer to the 4 byte buffer where the result will be placed.
 */
inline void Serialize32BE(std::uint32_t value, std::uint8_t* out_buf)noexcept{
	*out_buf = std::uint8_t((value >> 24) & 0xff);
	++out_buf;
	*out_buf = std::uint8_t((value >> 16) & 0xff);
	++out_buf;
	*out_buf = std::uint8_t((value >> 8) & 0xff);
	++out_buf;
	*out_buf = std::uint8_t(value & 0xff);
}



/**
 * @brief de-serialize 16 bit value, big-endian.
 * De-serialize 16 bit value from the sequence of bytes. Assume that most significant
 * byte goes first in the input byte sequence.
 * @param buf - pointer to buffer containing 2 bytes to convert from big-endian format.
 * @return 16 bit unsigned integer converted from big-endian byte order to native byte order.
 */
inline std::uint16_t Deserialize16BE(const std::uint8_t* buf)noexcept{
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
inline std::uint32_t Deserialize32BE(const std::uint8_t* buf)noexcept{
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



template <typename T> struct remove_constptr{
	typedef typename std::remove_const<typename std::remove_pointer<T>::type>::type type;
};




}
