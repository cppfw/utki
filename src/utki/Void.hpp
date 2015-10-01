/**
 * @author Ivan Gagis <igagis@gmail.com>
 */

#pragma once

#include "config.hpp"

namespace utki{


/**
 * @brief Void class.
 * Void class is to be used in the same situations when void* is used, but
 * when the object referred by the pointer should be destructable, i.e. its
 * virtual destructor will be called if Void* is deleted.
 */
class Void{
public:
	virtual ~Void()noexcept{}
};


}
