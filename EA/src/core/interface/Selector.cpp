/*
 * Selector.cpp
 *
 *  Created on: Mar 21, 2017
 *      Author: Bui Quang Minh
 */

#include "../../pch.h"
#include "Selector.h"

#include "../pool/OrganismPool.h"
#include "../../Common.h"

using namespace std;

namespace ea {

/**
 * @class Selector
 * The interface for selection operator.
 * This class provides the interface to implement custom selection operator.
 * Users should override DoSelect() in child class to implement their customized code.
 *
 * The size of the output pool is not given. If users want to create a Selector which
 * can produce a OrganismPool with arbitrary size, see ResizableSelector.
 *
 * Selector is compatible with Operator and OperatorGroup.
 * It takes an OrganismPool as input and produces another OrganismPool.
 *
 * @see DoSelect()
 * @see ResizableSelector
 */

/**
 * @fn OrganismPoolPtr Selector::DoSelect(const OrganismPoolPtr& pPool)
 * Implementation of the selection operator.
 * The function requires an OrganismPool as input and produces another OrganismPool as the output.
 *
 * Pools in OpenEA library are passed by reference, therefore, users should create a new
 * OrganismPool and modified in the new Pool. Modification in the old Pool is still allowed but
 * may lead to unwanted behavior.
 *
 * The size of the output pool is not given. If users want to create a Selector which
 * can produce a OrganismPool with arbitrary size, see ResizableSelector.
 *
 * @param pPool The input OrganismPool to be filtered.
 * @return A new filtered OrganismPool.
 */

/**
 * The public method of DoSelect().
 * This function handles the exception thrown when calling DoSelect() internally.
 * @param pPool The input OrganismPool to be filtered.
 * @return A new filtered OrganismPool.
 */
OrganismPoolPtr Selector::Select(const OrganismPoolPtr& pPool) {
	OrganismPoolPtr result;
	try {
		result = this->DoSelect(pPool);
	} catch (exception& e) {
		throw EA_EXCEPTION(EAException, SELECTOR_ERROR,
				"Exception caught when executing Selector.", current_exception());
	}
	return result;
}

/**
 * Invocation entry for Operator and OperatorGroup.
 * This function will call Select() internally. The Session context is ignored.
 * @param pSession The current running Session (given automatically by Operator).
 * @param pPool The input OrganismPool to be filtered.
 * @return A new filtered OrganismPool.
 */
OrganismPoolPtr Selector::operator ()(const SessionPtr& pSession,
		OrganismPoolPtr pPool) {
	return Select(pPool);
}

}	// namespace ea

