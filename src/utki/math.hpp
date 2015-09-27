/**
 * @file math.hpp
 * @author Ivan Gagis <igagis@gmail.com>
 * @brief Math utilities.
 */


#pragma once

#include <cmath>

#include "util.hpp"


namespace ting{
namespace math{

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
template <typename T> inline T Sign(T n)noexcept{
	return n < 0 ? T(-1) : T(1);
}



/**
 * @brief Get absolute value of a number.
 * General implementation of this function is as follows:
 * @code
 * template <typename T> inline T Abs(T n){
 *     return n > 0 ? n : (-n);
 * }
 * @endcode
 * @param n - number to get absolute value of.
 * @return absolute value of the passed argument.
 */
template <typename T> inline T Abs(T n)noexcept{
	return n < 0 ? (-n) : (n);
}



/**
 * @brief Round number to closest integer which is bigger or equal to the value.
 * @param n - value to round.
 * @return Rounded value.
 */
template <typename T> inline T Ceil(T n)noexcept{
	return n.Ceil();
}



template <> inline float Ceil<float>(float n)noexcept{
	return ::ceil(n);
}



template <> inline double Ceil<double>(double n)noexcept{
	return ::ceil(n);
}



/**
 * @brief Round number to closest integer which is less or equal to the value.
 * @param n - value to round.
 * @return Rounded value.
 */
template <typename T> inline T Floor(T n)noexcept{
	return n.Floor();
}



template <> inline float Floor<float>(float n)noexcept{
	return ::floor(n);
}



template <> inline double Floor<double>(double n)noexcept{
	return ::floor(n);
}



/**
 * @brief Round number to closest integer.
 * @param n - value to round.
 * @return Rounded value.
 */
template <typename T> inline T Round(T n)noexcept{
	T f = Floor<T>(n);
	if(Abs<T>(n - f) < T(0.5)){
		return f;
	}else{
		return f + T(1);
	}
}



/**
 * @brief Get number Pi.
 * @return number Pi.
 */
template <typename T> inline T DPi()noexcept{
	return T(3.14159265358979323846264338327950288);
}



/**
 * @brief Get 2 * Pi.
 * @return 2 * Pi.
 */
template <typename T> inline T D2Pi()noexcept{
	return T(2) * DPi<T>();
}



/**
 * @brief Get natural logarithm of 2, i.e. ln(2).
 * @return natural logarithm of 2.
 */
template <typename T> inline T DLnOf2()noexcept{
	return T(0.693147181);
}



//Power functions

/**
 * @brief Calculate x^2.
 * @param x - value.
 * @return x * x.
 */
template <typename T> inline T Pow2(T x)noexcept{
	return x * x;
}

/**
 * @brief Calculate x^3.
 */
template <typename T> inline T Pow3(T x)noexcept{
	return Pow2(x) * x;
}

/**
 * @brief Calculate x^4.
 */
template <typename T> inline T Pow4(T x)noexcept{
	return Pow2(Pow2(x));
}

/**
 * @brief Calculate x^5.
 */
template <typename T> inline T Pow5(T x)noexcept{
	return Pow4(x) * x;
}

/**
 * @brief Calculate x^6.
 */
template <typename T> inline T Pow6(T x)noexcept{
	return Pow2(Pow3(x));
}



/**
 * @brief Calculates x^p.
 * @param x - value
 * @param p - power
 * @return x^p
 */
template <typename T> inline T Pow(T x, T p)noexcept{
	return x.Pow(p);
}

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline float Pow<float>(float x, float p)noexcept{
	return ::pow(x, p);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline double Pow<double>(double x, double p)noexcept{
	return ::pow(x, p);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline long double Pow<long double>(long double x, long double p)noexcept{
	return ::pow(x, p);
}
#endif



/**
 * @brief Calculate sine of an angle.
 */
template <typename T> inline T Sin(T x)noexcept{
	return x.Sin();
}

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline float Sin<float>(float x)noexcept{
	return ::sin(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline double Sin<double>(double x)noexcept{
	return ::sin(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline long double Sin<long double>(long double x)noexcept{
	return ::sin(x);
}
#endif



/**
 * @brief Calculate cosine of an angle.
 */
template <typename T> inline T Cos(T x)noexcept{
	return x.Cos();
}

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline float Cos<float>(float x)noexcept{
	return ::cos(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline double Cos<double>(double x)noexcept{
	return ::cos(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline long double Cos<long double>(long double x)noexcept{
	return ::cos(x);
}
#endif



/**
 * @brief Calculate tangent of a number.
 */
template <typename T> inline T Tan(T x)noexcept{
	return x.Tan();
}

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline float Tan<float>(float x)noexcept{
	return ::tan(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline double Tan<double>(double x)noexcept{
	return ::tan(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline long double Tan<long double>(long double x)noexcept{
	return ::tan(x);
}
#endif



/**
 * @brief Calculate arcsine of a number.
 */
template <typename T> inline T Asin(T x)noexcept{
	return x.Asin();
}

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline float Asin<float>(float x)noexcept{
	return ::asin(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline double Asin<double>(double x)noexcept{
	return ::asin(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline long double Asin<long double>(long double x)noexcept{
	return ::asin(x);
}
#endif



/**
 * @brief Calculate arccosine of a number.
 */
template <typename T> inline T Acos(T x)noexcept{
	return x.Acos();
}

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline float Acos<float>(float x)noexcept{
	return ::acos(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline double Acos<double>(double x)noexcept{
	return ::acos(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline long double Acos<long double>(long double x)noexcept{
	return ::acos(x);
}
#endif



/**
 * @brief Calculate arctangent of a number.
 */
template <typename T> inline T Atan(T x)noexcept{
	return x.Atan();
}

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline float Atan<float>(float x)noexcept{
	return ::atan(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline double Atan<double>(double x)noexcept{
	return ::atan(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline long double Atan<long double>(long double x)noexcept{
	return ::atan(x);
}
#endif



/**
 * @brief Calculate square root of a number.
 */
template <typename T> inline T Sqrt(T x)noexcept{
	return x.Sqrt();
}

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline float Sqrt<float>(float x)noexcept{
	return ::sqrt(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline double Sqrt<double>(double x)noexcept{
	return ::sqrt(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline long double Sqrt<long double>(long double x)noexcept{
	return ::sqrt(x);
}
#endif



/**
 * @brief Calculate e^x.
 */
template <typename T> inline T Exp(T x)noexcept{
	return x.Exp();
}

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline float Exp<float>(float x)noexcept{
	return ::exp(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline double Exp<double>(double x)noexcept{
	return ::exp(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline long double Exp<long double>(long double x)noexcept{
	return ::exp(x);
}
#endif



/**
 * @brief Calculate ln(x).
 * Calculate natural logarithm of x.
 */
template <typename T> inline T Ln(T x)noexcept{
	return x.Ln();
}

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline float Ln<float>(float x)noexcept{
	return ::log(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline double Ln<double>(double x)noexcept{
	return ::log(x);
}
#endif

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
template <> inline long double Ln<long double>(long double x)noexcept{
	return ::log(x);
}
#endif



/**
 * @brief Calculate cubic root of a number.
 */
template <typename T> inline T CubicRoot(T x)noexcept{
	if(x > 0){
		return Exp<T>(Ln<T>(x) / 3);
	}
	
	if(x == 0){
		return 0;
	}
	
	return -Exp<T>(Ln<T>(-x) / 3);
}



/**
 * @brief Calculate argument of a complex number.
 * Any complex number
 *     C = x + i * y
 * can be represented in the form
 *     C = |C| * exp(i * arg)
 * where 'arg' is the argument of a complex number.
 * @param x - real part of a complex number.
 * @param y - imaginary part of a complex number.
 * @return argument of a complex number.
 */
template <typename T> inline T Arg(T x, T y)noexcept{
	T a;
	if(x == 0){
		a = DPi<T>() / 2;
	}else if(x > 0){
		a = T(Atan(Abs(y / x)));
	}else{
		a = DPi<T>() - T(Atan(Abs(y / x)));
	}

	if(y >= 0){
		return a;
	}else{
		return -a;
	}
}


}//~namespace math
}//~namespace ting
