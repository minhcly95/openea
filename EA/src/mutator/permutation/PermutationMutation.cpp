/*
 * PermutationMutation.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: Kiet Vo
 */

#include "../../pch.h"
#include "PermutationMutation.h"
#include "../../genome/PermutationGenome.h"

namespace ea {

/**
 * @class PermutationMutation
 * Base class provides functionality for Mutator used on PermutationGenome.
 * Most Mutator working on PermutationGenome requires the input genome length must be greater than
 * 1 and a random interval. This class implements that so code won't be repeated in child classes.
 * Child class needs to override DoMutate() instead of DoApply().
 *
 * For real mutation operator implementation, see InsertMutation, InversionMutation, ScrambleMutation
 * and SwapMutation.
 */

PermutationMutation::~PermutationMutation() {
}

/**
 * @fn void PermutationMutation::DoMutate(vector<uint>& genes, uint lowerIndex, uint upperIndex)
 * Implementation of the mutation operator on PermutationGenome.
 * This is the new implementation point of the mutation operator. This function will extract the array
 * of genes and draw a random integer interval, so child classes won't need to repeat the code.
 *
 * @param genes The array of genes as reference.
 * @param lowerIndex The lower bound of the interval.
 * @param upperIndex The upper bound of the interval.
 */

/**
 * Specialized implementation to be compatible with TypedMutator.
 * This function will extract the array of genes, draw a random interval and call DoMutate() internally.
 * This function also provides an informative error message if the input genome is too short.
 * @see TypedMutator::DoApply(const Ptr<T>&)
 */
PermutationGenomePtr PermutationMutation::DoApply(const PermutationGenomePtr& pTarget) {
	auto genome = pTarget->Clone();
	vector<uint>& genes = genome->GetGenes();

	uint genomeLength = genes.size();
	if (genomeLength < 2)
		throw invalid_argument("PermutationMutation requires genome length greater than 1.");

	// swap interval
	auto interval = Random::OrderedPair<uint>(0, genomeLength - 1);

	// mutate
	DoMutate(genes, interval.first, interval.second);

	return genome;
}

} /* namespace ea */
