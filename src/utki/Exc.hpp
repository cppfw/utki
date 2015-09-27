/**
 * @file Exc.hpp
 * @author Ivan Gagis <igagis@gmail.com>
 * @brief Basic Exception class.
 */

#pragma once

#include <string.h>
#include <string>
#include <exception>

#include "util.hpp"

namespace ting{

/**
 * @brief Basic exception class
 */
class Exc : public std::exception{
	std::string message;
public:
	/**
	 * @brief Constructor.
     * @param message - human friendly error description.
     */
	inline Exc(const std::string& message = std::string()) :
			message(message)
	{}

	virtual ~Exc()noexcept{}

	/**
	 * @brief Returns a pointer to exception message.
	 * @return a pointer to objects internal memory buffer holding
	 *         the exception message null-terminated string.
	 *         Note, that after the exception object is destroyed
	 *         the pointer returned by this method become invalid.
	 */
	const char *What()const noexcept{
		return this->what();
	}



private:
	//override from std::exception
	const char *what()const noexcept{
		return this->message.c_str();
	}
};

}//~namespace ting
