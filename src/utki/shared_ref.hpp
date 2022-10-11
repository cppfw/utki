/*
The MIT License (MIT)

Copyright (c) 2015-2022 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* ================ LICENSE END ================ */

#pragma once

#include <memory>
#include <stdexcept>

#include "debug.hpp"

namespace utki {

// TODO: doxygen
template <class T>
class shared_ref
{
	std::shared_ptr<T> p;

	explicit shared_ref(std::shared_ptr<T>&& ptr) :
		p(std::move(ptr))
	{}

public:
	shared_ref(const shared_ref&) = default;
	shared_ref& operator=(const shared_ref&) = default;

	shared_ref(shared_ref&&) = default;
	~shared_ref() = default;

	/**
	 * @brief Constructor for automatic conversion to convertible type.
	 * @param sr - shared_ref to convert.
	 */
	template < //
		typename TT,
		typename std::enable_if_t< //
			std::is_convertible_v< //
				TT*,
				T*>,
			bool> = true>
	shared_ref(const shared_ref<TT>& sr) noexcept :
		p(sr.to_shared_ptr())
	{}

	const std::shared_ptr<T>& to_shared_ptr() const noexcept
	{
		return this->p;
	}

	operator const std::shared_ptr<T>&() const noexcept
	{
		return this->p;
	}

	template < //
		typename TT,
		typename std::enable_if_t< //
			std::is_convertible_v< //
				T*,
				TT*>,
			bool> = true>
	operator std::shared_ptr<TT>() const noexcept
	{
		return this->p;
	}

	T& get() noexcept
	{
		ASSERT(this->p)
		return *this->p;
	}

	const T& get() const noexcept
	{
		ASSERT(this->p)
		return *this->p;
	}

	T* operator->() noexcept
	{
		return this->p.get();
	}

	const T* operator->() const noexcept
	{
		return this->p.get();
	}

	T& operator*() noexcept
	{
		ASSERT(this->p)
		return *this->p;
	}

	const T& operator*() const noexcept
	{
		ASSERT(this->p)
		return *this->p;
	}

	template <typename TT, typename... Args>
	friend shared_ref<TT> make_shared_ref(Args&&... args);
};

template <typename T, typename... Args>
shared_ref<T> make_shared_ref(Args&&... args)
{
	return shared_ref<T>(std::make_shared<T>(std::forward<Args>(args)...));
}

} // namespace utki
