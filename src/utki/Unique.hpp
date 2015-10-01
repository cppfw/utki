#pragma once

#include <memory>

#include "config.hpp"

namespace utki{


template< class T, class... Args > std::unique_ptr<T> makeUnique(Args&&... args);

/**
 * @brief Unique.
 * Forces usage of makeUnique() function to create an object.
 */
class Unique{
	template< class T, class... Args > friend std::unique_ptr<T> utki::makeUnique(Args&&...);
private:
	static void* operator new(size_t size){
		return ::operator new(size);
	}
	
public:
	virtual ~Unique()noexcept{}
};

/**
 * @brief Construct new unique_ptr managed object.
 * @param args - arguments of object class constructor.
 * @return std::unique_ptr pointing to a newly created object.
 */
template< class T, class... Args > std::unique_ptr<T> makeUnique(Args&&... args){
	static_assert(std::is_base_of<Unique, T>::value, "Class does not inherit utki::Unique, cannot create object.");
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


}
