/*
 * OperatorGroup.h
 *
 *  Created on: May 24, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../Common.h"
#include "../EA/Type/Core.h"
#include <type_traits>

namespace ea {

using namespace std;

/**
 * Wrapper for multiple operators to use as a placeholder in Strategy definition.
 * This wrapper provides numerous of useful functions for group management
 * such as Add(), Remove(), Find(), and Create() with auto-casting feature.
 * OperatorGroup is used in complement with
 * Strategy::ExecuteInParallel() when defining a custom Strategy.
 *
 * The class T of the group must be Operator compatible. For more details, see Operator.
 *
 * @tparam T An Operator compatible class.
 */
template <class T>
class OperatorGroup {
protected:
	/**
	 * The underlying list of operators.
	 */
	vector<Ptr<T>> mOps;

public:
	/**
	 * Create an empty operator group.
	 */
	OperatorGroup() : mOps() { }
	/**
	 * Create a group from an existed operators list.
	 * @param pOps A list of operators.
	 */
	OperatorGroup(const vector<Ptr<T>>& pOps) : mOps(pOps) { }
	virtual ~OperatorGroup() { }

	/**
	 * Get the number of operators in the group.
	 * @return The group size.
	 */
	uint GetSize() const {
		return mOps.size();
	}

	/**
	 * Get the operator at the given position.
	 * @param pAt The position of the operator in the group.
	 * @return The operator.
	 */
	const Ptr<T>& Get(uint pAt) const {
		return mOps[pAt];
	}
	/**
	 * Get the operator of type O at the given position.
	 * @tparam O The target type.
	 * @param pAt The position of the operator in the group.
	 * @return The operator.
	 */
	template <class O>
	Ptr<O> Get(uint pAt) const {
		Ptr<O> casted = dynamic_pointer_cast<O>(mOps[pAt]);
		if (!casted)
			throw EA_EXCEPTION(EAException, OPERATOR_BAD_CAST,
					"The wrapped operator cannot be casted into the given type.");
		return casted;
	}
	/**
	 * Find an operator of type O in the group.
	 * @tparam O Must be a child class of T.
	 * @return The operator if found, otherwise nullptr.
	 */
	template <class O>
	Ptr<O> Find() const {
		static_assert(is_base_of<T, O>::value,
				"OperatorGroup<T>::Find<O>() : O must be a child of T.");

		for (auto op : mOps) {
			Ptr<O> casted = dynamic_pointer_cast<O>(op);
			if (casted)
				return casted;
		}
		return nullptr;
	}

	/**
	 * Add an operator to the group.
	 * @param pOp The operator to be added.
	 */
	void Add(const Ptr<T>& pOp) {
		mOps.push_back(pOp);
	}
	/**
	 * Add an operator of type O to the group.
	 * @tparam O Must be a child class of T (auto-deduced).
	 * @param pOp The operator to be added.
	 */
	template <class O>
	void Add(const Ptr<O>& pOp) {
		static_assert(is_base_of<T, O>::value,
				"OperatorGroup<T>::Add<O>() : O must be a child of T.");

		Add(static_pointer_cast<T>(pOp));
	}

	/**
	 * Remove the operator at the given position.
	 * @param pAt The position of the operator in the group.
	 * @return The removed operator.
	 */
	Ptr<T> Remove(uint pAt) {
		Ptr<T> target = mOps[pAt];
		mOps.erase(mOps.begin() + pAt);
		return target;
	}
	/**
	 * Remove the given operator from the group.
	 * @param pOp The operator to be removed
	 */
	void Remove(const Ptr<T>& pOp) {
		mOps.erase(std::remove(mOps.begin(), mOps.end(), pOp));
	}
	/**
	 * Remove **an** operator of type O in the group.
	 * @tparam O Must be a child class of T.
	 * @return The removed operator if found, otherwise nullptr.
	 */
	template <class O>
	Ptr<O> Remove() {
		static_assert(is_base_of<T, O>::value,
				"OperatorGroup<T>::Remove<O>() : O must be a child of T.");

		Ptr<O> target = Find<O>();
		if (target)
			Remove(static_pointer_cast<T>(target));
		return target;
	}

	/**
	 * Create an operator by calling its constructor with given arguments and add it to the group.
	 * @tparam O Must be a child class of T.
	 * @tparam Args The type of the arguments (auto-deduced).
	 * @param args The arguments to be forwarded to the constructor of class O.
	 * @return The newly created operator of class O.
	 */
	template <class O, class... Args>
	Ptr<O> Create(Args&&... args) {
		static_assert(is_base_of<T, O>::value,
				"OperatorGroup<T>::Create<O>() : O must be a child of T.");

		Ptr<O> op = make_shared<O>(forward<Args>(args)...);
		Add(op);
		return op;
	}

	/**
	 * Create an object of class O and feed it in the contructor of class T.
	 * The object of class O will be created in the same way as in the function Create().
	 * However, instead of adding that object into the group by casting, it will be used as the default argument
	 * for the constructor of class T, and the newly created operator of class T will be added to the group.
	 * @tparam O Default argument type of the constructor of class T.
	 * @tparam Args The type of the arguments for the constructor of class O (auto-deduced).
	 * @param args The arguments to be forwarded to the constructor of class O.
	 * @return The newly created operator of class T.
	 */
	template <class O, class... Args>
	Ptr<T> CreateBase(Args&&... args) {
		Ptr<O> obj = make_shared<O>(forward<Args>(args)...);
		Ptr<T> op = make_shared<T>(obj);
		Add(op);
		return op;
	}

	/**
	 * Helper struct to test whether @tt{T_::OutputType} is void or not.
	 * This struct is used in the std::enable_if clause of InParallel() functions
	 * to enable the correct overload.
	 * @tparam T_ The type to be tested (must be indirect test to avoid hard errors)
	 */
	template<class T_>
	struct void_op {
		/**
		 * Whether @tt{T_::OutputType} is void.
		 */
		constexpr static bool value = is_void<typename T_::OutputType>::value;
	};

	/**
	 * The return type of InParallel().
	 * If @tt{T::OutputType} is not void, ReturnType will be a vector of @tt{T::OutputType}.
	 * Otherwise, ReturnType will be void.
	 */
	using ReturnType = typename conditional<void_op<T>::value, void,
			vector<typename T::OutputType>>::type;

	/**
	 * Execute the operator group by calling them in parallel (non-void return type version).
	 * This function is only available if @tt{T::OutputType} is not void.
	 * In that case, this function will return a vector of @tt{T::OutputType}
	 * containing the output of each operator in the group.
	 * @tparam Args The type of the arguments (auto-deduced).
	 * @param session The running Session which will be given when calling Strategy::ExecuteInParallel().
	 * @param args The arguments to be forwarded to @tt{T::operator()}.
	 * @return A list of output from @tt{T::operator()}.
	 */
	template <class T_ = T, class... Args>
	enable_if_t<!void_op<T_>::value, ReturnType>
	InParallel(const SessionPtr& session, Args&&... args) const {
		ReturnType results;
		for (auto op : mOps)
			results.push_back((*op)(session, forward<Args>(args)...));
		return results;
	}

	/**
	 * Execute the operator group by calling them in parallel (void return type version).
	 * This function is only available if @tt{T::OutputType} is void.
	 * In that case, this function will also return void.
	 * @tparam Args The type of the arguments (auto-deduced).
	 * @param session The running Session which will be given when calling Strategy::ExecuteInParallel().
	 * @param args The arguments to be forwarded to @tt{T::operator()}.
	 */
	template <class T_ = T, class... Args>
	enable_if_t<void_op<T_>::value, void>
	InParallel(const SessionPtr& session, Args&&... args) const {
		for (auto op : mOps)
			(*op)(session, forward<Args>(args)...);
	}
};

}



