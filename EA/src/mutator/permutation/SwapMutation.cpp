/*
 * SwapMutation.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: Kiet Vo
 */

#include "../../pch.h"
#include "SwapMutation.h"

namespace ea {

/**
 * @class SwapMutation
 * Implementation of **Swap mutation** operator for PermutationGenome.
 * This Mutator is only applicable on PermutationGenome.
 *
 * When apply, two genes are chosen randomly and swapped.
 *
 * This operator guarantees that the two genes are different.
 * That means the new genome will always be a different one.
 *
 * @method
 * Use std::swap to swap two genes.
 *
 * @time
 * O(1)
 *
 * @name{SwapMutation}
 */

SwapMutation::SwapMutation() {
}

SwapMutation::~SwapMutation() {
}

void SwapMutation::DoMutate(vector<uint>& genes, uint lowerIndex, uint upperIndex) {
	swap(genes[lowerIndex], genes[upperIndex]);
}

} /* namespace ea */
