/*
 * ResizableSelector.cpp
 *
 *  Created on: May 18, 2017
 *      Author: Bui Quang Minh
 */

#include "ResizableSelector.h"

#include "../pch.h"
#include "../EA/Utility.h"

namespace ea {

/**
 * @class ResizableSelector
 * A Selector interface allowing resizable output pool.
 * Using this interface, the size of the output pool can be specified
 * and the underlying selection method will adapt to the given size.
 * For most selection method, this is the right interface to derive from.
 *
 * Instead of overriding DoSelect(OrganismPoolPtr const&) function, child classes
 * should override DoSelect(OrganismPoolPtr const&,uint) which also provides the second
 * argument which is the wanted output pool size.
 *
 * ResizableSelector is compatible with Operator and OperatorGroup.
 * It takes an OrganismPool and the target size as input and produces another OrganismPool.
 *
 * @see DoSelect(OrganismPoolPtr const&,uint)
 */

/**
 * Create a ResizableSelector with the given output pool size.
 * The output pool size can be adjusted later by using SetSize().
 * @param pTargetSize The expected output pool size.
 */
ResizableSelector::ResizableSelector(uint pTargetSize) :
		mTargetSize(pTargetSize) {
}

ResizableSelector::~ResizableSelector() {
}

/**
 * Alias of SetSize().
 * @param pTargetSize The expected output pool size.
 */
void ResizableSelector::Resize(uint pTargetSize) {
	SetSize(pTargetSize);
}

/**
 * Set the expected output pool size.
 * @param pTargetSize The expected output pool size.
 */
void ResizableSelector::SetSize(uint pTargetSize) {
	mTargetSize = pTargetSize;
}

/**
 * Get the expected output pool size.
 * @return The expected output pool size.
 */
uint ResizableSelector::GetSize() {
	return mTargetSize;
}

/**
 * Public method of DoSelect(OrganismPoolPtr const&,uint).
 * This is an extension of Selector::Select() which requires the expected output pool size
 * as the second argument. The functionality of this function is equivalent to calling
 * SetSize() before calling the old Selector::Select() function.
 * @param pPool The input OrganismPool to be filtered.
 * @param pTargetSize The expected output pool size.
 * @return A new filtered OrganismPool.
 */
OrganismPoolPtr ResizableSelector::Select(OrganismPoolPtr const& pPool, uint pTargetSize) {
	SetSize(pTargetSize);
	return Selector::Select(pPool);
}

/**
 * @fn OrganismPoolPtr ResizableSelector::DoSelect(const OrganismPoolPtr& pPool, uint pTargetSize)
 * Implementation of the selection operator with resizable output pool.
 * The function requires an OrganismPool as input and produces another OrganismPool as the output.
 * The implementation must ensure that the size of the output OrganismPool
 * must be the same as the expected size, otherwise exception will be thrown.
 *
 * The implementation should follow the instruction described in Selector::DoSelect().
 *
 * @param pPool The input OrganismPool to be filtered.
 * @param pTargetSize The expected output pool size.
 * @return A new filtered OrganismPool.
 */

/**
 * Specialized implementation to be compatible with Selector.
 * This function will forward the call to the new DoSelect(const OrganismPoolPtr&, uint) internally.
 * @param pPool The input OrganismPool to be filtered.
 * @return A new filtered OrganismPool.
 */
OrganismPoolPtr ResizableSelector::DoSelect(const OrganismPoolPtr& pPool) {
	OrganismPoolPtr result = this->DoSelect(pPool, mTargetSize);
	if (result->size() != mTargetSize)
		throw EA_EXCEPTION(EAException, PROXY_SELECTOR_FAILED,
				"Selector implementation returned wrong pool size (required "
						+ to_string(mTargetSize) + ", returned "
						+ to_string(result->size()) + ").");
	return result;
}

/**
 * Invocation entry for Operator and OperatorGroup.
 * This function will call Select() internally. The Session context is ignored.
 * @param session The current running Session (given automatically by Operator).
 * @param pPool The input OrganismPool to be filtered.
 * @param pTargetSize The expected output pool size.
 * @return A new filtered OrganismPool.
 */
OrganismPoolPtr ResizableSelector::operator ()(const SessionPtr& session,
		OrganismPoolPtr pPool, uint pTargetSize) {
	return Select(pPool, pTargetSize);
}

} /* namespace ea */
