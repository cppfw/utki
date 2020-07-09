#pragma once

#include <memory>

#include "config.hpp"
#include "debug.hpp"

namespace utki{

/**
 * @brief Base class for objects managed by std::shared_ptr.
 * This class is to be used as virtual base class for cases when shared_from_this()
 * is needed in case of multiple inheritance.
 */
class shared : public std::enable_shared_from_this<shared>{
	static void* operator new(size_t size){
		return ::operator new(size);
	}
	
public:
	virtual ~shared()noexcept{}
};

/**
 * @brief Helper function to make weak_ptr out of shared_ptr.
 * @param ptr - shared_ptr out of which to make weak_ptr.
 * @return std::weak_ptr created from given std::shared_ptr.
 */
template <class T> std::weak_ptr<T> make_weak(const std::shared_ptr<T>& ptr){
	return std::weak_ptr<T>(ptr);
}

/**
 * @brief Helper function to make shared pointer from shared object.
 * This function only works for objects which are derived from std::enable_shared_from_this.
 * This helper function essentially just calls shared_from_this() and then does a dynamic cast
 * to the template type. Thus, one does not have to do the dynamic cast manually.
 * @return shared pointer to the given shared object.
 */
template <class T> std::shared_ptr<T> make_shared_from_this(T& o){
	return std::dynamic_pointer_cast<T>(o.shared_from_this());
}

/**
 * @brief Helper function to make weak pointer from shared object.
 * This function only works for objects which are derived from std::enable_shared_from_this.
 * This helper function essentially just calls shared_from_this() and then does a dynamic cast
 * to the template type and then converts to weak pointer.
 * @return weak pointer to the given shared object.
 */
template <class T> std::weak_ptr<T> make_weak_from_this(T& o){
	return utki::make_weak(utki::make_shared_from_this(o));
}

}
