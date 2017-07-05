/*
 * Recombinator.cpp
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../../pch.h"
#include "Recombinator.h"
#include "../../Common.h"
#include "../pool/GenomePool.h"

namespace ea {

/**
 * @class Recombinator
 * The interface for recombination operator.
 * This class provides the interface to implement custom recombination operator.
 * Users should override DoCombine() in child class to implement their customized code.
 *
 * Recombinator is also the generalized version of Mutator and Initializer for arbitrary number
 * of parents. Users can use any Mutator or Initializer as a Recombinator and get the same effect.
 *
 * Recombinator is Operator compatible via Variator interface.
 * It will produce an output GenomePool k-times smaller then the input GenomePool
 * with k is the number of parents for each offspring.
 * Although Recombinator is compatible with Operator, if some parent selection method is used
 * priorly, users should consider MetaRecombinator.
 */

/**
 * @fn GenomePtr Recombinator::DoCombine(vector<GenomePtr>& pParents)
 * Implementation of the recombination operator.
 * The function requires a vector of Genome, representing the parents’ genomes that need to be recombined.
 * The length of this vector must be equal to the return value of the GetParentCount()
 * function. GetParentCount() must always return the same value. Users don’t need to check
 * the size of the input anymore, it has been done by the proxy caller Combine().
 *
 * Genomes in OpenEA library are passed by reference, therefore if users want to modify an existing Genome,
 * it should be cloned instead of modified directly. Otherwise, any Genome which shares the same reference
 * will be modified which, in most case, is not the wanted behavior.
 * In order to clone a Genome, the method GenomeClonable::Clone() or Genome::CloneBase() can be used.
 *
 * @param pParents The list of parents which need to be recombined.
 * @return A completely new Genome object which is the combined result of the input parents.
 */

/**
 * @fn uint Recombinator::GetParentCount()
 * The number of parents required by this Recombinator.
 * This function needs to be overriden in child class.
 * This function must always return the same value. This function is used as reference
 * to check the size of the given input vector for DoCombine().
 * @return The number of parents required.
 */

/**
 * Public method of DoCombine().
 * This function handles the exception and validity of the input, then call DoCombine() internally.
 * @param pParents The list of parents which need to be recombined.
 * @return A completely new Genome object which is the combined result of the input parents.
 */
GenomePtr Recombinator::Combine(vector<GenomePtr>& pParents) {
	if (pParents.size() != this->GetParentCount())
		throw EA_EXCEPTION(EAException, PROXY_RECOMBINATOR_FAILED,
				"Parents count is not consistent.");

	try {
		return DoCombine(pParents);
	} catch (exception& e) {
		throw EA_EXCEPTION(EAException, RECOMBINATOR_ERROR,
				"Exception caught when executing Recombinator.", current_exception());
	}
}

/**
 * Specialized implementation to be compatible with Variator.
 * This implementation will combine k-parents of the input pool into an offspring.
 * Therefore the output pool size is k times smaller than the input pool.
 * The number k is identified by the GetParentCount() function.
 * If the input pool is not a multiple of k, an error will be thrown.
 *
 * GetParentCount() must be greater than 0 to be compatible with Variator.
 * Otherwise, an error will be thrown.
 *
 * @param pPool The input GenomePool to be varied.
 * @return A new altered GenomePool.
 * @see Variator::DoVariate()
 */
GenomePoolPtr Recombinator::DoVariate(const GenomePoolPtr& pPool) {
	uint k = GetParentCount();
	if (k == 0)
		throw EA_EXCEPTION(EAException, PROXY_RECOMBINATOR_FAILED,
				"Initializer or Recombinator with 0 parents are not compatible with Variator.");

	if (pPool->size() % k != 0)
		throw EA_EXCEPTION(EAException, PROXY_RECOMBINATOR_FAILED,
				"The input pool size must be a multiple of GetParentCount()"
					"(pool size = " + to_string(pPool->size()) +
					", parent count = " + to_string(k) + ")");

	GenomePoolPtr inputPool = pPool, outputPool = make_shared<GenomePool>(inputPool->size() / k);
	auto begin = inputPool->begin();

	MultiThreading::For(0, inputPool->size() / k, [&] (int i) {
		vector<GenomePtr> subPool(begin + i*k, begin + (i+1)*k);
		(*outputPool)[i] = Combine(subPool);
	});

	return outputPool;
}

} /* namespace ea */
