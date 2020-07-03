#pragma once

#include <functional>
#include <map>
#include <algorithm>
#include <memory>

#include "debug.hpp"

namespace utki{

template<typename T_A, typename T_B> std::pair<T_B, T_A> flip_pair(const std::pair<T_A, T_B> &p){
    return std::pair<T_B, T_A>(p.second, p.first);
}

template<typename A, typename B> std::map<B, A> flip_map(const std::map<A, B> &m){
    std::map<B, A> ret;
    std::transform(m.begin(), m.end(), std::inserter(ret, ret.begin()), flip_pair<A, B>);
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
 *     utki::scope_exit some_scope_exit([](){
 *         std::cout << "Leaving scope" << std::endl;
 *     });
 * }
 * @endcode
 */
class scope_exit{
	std::function<void()> f;

public:
	scope_exit(const scope_exit&) = delete;
	scope_exit& operator=(const scope_exit&) = delete;
	
	/**
	 * @brief Constructor.
	 * @param f - function to call on object destruction.
	 */
	scope_exit(decltype(f)&& f) :
			f(std::move(f))
	{}
	
    ~scope_exit()noexcept{
		if(this->f){
			this->f();
		}
	}
	
	/**
	 * @brief Disarm scope exit object.
	 * This function disarms the scope_exit object, so that it will not do any action on destruction.
	 * @return the previous function which had to be executed on object's destruction.
	 */
	decltype(f) reset()noexcept{
		auto ret = std::move(this->f);
		this->f = nullptr;
		return ret;
	}
};

/**
 * @brief serialize 16 bit value, little-endian.
 * Serialize 16 bit value, less significant byte first.
 * @param value - the value.
 * @param out_buf - pointer to the 2 byte buffer where the result will be placed.
 * @return pointer to the next byte after serialized value.
 */
inline std::uint8_t* serialize16le(std::uint16_t value, std::uint8_t* out_buf)noexcept{
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
inline std::uint8_t* serialize32le(std::uint32_t value, std::uint8_t* out_buf)noexcept{
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
inline std::uint8_t* serialize64le(std::uint64_t value, std::uint8_t* out_buf)noexcept{
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
inline std::uint16_t deserialize16le(const std::uint8_t* buf)noexcept{
	std::uint16_t ret;

	// assume little-endian
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
inline std::uint32_t deserialize32le(const std::uint8_t* buf)noexcept{
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
inline std::uint64_t deserialize64le(const std::uint8_t* buf)noexcept{
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
inline std::uint8_t* serialize16be(std::uint16_t value, std::uint8_t* out_buf)noexcept{
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
inline std::uint8_t* serialize32be(std::uint32_t value, std::uint8_t* out_buf)noexcept{
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
inline std::uint8_t* serialize64be(std::uint64_t value, std::uint8_t* out_buf)noexcept{
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
inline std::uint16_t deserialize16be(const std::uint8_t* buf)noexcept{
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
inline std::uint32_t deserialize32be(const std::uint8_t* buf)noexcept{
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
inline std::uint64_t deserialize64be(const std::uint8_t* buf)noexcept{
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

/**
 * @brief Construct an object and store it in unique pointer.
 * This is a replacement of std::make_unique() when C++14 is not available.
 * @param args - object constructor arguments.
 * @return std::unique_ptr to a newly created object.
 */
template< class T, class... Args > std::unique_ptr<T> make_unique(Args&&... args){
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}
