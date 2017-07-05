/*
 * Operator.h
 *
 *  Created on: May 24, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../Common.h"
#include "../EA/Type/Core.h"

namespace ea {

/**
 * Operator wrapper to use as a placeholder in Strategy definition.
 * This wrapper provides numerous of useful functions such as Get(), Set() and Create()
 * with auto-casting feature. Operator is used in complement with
 * Strategy::Execute() when defining a custom Strategy.
 *
 * To make a class T Operator-compatible, first it needs to define @tt{T::OutputType} which
 * is the return type of the execution. Then overload @tt{T::operator()} which returns the same type as above.
 * The type of the first argument of the overloaded @tt{T::operator()} must be a
 * @ref SessionPtr, which will be given when Strategy::Execute() is called.
 *
 * @tparam T An operator which must has a typedef of @tt{T::OutputType}
 * and overload the function @tt{T::operator()}.
 */
template <class T>
class Operator {
private:
	Ptr<T> mOp;

public:
	/**
	 * Create a wrapper to nullptr.
	 */
	Operator() : mOp(nullptr) { }
	/**
	 * Create a wrapper to the given operator.
	 * @param pOp The operator to be wrapped.
	 */
	Operator(const Ptr<T>& pOp) : mOp(pOp) { }

	/**
	 * Get the wrapped operator.
	 * @return The wrapped operator.
	 */
	const Ptr<T>& Get() const {
		return mOp;
	}
	/**
	 * Get the wrapped operator and cast to type O.
	 * @tparam O The target type.
	 * @return The wrapped operator in type O.
	 */
	template <class O>
	Ptr<O> Get() const {
		Ptr<O> casted = dynamic_pointer_cast<O>(mOp);
		if (!casted)
			throw EA_EXCEPTION(EAException, OPERATOR_BAD_CAST,
					"The wrapped operator cannot be casted into the given type.");
		return casted;
	}
	/**
	 * Set the operator of the wrapper.
	 * @param pOp The operator to be wrapped.
	 */
	void Set(const Ptr<T>& pOp) {
		mOp = pOp;
	}
	/**
	 * Set the operator of the wrapper with an object of type O.
	 * @tparam O Must be a child class of T (auto-deduced).
	 * @param pOp The operator to be wrapped.
	 */
	template <class O>
	void Set(const Ptr<O>& pOp) {
		static_assert(is_base_of<T, O>::value,
				"Operator<T>::Set<O>() : O must be a child of T.");

		mOp = static_pointer_cast<T>(pOp);
	}

	/**
	 * Check whether the underlying operator exists or not.
	 * @return true if this wrapper has actually wrapped an operator.
	 */
	operator bool() const {
		return bool(mOp);
	}

	/**
	 * Same as Get().
	 * @return The wrapped operator.
	 */
	operator Ptr<T>() const {
		return Get();
	}
	/**
	 * Same as Set<O>().
	 * @tparam O Must be a child class of T.
	 * @param pOp The operator to be wrapped.
	 */
	template <class O>
	void operator =(const Ptr<O>& pOp) {
		static_assert(is_base_of<T, O>::value,
				"Operator<T>::operator= <O>() : O must be a child of T.");

		Set(pOp);
	}

	/**
	 * Create and set the underlying operator by calling its constructor with given arguments.
	 * @tparam O Must be a child class of T.
	 * @tparam Args The type of the arguments (auto-deduced).
	 * @param args The arguments to be forwarded to the constructor of class O.
	 * @return The newly created operator of class O.
	 */
	template <class O, class... Args>
	Ptr<O> Create(Args&&... args) {
		static_assert(is_base_of<T, O>::value,
				"Operator<T>::Create<O>() : O must be a child of T.");

		Ptr<O> op = make_shared<O>(forward<Args>(args)...);
		Set(op);
		return op;
	}

	/**
	 * Create an object of class O and feed it in the contructor of class T.
	 * The object of class O will be created in the same way as in the function Create().
	 * However, instead of setting that object to the wrapper by casting, it will be used as the default argument
	 * for the constructor of class T, and the newly created operator of class T will be set to the wrapper.
	 * @tparam O Default argument type of the constructor of class T.
	 * @tparam Args The type of the arguments for the constructor of class O (auto-deduced).
	 * @param args The arguments to be forwarded to the constructor of class O.
	 * @return The newly created operator of class T.
	 */
	template <class O, class... Args>
	Ptr<T> CreateBase(Args&&... args) {
		Ptr<O> obj = make_shared<O>(forward<Args>(args)...);
		Ptr<T> op = make_shared<T>(obj);
		Set(op);
		return op;
	}

	/**
	 * Execute the wrapped operator by calling its @tt{operator()} function.
	 * @tparam Args The type of the arguments (auto-deduced).
	 * @param session The running Session which will be given when calling Strategy::Execute().
	 * @param args The arguments to be forwarded to @tt{T::operator()}.
	 * @return The return of @tt{T::operator()}.
	 */
	template <class... Args>
	typename T::OutputType operator ()(const SessionPtr& session, Args&&... args) const {
		return (*mOp)(session, forward<Args>(args)...);
	}
};

}
