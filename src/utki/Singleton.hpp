/**
 * @file Singleton.hpp
 * @author Ivan Gagis <igagis@gmail.com>
 * @brief Singleton base class (template).
 */

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
 *	class MySingleton : public ting::IntrusiveSingleton<MySingleton, MySingleton>{
 *		friend class ting::IntrusiveSingleton<MySingleton, MySingleton>;
 *		static ting::IntrusiveSingleton<MySingleton, MySingleton>::T_Instance instance;
 * 
 *	public:
 *		void DoSomething(){
 *			//...
 *		}
 *  };
 * 
 *	//define the static variable somewhere in .cpp file.
 *  ting::IntrusiveSingleton<MySingleton, MySingleton>::T_Instance MySingleton::instance;
 *
 *	int main(int, char**){
 *		MySingleton mySingleton;
 *
 *		MySingleton::Inst().DoSomething();
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

	typedef std::unique_ptr<T> T_Instance;
	
private:

	//copying is not allowed
	IntrusiveSingleton(const IntrusiveSingleton&);
	IntrusiveSingleton& operator=(const IntrusiveSingleton&);
	
public:
	
	/**
	 * @brief tells if singleton object is created or not.
	 * Note, this function is not thread safe.
	 * @return true if object is created.
	 * @return false otherwise.
	 */
	inline static bool IsCreated(){
		return T_InstanceOwner::instance != 0;
	}

	/**
	 * @brief get singleton instance.
	 * @return reference to singleton object instance.
	 */
	static T& Inst(){
		ASSERT_INFO(IsCreated(), "IntrusiveSingleton::Inst(): Singleton object is not created")
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
 *	class MySingleton : public ting::Singleton<MySingleton>{
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
	
private:

	//copying is not allowed
	Singleton(const Singleton&);
	Singleton& operator=(const Singleton&);

private:
	
	static typename IntrusiveSingleton<T, Singleton<T> >::T_Instance instance;
};

template <class T> typename utki::IntrusiveSingleton<T, Singleton<T> >::T_Instance utki::Singleton<T>::instance;

}//~namespace ting
