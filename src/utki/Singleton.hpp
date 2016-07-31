#pragma once

#include "Exc.hpp"
#include "debug.hpp"

#include <memory>


namespace utki{



/**
 * @brief Intrusive singleton base class.
 * This is a basic intrusive singleton template.
 * Template params: T - your singletone class type, T_InstanceOwner - class which owns the static 'instance' variable.
 * In most cases T_InstanceOwner is the same as T.
 * Usage as follows:
 * @code
 *	class MySingleton : public utki::IntrusiveSingleton<MySingleton, MySingleton>{
 *		friend class utki::IntrusiveSingleton<MySingleton, MySingleton>;
 *		static utki::IntrusiveSingleton<MySingleton, MySingleton>::T_Instance instance;
 * 
 *	public:
 *		void doSomething(){
 *			//...
 *		}
 *  };
 * 
 *	//define the static variable somewhere in .cpp file.
 *  utki::IntrusiveSingleton<MySingleton, MySingleton>::T_Instance MySingleton::instance;
 *
 *	int main(int, char**){
 *		MySingleton mySingleton;
 *
 *		MySingleton::inst().doSomething();
 *	}
 * @endcode
 */
template <class T, class T_InstanceOwner = T> class IntrusiveSingleton{

protected://use only as a base class
	IntrusiveSingleton(){
		if(T_InstanceOwner::instance){
			throw utki::Exc("Singleton::Singleton(): instance is already created");
		}

		T_InstanceOwner::instance.reset(static_cast<T*>(this));
	}

	typedef IntrusiveSingleton<T> T_Singleton;
	
	//Use unique_ptr because it is automatically initialized to nullptr. Automatic object destruction is not used.
	typedef std::unique_ptr<T> T_Instance;
	
public:

	IntrusiveSingleton(const IntrusiveSingleton&) = delete;
	IntrusiveSingleton& operator=(const IntrusiveSingleton&) = delete;
	
public:
	
	/**
	 * @brief tells if singleton object is created or not.
	 * Note, this function is not thread safe.
	 * @return true if object is created.
	 * @return false otherwise.
	 */
	static bool isCreated(){
		return T_InstanceOwner::instance.operator bool();
	}

	/**
	 * @brief get singleton instance.
	 * @return reference to singleton object instance.
	 */
	static T& inst(){
		ASSERT_INFO(isCreated(), "IntrusiveSingleton::Inst(): Singleton object is not created")
		return *T_InstanceOwner::instance;
	}

	virtual ~IntrusiveSingleton()noexcept{
		ASSERT(T_InstanceOwner::instance.operator->() == static_cast<T*>(this))
		T_InstanceOwner::instance.release();
	}
};



/**
 * @brief Singleton base class.
 * This is a basic non-intrusive singleton template.
 * Note, that Singleton inherits the IntrusiveSingleton class, so it inherits all
 * its static methods, the most important one is Inst().
 * Usage as follows:
 * @code
 *	class MySingleton : public utki::Singleton<MySingleton>{
 *	public:
 *		void DoSomething(){
 *			//...
 *		}
 *  };
 *
 *	int main(int, char**){
 *		MySingleton mySingleton;
 *
 *		MySingleton::Inst().DoSomething();
 *	}
 * @endcode
 */
template <class T> class Singleton : public IntrusiveSingleton<T, Singleton<T> >{
	friend class IntrusiveSingleton<T, Singleton<T> >;
protected:
	Singleton(){}
	
public:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

private:
	
	static typename IntrusiveSingleton<T, Singleton<T> >::T_Instance instance;
};

template <class T> typename utki::IntrusiveSingleton<T, Singleton<T> >::T_Instance utki::Singleton<T>::instance;

}
