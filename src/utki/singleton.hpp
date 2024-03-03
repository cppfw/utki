/*
The MIT License (MIT)

Copyright (c) 2015-2024 Ivan Gagis <igagis@gmail.com>

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

namespace utki {

/**
 * @brief Intrusive singleton base class.
 * This is a basic intrusive singleton template.
 * @tparam object_type - your singletone class type.
 * @tparam instance_owner_type - class which owns the static 'instance' variable.
 *                               In most cases instance_owner_type is the same as object_type.
 *
 * Usage as follows:
 * @code
 *	class my_singleton : public utki::intrusive_singleton<my_singleton, my_singleton>{
 *		friend class utki::intrusive_singleton<my_singleton, my_singleton>;
 *		static utki::intrusive_singleton<my_singleton, my_singleton>::instance_type instance;
 *
 *	public:
 *		void do_something(){
 *			//...
 *		}
 *  };
 *
 *	// define the static variable somewhere in .cpp file.
 *  utki::intrusive_singleton<my_singleton, my_singleton>::instance_type my_singleton::instance;
 *
 *	int main(int, char**){
 *		my_singleton my_singleton_instance;
 *
 *		my_singleton::inst().do_something();
 *	}
 * @endcode
 */
template <class object_type, class instance_owner_type = object_type>
class intrusive_singleton
{
protected: // use only as a base class
	intrusive_singleton()
	{
		if (instance_owner_type::instance) {
			throw std::logic_error("singleton::singleton(): instance is already created");
		}

		instance_owner_type::instance.reset(static_cast<object_type*>(this));
	}

	using singleton_type = intrusive_singleton<object_type>;

	// use unique_ptr because it is automatically initialized to nullptr. Automatic object destruction is not used.
	using instance_type = std::unique_ptr<object_type>;

public:
	intrusive_singleton(const intrusive_singleton&) = delete;
	intrusive_singleton& operator=(const intrusive_singleton&) = delete;

	intrusive_singleton(intrusive_singleton&&) = delete;
	intrusive_singleton& operator=(intrusive_singleton&&) = delete;

public:
	/**
	 * @brief tells if singleton object is created or not.
	 * Note, this function is not thread safe.
	 * @return true if object is created.
	 * @return false otherwise.
	 */
	static bool is_created()
	{
		return instance_owner_type::instance != nullptr;
	}

	/**
	 * @brief get singleton instance.
	 * @return reference to singleton object instance.
	 */
	static object_type& inst()
	{
		ASSERT(is_created(), [](auto& o) {
			o << "intrusive_singleton::inst(): singleton object is not created";
		})
		return *instance_owner_type::instance;
	}

	virtual ~intrusive_singleton() noexcept
	{
		ASSERT(instance_owner_type::instance.get() == static_cast<object_type*>(this))
		[[maybe_unused]] auto ptr = instance_owner_type::instance.release();
	}
};

// non-intrusive singleton is not supported in MSVC
#if CFG_COMPILER != CFG_COMPILER_MSVC

/**
 * @brief Singleton base class.
 * This is a basic non-intrusive singleton template.
 * Note, that singleton inherits the intrusive_singleton class, so it inherits all
 * its static methods, the most important one is inst().
 * Usage as follows:
 * @code
 *	class my_singleton : public utki::singleton<my_singleton>{
 *	public:
 *		void do_something(){
 *			//...
 *		}
 *  };
 *
 *	int main(int, char**){
 *		my_singleton my_singleton_instance;
 *
 *		my_singleton::inst().do_something();
 *	}
 * @endcode
 */
template <class object_type>
class singleton : public intrusive_singleton<object_type, singleton<object_type>>
{
	friend class intrusive_singleton<object_type, singleton<object_type>>;

protected:
	singleton() = default;

public:
	~singleton() override = default;

	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;

	singleton(singleton&&) = delete;
	singleton& operator=(singleton&&) = delete;

private:
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
	static typename intrusive_singleton<object_type, singleton<object_type>>::instance_type instance;
};

template <class object_type>
typename utki::intrusive_singleton<object_type, singleton<object_type>>::instance_type
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
	utki::singleton<object_type>::instance;

#else

template <class object_type>
class singleton
{
	static_assert(
		std::is_same<object_type, object_type&>::value, // always false
		"non-intrusive singleton is not supported under MSVC compiler, use utki::intrusive_singleton"
	);
};

#endif

} // namespace utki
