#pragma once

#include <memory>
#include <cassert>


// Singleton class to make children classes singleton.
template<typename T>
class Singleton {
private:
	// Single instance
	static std::unique_ptr<T> instance_;

protected:
	Singleton() {};

public:
	// Avoids making copies of a singleton
	Singleton<T>& operator=(const Singleton<T>& s) = delete;
	Singleton(const Singleton<T>& s) = delete;

	virtual ~Singleton() {};


	// Initializes the instance with the desired parameters (.. args)
	template<typename ...T_args>
	inline static T* init(T_args &&...args) {
		assert(instance_.get() == nullptr, "Instance already exists");
		instance_.reset(new T(std::forward<T_args>(args)...));
		
		return instance_.get();
	}

	// To close singletons in a specific order
	inline static void close() { instance_.reset(); }


	// To get the pointer of the instance
	inline static T* instance() {
		// If it doesn't exist, it gets created
		if (instance_.get() == nullptr) init();

		// Then, returns the pointer
		return instance_.get();
	}
};

template<typename T>
std::unique_ptr<T> Singleton<T>::instance_;


/*
* Use example:
*
*	class A : public Singleton<A> {
*		friend Singleton<A>;
*
*	private:
*		// Constructors must be private and the class must implement at least a
		// constructor with no args just in case init() is called with no args
*		A() { ... };
*		A(args1) { ... };
*		A(args2) { ...};
*
*		method1() { ... };
*		method2() { ... };
*		...
*
*	public:
*		// To avoid copying/moving the instance
*		A(A&) = delete;
*		A(A&&) = delete;
*		A& operator=(A&) = delete;
*		A& operator=(A&&) = delete;
* 
*		// Destructor can also be virtual
*		~A() { ... }
*
*		method3() { ... };
*		method4() { ... };
*		...
*	}
*
*
*
* The child class must be instanced as:
*	// args depend on A's constructor(s) args
*	A::init(args);

*/
