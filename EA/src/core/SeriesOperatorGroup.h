/*
 * SeriesOperatorGroup.h
 *
 *  Created on: May 24, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../Common.h"
#include "../EA/Type/Core.h"
#include "OperatorGroup.h"

namespace ea {

using namespace std;

/**
 * An OperatorGroup which can execute operators in series.
 * This is an extension of OperatorGroup allowing execution of operators in series.
 * SeriesOperatorGroup is used in complement with both
 * Strategy::ExecuteInSeries() and Strategy::ExecuteInParallel() when defining a custom Strategy.
 *
 * The underlying operator class must have the input type same as output type
 * because the output of the previous operator will be fed to the next one as the inputs.
 * Which means also that the type of the second argument of @tt{T::operator()} must also
 * be @tt{T::InputType} (while the first one is always @ref SessionPtr).
 * @tparam T An Operator compatible class which must define a non-void @tt{T::InputType}
 * which is the same type of @tt{T::OutputType}.
 */
template <class T>
class SeriesOperatorGroup : public OperatorGroup<T> {
	static_assert(!is_void<typename T::InputType>::value,
			"InputType for SeriesOperatorGroup must be non-void.");
	static_assert(is_same<typename T::OutputType, typename T::InputType>::value,
			"OutputType for SeriesOperatorGroup must be the same as the InputType.");

public:
	using ProcessingType = typename T::InputType;

	/**
	 * Create an empty series operator group.
	 */
	SeriesOperatorGroup() : OperatorGroup<T>() { }
	/**
	 * Create a group from an existed series operators list.
	 * @param pOps A list of operators.
	 */
	SeriesOperatorGroup(const vector<Ptr<T>>& pOps) : OperatorGroup<T>(pOps) { }

	/**
	 * Execute the operator group by calling them in series.
	 * @tparam Args The type of the arguments (auto-deduced).
	 * @param session The running Session which will be given when calling Strategy::ExecuteInSeries().
	 * @param data The input data to be processed.
	 * @param args The arguments to be forwarded to @tt{T::operator()}.
	 * @return The output of the processing.
	 */
	template <class... Args>
	ProcessingType InSeries(const SessionPtr& session, ProcessingType data, Args&&... args) const {
		for (auto op : this->mOps)
			data = (*op)(session, data, forward<Args>(args)...);
		return data;
	}
};

}



