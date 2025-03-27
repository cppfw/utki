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

#include "config.hpp"
#include "debug.hpp"
#include "shared_ref.hpp"

namespace utki {

/**
 * @brief Base class for objects managed by std::shared_ptr.
 * This class is to be used as virtual base class for cases when shared_from_this()
 * is needed in case of multiple inheritance.
 */
class shared : public std::enable_shared_from_this<shared>
{
	static void* operator new(size_t size)
	{
		return ::operator new(size);
	}

protected:
	shared() = default;

public:
	shared(const shared&) = default;
	shared& operator=(const shared&) = default;

	shared(shared&&) = default;
	shared& operator=(shared&&) = default;

	virtual ~shared() noexcept = default;
};

/**
 * @brief Helper function to make weak_ptr out of shared_ptr.
 * @param ptr - shared_ptr out of which to make weak_ptr.
 * @return std::weak_ptr created from given std::shared_ptr.
 */
template <class object_type>
std::weak_ptr<object_type> make_weak(const std::shared_ptr<object_type>& ptr)
{
	return std::weak_ptr<object_type>(ptr);
}

/**
 * @brief Helper function to make weak_ptr out of shared_ref.
 * @param ref - shared_ref out of which to make weak_ptr.
 * @return std::weak_ptr created from given utki::shared_ref.
 */
template <class object_type>
std::weak_ptr<object_type> make_weak(const utki::shared_ref<object_type>& ref)
{
	return std::weak_ptr<object_type>(ref.to_shared_ptr());
}

/**
 * @brief Helper function to make shared pointer from shared object.
 * This function only works for types which are derived from std::enable_shared_from_this
 * or from utki::shared.
 * This helper function essentially just calls shared_from_this() and then does a dynamic cast
 * to the template type if needed. Thus, one does not have to do the dynamic cast manually.
 * @return shared reference to the given shared object.
 */
template <class object_type>
utki::shared_ref<object_type> make_shared_from(object_type& o)
{
	static_assert(
		std::is_base_of_v<shared, object_type> ||
			std::is_base_of_v<std::enable_shared_from_this<object_type>, object_type>,
		"make_shared_from(): argument must be derived from utki::shared or std::enable_shared_from_this"
	);

	if constexpr (std::is_base_of_v<shared, object_type>) {
		return utki::shared_ref<object_type>(std::dynamic_pointer_cast<object_type>(o.shared_from_this()));
	} else if constexpr (std::is_base_of_v<std::enable_shared_from_this<object_type>, object_type>) {
		return utki::shared_ref<object_type>(o.shared_from_this());
	}
}

/**
 * @brief Helper function to make weak pointer from shared object.
 * This function only works for types which are derived from std::enable_shared_from_this
 * or from utki::shared.
 * This helper function essentially just calls shared_from_this() and then does a dynamic cast
 * to the template type if needed and then converts to weak pointer.
 * @return weak pointer to the given shared object.
 */
template <class object_type>
std::weak_ptr<object_type> make_weak_from(object_type& o)
{
	static_assert(
		std::is_base_of_v<shared, object_type> ||
			std::is_base_of_v<std::enable_shared_from_this<object_type>, object_type>,
		"make_weak_from(): argument must be derived from utki::shared or std::enable_shared_from_this"
	);

	if constexpr (std::is_base_of_v<shared, object_type>) {
		return utki::make_weak(utki::make_shared_from(o));
	} else if constexpr (std::is_base_of_v<std::enable_shared_from_this<object_type>, object_type>) {
		return o.weak_from_this();
	}
}

} // namespace utki
