#pragma once

#include <string>
#include <exception>

#include "config.hpp"

namespace utki{

/**
 * @brief Basic exception class
 */
class exception : public std::exception{
	std::string message;
public:
	/**
	 * @brief Constructor.
	 * @param message - human friendly error description.
	 */
	exception(const std::string& message = std::string()) :
			message(message)
	{}

	virtual ~exception()noexcept{}

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

/**
 * @brief Basic exception class for "object not found" errors.
 */
class not_found : public exception{
public:
	not_found(const std::string& message) :
			exception(message)
	{}
};

/**
 * @brief Basic exception class for "invalid state" errors.
 */
class invalid_state : public exception{
public:
	invalid_state(const std::string& message) :
			exception(message)
	{}
};

// TODO: deprecated, remove.
typedef invalid_state illegal_state;

}
