#pragma once

#include <memory>

#include "config.hpp"
#include "debug.hpp"

namespace utki{

template< class T, class... Args > std::shared_ptr<T> makeShared(Args&&... args);


/**
 * @brief Base class for objects managed by std::shared_ptr.
 */
class Shared : public std::enable_shared_from_this<Shared>{
	template< class T, class... Args > friend std::shared_ptr<T> utki::makeShared(Args&&...);
	
	static void* operator new(size_t size){
		return ::operator new(size);
	}
	
protected:
	/**
	 * @brief Get shared pointer from this.
     * @param thisPtr - 'this' used to determine type for casting.
     * @return shared_ptr to this object.
     */
	template <class T> std::shared_ptr<T> sharedFromThis(T* thisPtr){
		return std::move(std::dynamic_pointer_cast<T>(this->shared_from_this()));
	}
	
	template <class T> std::shared_ptr<const T> sharedFromThis(T* thisPtr)const{
		return std::move(std::dynamic_pointer_cast<const T>(this->shared_from_this()));
	}
public:
	virtual ~Shared()noexcept{}
};



/**
 * @brief Construct new Shared objects.
 * @param args - arguments of object class constructor.
 * @return std::shared_ptr pointing to a newly created object.
 */
//TODO: this is deprecated in favor of std::make_shared, remove this function in version 2.0.0
template< class T, class... Args > std::shared_ptr<T> makeShared(Args&&... args){
	static_assert(std::is_base_of<Shared, T>::value, "Class does not inherit utki::Shared, cannot create object.");
	TRACE(<< "utki::makeShared() is deprecated, use std::make_shared() instead" << std::endl)
	return std::make_shared<T>(std::forward<Args>(args)...);
}



/**
 * @brief Helper function to make weak_ptr out of shared_ptr.
 * @param ptr - shared_ptr out of which to make weak_ptr.
 * @return std::weak_ptr created from given std::shared_ptr.
 */
template <class T> std::weak_ptr<T> makeWeak(const std::shared_ptr<T>& ptr){
	return std::weak_ptr<T>(ptr);
}



}
