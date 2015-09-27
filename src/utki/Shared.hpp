/**
 * @author Ivan Gagis <igagis@gmail.com>
 */

#pragma once

#include <memory>

#include "util.hpp"

namespace utki{

template< class T, class... Args > std::shared_ptr<T> New(Args&&... args);


/**
 * @brief Base class for objects managed by std::shared_ptr.
 */
class Shared : public std::enable_shared_from_this<Shared>{
	template< class T, class... Args > friend std::shared_ptr<T> utki::New(Args&&...);
	
	static void* operator new(size_t size){
		return ::operator new(size);
	}
	
protected:
	template <class T> std::shared_ptr<T> SharedFromThis(T* thisPtr)const{
		return std::move(std::dynamic_pointer_cast<T>(const_cast<Shared*>(this)->shared_from_this()));
	}
	
public:
	virtual ~Shared()noexcept{}
};



/**
 * @brief Function to construct new Shared objects.
 * @param args - arguments of object class constructor.
 * @return std::shared_ptr pointing to a newly created object.
 */
template< class T, class... Args > std::shared_ptr<T> New(Args&&... args){
	return std::move(std::shared_ptr<T>(new T(std::forward<Args>(args)...)));
}



}
