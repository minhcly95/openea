/*
 * InversionMutation.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: Kiet Vo
 */

#include "../../pch.h"
#include "InversionMutation.h"

namespace ea {

/**
 * @class InversionMutation
 * Implementation of **Inversion mutation** operator for PermutationGenome.
 * This Mutator is only applicable on PermutationGenome.
 *
 * When apply, a random segment of genes will be inverted.
 *
 * This operator guarantees that the segment length is always at least 2.
 * That means the new genome will always be a different one.
 *
 * @method
 * Use std::reverse to reverse the segment.
 *
 * @time
 * O(N)
 *
 * @name{InversionMutation}
 */

InversionMutation::InversionMutation() {
}

InversionMutation::~InversionMutation() {
}

void InversionMutation::DoMutate(vector<uint>& genes, uint lowerIndex, uint upperIndex) {
	reverse(genes.begin() + lowerIndex, genes.begin() + upperIndex + 1);
}

} /* namespace ea */
