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

#include "debug.hpp"

namespace utki {

/**
 * @brief Unique pointer which cannot be null.
 * unqiue_ref is same as std::unique_ptr except that it practically is never null.
 * Objects have to be created with utki::make_unique() in order to be managed by unqiue_ref.
 * The unique_ref is implemented as a wrapper around std::unique_ptr.
 *
 * unique_ref mimics the API provided by std::reference_wrapper.
 *
 * unique_ref doesn't provide default constructor.
 *
 * unique_ref provides move constructor and move-assignment operator
 * which will leave the moved-from unique_ref instance in invalid state.
 * This is considered okay. Because implicit moving occurs in temporary context where
 * it is not possible to dereference moved-from instance. In case of user explicitly using
 * std::move() it is possible to dereference moved-from instance, and for that it is
 * suggested to rely on lint tools (e.g. clang-tidy) to catch such misuses of unique_ref (use after move).
 *
 * @tparam object_type - type pointed by the pointer.
 */
template <typename object_type>
class unique_ref
{
	template <typename other_object_type>
	friend class unique_ref;

	std::unique_ptr<object_type> p;

public:
	/**
	 * @brief Construct a new unique_ref from non-null std::unique_ptr.
	 * If DEBUG macro is defined, it will assert in case passed in pointer is null.
	 * @param ptr - std::unique_ptr pointer to initialize the unique_ref.
	 *              Must not be null, otherwise it causes undefined behaviour.
	 */
	explicit unique_ref(std::unique_ptr<object_type> ptr) :
		p(std::move(ptr)){ASSERT(this->p)}

		// there should be no default constructor, as unique_ref cannot be nullptr
		unique_ref() = delete;

	unique_ref(const unique_ref&) = delete;
	unique_ref& operator=(const unique_ref&) = delete;

	unique_ref(unique_ref&& r) = default;
	unique_ref& operator=(unique_ref&& r) = default;

	~unique_ref() = default;

	/**
	 * @brief Constructor for automatic conversion to convertible type.
	 * @param sr - unique_ref to convert.
	 */
	template < //
		typename other_object_type,
		typename std::enable_if_t< //
			std::is_convertible_v< //
				other_object_type*,
				object_type*>,
			bool> = true>
	unique_ref(unique_ref<other_object_type>&& sr) noexcept :
		p(std::move(sr.p))
	{
		ASSERT(this->p)
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
 * @brief Create instance of an object managed by unique_ref.
 *
 * @tparam object_type - object type.
 * @tparam arguments_type - pack of object's constructor argument types.
 * @param args - object's constructor arguments.
 * @return unique_ref<object_type> to the newly created object.
 */
template <typename object_type, typename... arguments_type>
unique_ref<object_type> make_unique(arguments_type&&... args)
{
	return unique_ref<object_type>(std::make_unique<object_type>(std::forward<arguments_type>(args)...));
}
} // namespace utki
