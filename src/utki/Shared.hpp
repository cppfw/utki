/* The MIT License:

Copyright (c) 2014 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */

// Home page: http://ting.googlecode.com



/**
 * @author Ivan Gagis <igagis@gmail.com>
 */

#pragma once

#include <memory>

#include "util.hpp"

namespace ting{

template< class T, class... Args > std::shared_ptr<T> New(Args&&... args);


/**
 * @brief Base class for objects managed by std::shared_ptr.
 */
class Shared : public std::enable_shared_from_this<Shared>{
	template< class T, class... Args > friend std::shared_ptr<T> ting::New(Args&&...);
	
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
