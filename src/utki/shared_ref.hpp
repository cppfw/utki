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
 * @tparam object_type - type pointed by the pointer.
 */
template <class object_type>
class shared_ref
{
	template <typename other_object_type, typename... arguments_type>
	friend shared_ref<other_object_type> make_shared_ref(arguments_type&&... args);

	template <typename dst_type, typename src_type>
	friend shared_ref<dst_type> dynamic_reference_cast(const shared_ref<src_type>& r);

	template <class other_object_type>
	friend utki::shared_ref<other_object_type> make_shared_from(other_object_type&);

	std::shared_ptr<object_type> p;

	explicit shared_ref(std::shared_ptr<object_type>&& ptr) :
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
		typename other_object_type,
		typename std::enable_if_t< //
			std::is_convertible_v< //
				other_object_type*,
				object_type*>,
			bool> = true>
	shared_ref(const shared_ref<other_object_type>& sr) noexcept :
		p(sr.to_shared_ptr())
	{
		ASSERT(this->p)
	}

	/**
	 * @brief Get underlying std::shared_ptr.
	 *
	 * @return Underlying std::shared_ptr.
	 */
	const std::shared_ptr<object_type>& to_shared_ptr() const noexcept
	{
		return this->p;
	}

	/**
	 * @brief Convert to std::shared_ptr.
	 * This is an equivalent of to_shared_ptr().
	 *
	 * @return Underlying std::shared_ptr.
	 */
	operator const std::shared_ptr<object_type>&() const noexcept
	{
		return this->p;
	}

	/**
	 * @brief Convert to std::shared_ptr of convertible type.
	 *
	 * @tparam other_object_type - pointed type to convert to.
	 * @return std::shared_ptr<other_object_type> pointing to the converted type.
	 */
	template < //
		typename other_object_type,
		typename std::enable_if_t< //
			std::is_convertible_v< //
				object_type*,
				other_object_type*>,
			bool> = true>
	operator std::shared_ptr<other_object_type>() const noexcept
	{
		return this->p;
	}

	/**
	 * @brief Convert to std::weak_ptr.
	 * This operator converts the underlying std::shared_ptr to std::weak_ptr,
	 * possibly pointing to a convertible type.
	 *
	 * @tparam other_object_type - pointed type to convert to.
	 * @return std::weak_ptr<other_object_type> pointing to the converted type.
	 */
	template < //
		typename other_object_type,
		typename std::enable_if_t< //
			std::is_convertible_v< //
				object_type*,
				other_object_type*>,
			bool> = true>
	operator std::weak_ptr<other_object_type>() const noexcept
	{
		return this->p;
	}

	/**
	 * @brief Get reference to the pointed type.
	 *
	 * @return Const reference to the pointed type.
	 */
	object_type& get() const noexcept
	{
		return *this->p;
	}

	/**
	 * @brief Operator arrow.
	 *
	 * @return Pointer to the pointed object. Never nullptr.
	 */
	object_type* operator->() const noexcept
	{
		return this->p.get();
	}

	/**
	 * @brief Dereference operator.
	 *
	 * @return Reference to the pointed object.
	 */
	object_type& operator*() const noexcept
	{
		return *this->p;
	}
};

/**
 * @brief Create instance of an object managed by shared_ref.
 *
 * @tparam object_type - object type.
 * @tparam arguments_type - pack of object's constructor argument types.
 * @param args - object's constructor arguments.
 * @return shared_ref<object_type> to the newly created object.
 */
template <typename object_type, typename... arguments_type>
shared_ref<object_type> make_shared_ref(arguments_type&&... args)
{
	return shared_ref<object_type>(std::make_shared<object_type>(std::forward<arguments_type>(args)...));
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
		[[maybe_unused]] const auto& res = dynamic_cast<dst_type&>(r.get());
	}
	return shared_ref<dst_type>(std::move(p));
}

} // namespace utki
