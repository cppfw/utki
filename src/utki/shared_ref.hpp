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
#include "util.hpp"

namespace utki {

/**
 * @brief Reference counting pointer which cannot be null.
 * The shared_ref is same as std::shared_ptr except that it is never null.
 * Objects have to be created with make_shared_ref() in order to be managed by shared_ref.
 * The shared_ref is implemented as a wrapper around std::shared_ptr.
 *
 * The sahred_ref doesn't have a move constructor, because otherwise it would contradict to the
 * whole idea of the non-null pointer, since moved from pointer would become null then.
 * Because of this the recommended way to pass shared_ref as an argument to a function is
 * via the constant reference, like:
 *
 * @code {.cpp}
 * void func(const shared_ref<some_class>& str);
 * @endcode
 *
 * this way the copying of the shared_ref will be deferred to the very last moment when it is actually needed.
 *
 * @tparam T - type pointed by the pointer.
 */
template <class T>
class shared_ref
{
	template <typename TT, typename... A>
	friend shared_ref<TT> make_shared_ref(A&&... args);

	template <typename dst_type, typename src_type>
	friend shared_ref<dst_type> dynamic_reference_cast(const shared_ref<src_type>& r);

	template <class tt>
	friend utki::shared_ref<tt> make_shared_from(tt&);

	std::shared_ptr<T> p;

	explicit shared_ref(std::shared_ptr<T>&& ptr) :
		p(std::move(ptr))
	{
		ASSERT(this->p)
	}

public:
	// there should be no default constructor, as shared_ref cannot be nullptr
	shared_ref() = delete;

	// moved-from shared_ref cannot remain nullptr, so there should be no move constructor
	shared_ref(shared_ref&&) = delete;

	shared_ref(const shared_ref&) = default;
	shared_ref& operator=(const shared_ref&) = default;

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
	{
		ASSERT(this->p)
	}

	/**
	 * @brief Get underlying std::shared_ptr.
	 *
	 * @return Underlying std::shared_ptr.
	 */
	const std::shared_ptr<T>& to_shared_ptr() const noexcept
	{
		return this->p;
	}

	/**
	 * @brief Convert to std::shared_ptr.
	 * This is an equivalent of to_shared_ptr().
	 *
	 * @return Underlying std::shared_ptr.
	 */
	operator const std::shared_ptr<T>&() const noexcept
	{
		return this->p;
	}

	/**
	 * @brief Convert to std::shared_ptr of convertible type.
	 *
	 * @tparam TT - pointed type to convert to.
	 * @return std::shared_ptr<TT> pointing to the converted type.
	 */
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

	/**
	 * @brief Convert to std::weak_ptr.
	 * This operator converts the underlying std::shared_ptr to std::weak_ptr,
	 * possibly pointing to a convertible type.
	 *
	 * @tparam TT - pointed type to convert to.
	 * @return std::weak_ptr<TT> pointing to the converted type.
	 */
	template < //
		typename TT,
		typename std::enable_if_t< //
			std::is_convertible_v< //
				T*,
				TT*>,
			bool> = true>
	operator std::weak_ptr<TT>() const noexcept
	{
		return this->p;
	}

	/**
	 * @brief Get reference to the pointed type.
	 *
	 * @return Const reference to the pointed type.
	 */
	T& get() const noexcept
	{
		return *this->p;
	}

	/**
	 * @brief Operator arrow.
	 *
	 * @return Pointer to the pointed object. Never nullptr.
	 */
	T* operator->() const noexcept
	{
		return this->p.get();
	}

	/**
	 * @brief Dereference operator.
	 *
	 * @return Reference to the pointed object.
	 */
	T& operator*() const noexcept
	{
		return *this->p;
	}
};

/**
 * @brief Create instance of an object managed by shared_ref.
 *
 * @tparam T - object type.
 * @tparam A - pack of object's constructor argument types.
 * @param args - object's constructor arguments.
 * @return shared_ref<T> to the newly created object.
 */
template <typename T, typename... A>
shared_ref<T> make_shared_ref(A&&... args)
{
	return shared_ref<T>(std::make_shared<T>(std::forward<A>(args)...));
}

/**
 * @brief Perform dynamic cast of shared_ref.
 *
 * @tparam dst_type - cast target type.
 * @tparam src_type - source type.
 * @param r - shared_ref to cast.
 * @return shared_ref to cast target type.
 * @throw std::bad_cast in case src_type cannot be casted to dst_type.
 */
template <typename dst_type, typename src_type>
shared_ref<dst_type> dynamic_reference_cast(const shared_ref<src_type>& r)
{
	auto p = std::dynamic_pointer_cast<dst_type>(r.to_shared_ptr());
	if (!p) {
		// this cast will throw std::bad_cast for us
		const auto& res = dynamic_cast<dst_type&>(r.get());
		touch(res);
	}
	return shared_ref<dst_type>(std::move(p));
}

} // namespace utki
