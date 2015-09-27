/**
 * @file math.hpp
 * @author Ivan Gagis <igagis@gmail.com>
 * @brief Math utilities.
 */


#pragma once

#include <cmath>

#include "util.hpp"


namespace utki{

/**
 * @brief Get sign of a number.
 * Template function which returns the sign of a number.
 * General implementation of this template is as easy as:
 * @code
 * template <typename T> inline T Sign(T n){
 *     return n > 0 ? (1) : (-1);
 * }
 * @endcode
 * @param n - number to get sign of.
 * @return -1 if the argument is negative.
 * @return 1 if the number is positive.
 */
template <typename T> inline T sign(T n)noexcept{
	return n < 0 ? T(-1) : T(1);
}



/**
 * @brief Get number Pi.
 * @return number Pi.
 */
template <typename T> inline T pi()noexcept{
	return T(3.14159265358979323846264338327950288);
}



/**
 * @brief Get 2 * Pi.
 * @return 2 * Pi.
 */
template <typename T> inline T twoPi()noexcept{
	return T(2) * pi<T>();
}



/**
 * @brief Get natural logarithm of 2, i.e. ln(2).
 * @return natural logarithm of 2.
 */
template <typename T> inline T logOf2()noexcept{
	return T(0.693147181);
}



//Power functions

/**
 * @brief Calculate x^2.
 * @param x - value.
 * @return x * x.
 */
template <typename T> inline T pow2(T x)noexcept{
	return x * x;
}

/**
 * @brief Calculate x^3.
 */
template <typename T> inline T pow3(T x)noexcept{
	return pow2(x) * x;
}

/**
 * @brief Calculate x^4.
 */
template <typename T> inline T pow4(T x)noexcept{
	return pow2(pow2(x));
}

/**
 * @brief Calculate x^5.
 */
template <typename T> inline T pow5(T x)noexcept{
	return pow4(x) * x;
}

/**
 * @brief Calculate x^6.
 */
template <typename T> inline T pow6(T x)noexcept{
	return pow2(pow3(x));
}



}
