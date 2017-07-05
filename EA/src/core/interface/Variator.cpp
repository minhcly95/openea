/*
 * Variator.cpp
 *
 *  Created on: May 18, 2017
 *      Author: Bui Quang Minh
 */

#include "../../pch.h"
#include "Variator.h"
#include "../../Common.h"

#include "../pool/GenomePool.h"

namespace ea {

/**
 * @class Variator
 * The interface for variation operator.
 * This class provides the interface to implement custom selection operator.
 * Users should override DoVariate() in child class to implement their customized code.
 *
 * This interface works at the Pool level, which takes in a GenomePool and produces another GenomePool.
 * For the interfaces which work at the individual level, see Mutator or Recombinator,
 * and their Pool level wrappers: MetaMutator and MetaRecombinator.
 *
 * Variator is compatible with Operator, OperatorGroup and SeriesOperatorGroup.
 * It takes a GenomePool as input and produces another GenomePool.
 *
 * @see DoVariate()
 * @see Mutator
 * @see Recombinator
 * @see MetaMutator
 * @see MetaRecombinator
 */

/**
 * @fn GenomePoolPtr Variator::DoVariate(const GenomePoolPtr& pPool)
 * Implementation of the variation operator (need to be overridden in child class).
 * The function requires a GenomePool as input and produces another GenomePool as the output.
 *
 * Pools in OpenEA library are passed by reference, therefore, users should create a new
 * GenomePool and modified in the new Pool. Modification in the old Pool is still allowed but
 * may lead to unwanted behavior.
 *
 * @param pPool The input GenomePool to be varied.
 * @return A new altered GenomePool.
 */

Variator::~Variator() {
}

/**
 * The public method of DoVariate().
 * This function handles the exception thrown when calling DoVariate() internally.
 * This function cannot be called if implementation of DoVariate() uses GetSession().
 * @param pPool The input GenomePool to be varied.
 * @return A new altered GenomePool.
 */
GenomePoolPtr Variator::Variate(const GenomePoolPtr& pPool) {
	try {
		return this->DoVariate(pPool);
	} catch (exception& e) {
		throw EA_EXCEPTION(EAException, VARIATOR_ERROR,
				"Exception caught when executing Variator.", current_exception());
	}
}

/**
 * Invocation entry for Operator and OperatorGroup.
 * This function will call Variate() internally. The Session context is available via GetSession() function.
 * If GetSession() is used, the algorithm depends on Session and Variate() cannot be called.
 * @param pSession The current running Session (given automatically by Operator).
 * @param pGenomePool The input GenomePool to be varied.
 * @return A new altered GenomePool.
 */
GenomePoolPtr Variator::operator ()(const SessionPtr& pSession, const GenomePoolPtr& pGenomePool) {
	mSession = pSession;
	GenomePoolPtr result = Variate(pGenomePool);
	mSession = nullptr;
	return result;
}

/**
 * Get the Session running context.
 * This Session object is only available when Variator is invoked via an Operator of OperatorGroup.
 * Otherwise, this function throws an exception.
 * @return The current Session running context.
 */
const SessionPtr& Variator::GetSession() const {
	if (!mSession)
		throw EA_EXCEPTION(EAException, SESSION_DEPENDENT,
				"This Variator is Session-dependent. Variate() cannot be called.");
	return mSession;
}

/**
 * Set the Session running context.
 * Use this function if users want to invoke the Variate() method within a specific Session context.
 * It is only required if the implementation depends on Session (see GetSession()).
 * @param pSession The Session running context.
 */
void Variator::SetSession(const SessionPtr& pSession) {
	mSession = pSession;
}

} /* namespace ea */

