#pragma once

#include "debug.hpp"
#include "config.hpp"

#include <memory>

namespace utki{

/**
 * @brief Intrusive singleton base class.
 * This is a basic intrusive singleton template.
 * Template params: T - your singletone class type, T_InstanceOwner - class which owns the static 'instance' variable.
 * In most cases T_InstanceOwner is the same as T.
 * Usage as follows:
 * @code
 *	class my_singleton : public utki::intrusive_singleton<my_singleton, my_singleton>{
 *		friend class utki::intrusive_singleton<my_singleton, my_singleton>;
 *		static utki::intrusive_singleton<my_singleton, my_singleton>::T_Instance instance;
 * 
 *	public:
 *		void do_something(){
 *			//...
 *		}
 *  };
 * 
 *	//define the static variable somewhere in .cpp file.
 *  utki::intrusive_singleton<my_singleton, my_singleton>::T_Instance my_singleton::instance;
 *
 *	int main(int, char**){
 *		my_singleton my_singleton_instance;
 *
 *		my_singleton::inst().do_something();
 *	}
 * @endcode
 */
template <class T, class T_InstanceOwner = T> class intrusive_singleton{

protected: // use only as a base class
	intrusive_singleton(){
		if(T_InstanceOwner::instance){
			throw std::logic_error("singleton::singleton(): instance is already created");
		}

		T_InstanceOwner::instance.reset(static_cast<T*>(this));
	}

	typedef intrusive_singleton<T> T_Singleton;
	
	// use unique_ptr because it is automatically initialized to nullptr. Automatic object destruction is not used.
	typedef std::unique_ptr<T> T_Instance;
	
public:

	intrusive_singleton(const intrusive_singleton&) = delete;
	intrusive_singleton& operator=(const intrusive_singleton&) = delete;
	
public:
	/**
	 * @brief tells if singleton object is created or not.
	 * Note, this function is not thread safe.
	 * @return true if object is created.
	 * @return false otherwise.
	 */
	static bool is_created(){
		return T_InstanceOwner::instance != nullptr;
	}

	/**
	 * @brief get singleton instance.
	 * @return reference to singleton object instance.
	 */
	static T& inst(){
		ASSERT(is_created(), [](auto&o){o << "intrusive_singleton::inst(): singleton object is not created";})
		return *T_InstanceOwner::instance;
	}

	virtual ~intrusive_singleton()noexcept{
		ASSERT(T_InstanceOwner::instance.get() == static_cast<T*>(this))
		T_InstanceOwner::instance.release();
	}
};

// non-intrusive singleton is not supported in MSVC
#if M_COMPILER != M_COMPILER_MSVC

/**
 * @brief Singleton base class.
 * This is a basic non-intrusive singleton template.
 * Note, that Singleton inherits the IntrusiveSingleton class, so it inherits all
 * its static methods, the most important one is Inst().
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
template <class T> class singleton : public intrusive_singleton<T, singleton<T> >{
	friend class intrusive_singleton<T, singleton<T> >;
protected:
	singleton(){}
	
public:
	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;

private:
	
	static typename intrusive_singleton<T, singleton<T> >::T_Instance instance;
};

template <class T> typename utki::intrusive_singleton<T, singleton<T> >::T_Instance utki::singleton<T>::instance;

#else

template <class T> class singleton{
	static_assert(
			std::is_same<T, T&>::value, // always false
			"non-intrusive singleton is not supported under MSVC compiler, use utki::intrusive_singleton"
		);
};

#endif

}
