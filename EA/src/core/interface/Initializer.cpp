/*
 * Initializer.cpp
 *
 *  Created on: Mar 19, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../../pch.h"
#include "Initializer.h"
#include "../../misc/EAException.h"
#include "../pool/GenomePool.h"
#include "../Session.h"
#include "../Population.h"

namespace ea {

/**
 * @class Initializer
 * The interface for initialization method.
 * This class provides the interface to implement custom initialization method.
 * Users should override DoInitialize() in child class to implement their customized code.
 *
 * Initializer is a specialization of Recombinator when GetParentCount() returns 0. Therefore,
 * users can also use any Initializer as a Recombinator.
 *
 * Although Initializer is descendant of Variator, it is not compatible with Variator::Variate().
 * Initializer is compatible with Operator and OperatorGroup by defining its own invocation entry.
 * It takes the expected size and produces a GenomePool.
 *
 * @see DoInitialize()
 */

/**
 * @fn GenomePoolPtr Initializer::DoInitialize()
 * Implementation of the initialization method.
 * The method requires a pool size as the input parameter. The implementation of
 * this method should return a vector of Genome with the vector size equal to the input pool size.
 * Otherwise, the proxy caller Initialize() will throw an error for inconsistent implementation.
 *
 * @param pSize The expected size of the returned vector.
 * @return A vector of Genome with size equals to the expected size.
 * @see Recombinator::DoCombine()
 */

/**
 * @fn uint Initializer::GetParentCount()
 * Specialized implementation to be compatible with Recombinator.
 * This function will always return 0 because an Initializer doesn't require any parent to spawn offspring.
 * @return Always 0.
 */

/**
 * Specialized implementation to be compatible with Recombinator.
 * This function will call Initialize() internally.
 * @see Recombinator::DoCombine()
 */
GenomePtr Initializer::DoCombine(vector<GenomePtr>& pParents) {
	return (*Initialize(1))[0];
}

/**
 * Public method of DoInitialize().
 * This function handles the exception and validity of the input and output, then call DoInitialize() internally.
 * @param pSize The expected size of the returned vector.
 * @return A vector of Genome with size equals to the expected size.
 * @see DoInitialize()
 */
GenomePoolPtr Initializer::Initialize(uint pSize) {
	GenomePoolPtr result;
	try {
		result = DoInitialize(pSize);
	} catch (exception& e) {
		throw EA_EXCEPTION(EAException, INITIALIZER_ERROR,
				"Exception caught when executing Initializer.", current_exception());
	}
	if (result->size() != pSize)
		throw EA_EXCEPTION(EAException, PROXY_INITIALIZER_FAILED,
				"Initilizer implementation returned wrong pool size.");
	return result;
}

GenomePoolPtr Initializer::operator() (const SessionPtr& pSession, uint pSize) {
	SetSession(pSession);
	GenomePoolPtr result = Initialize(pSize);
	SetSession(nullptr);
	return result;
}

}	// namespace ea
