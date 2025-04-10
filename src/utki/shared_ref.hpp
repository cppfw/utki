/*
The MIT License (MIT)

utki - Utility Kit for C++.

Copyright (c) 2015-2025 Ivan Gagis <igagis@gmail.com>

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

#include "config.hpp"
#include "debug.hpp"

namespace utki {

/**
 * @brief Reference counting pointer which cannot be null.
 * shared_ref is same as std::shared_ptr except that it practically is never null.
 * Objects have to be created with utki::make_shared() in order to be managed by shared_ref.
 * The shared_ref is implemented as a wrapper around std::shared_ptr.
 *
 * shared_ref mimics the API provided by std::reference_wrapper.
 *
 * shared_ref doesn't provide default constructor.
 *
 * For better code performance shared_ref provides move constructor and move-assignment operator
 * which will leave the moved-from shared_ref instance in invalid state.
 * This is considered okay. Because implicit moving occurs in temporary context where
 * it is not possible to dereference moved-from instance. In case of user explicitly using
 * std::move() it is possible to dereference moved-from instance, and for that it is
 * suggested to rely on lint tools (e.g. clang-tidy) to catch such misuses of shared_ref.
 *
 * @tparam object_type - type pointed by the pointer.
 */
template <class object_type>
class shared_ref
{
	template <typename other_object_type, typename... arguments_type>
	friend shared_ref<other_object_type> make_shared(arguments_type&&... args);

	template <typename dst_type, typename src_type>
	friend shared_ref<dst_type> dynamic_reference_cast(const shared_ref<src_type>& r);

	std::shared_ptr<object_type> p;

public:
	/**
	 * @brief Construct a new shared_ref from non-null std::shared_ptr.
	 * If DEBUG macro is defined, it will assert in case passed in pointer is null.
	 * @param ptr - std::shared_ptr pointer to initialize the shared_ref.
	 *              Must not be null, otherwise it causes undefined behaviour.
	 */
	explicit shared_ref(std::shared_ptr<object_type> ptr) :
		p(std::move(ptr))
	{
		ASSERT(this->p)
	}

public:
	// there should be no default constructor, as shared_ref cannot be nullptr
	shared_ref() = delete;

	shared_ref(const shared_ref&) = default;
	shared_ref& operator=(const shared_ref&) = default;

	shared_ref(shared_ref&& r) = default;
	shared_ref& operator=(shared_ref&& r) = default;

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
	 * @brief Automatic conversion to shared_ptr.
	 */
	operator const std::shared_ptr<object_type>&() const noexcept
	{
		return this->p;
	}

	/**
	 * @brief Automatic conversion to shared_ptr pointing to const object.
	 */
	template <typename enable_type = object_type>
	operator std::shared_ptr<std::enable_if_t<!std::is_const_v<enable_type>, const enable_type>>() const noexcept
	{
		return this->p;
	}

	/**
	 * @brief Get reference to the object.
	 *
	 * @return Reference to the object.
	 */
	constexpr object_type& get() const noexcept
	{
		ASSERT(this->p)
		return *this->p;
	}

	/**
	 * @brief Convert to the reference to the object.
	 * @return Reference to the object.
	 */
	constexpr operator object_type&() const noexcept
	{
		return this->get();
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
shared_ref<object_type> make_shared(arguments_type&&... args)
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
