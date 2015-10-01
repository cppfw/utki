/**
 * @file Exc.hpp
 * @author Ivan Gagis <igagis@gmail.com>
 * @brief Basic Exception class.
 */

#pragma once

#include <string>
#include <exception>

#include "config.hpp"

namespace utki{

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
	Exc(const std::string& message = std::string()) :
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
	const char *what()const noexcept override{
		return this->message.c_str();
	}
};

}//~namespace
