/*
 * InsertMutation.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: Kiet Vo
 */

#include "../../pch.h"
#include "InsertMutation.h"

namespace ea {

/**
 * @class InsertMutation
 * Implementation of **Insert mutation** operator for PermutationGenome.
 * This Mutator is only applicable on PermutationGenome.
 *
 * When apply, a random gene will be picked out and inserted into a random position.
 *
 * This operator guarantees that the new position is different than the old one.
 *
 * @method
 * Erase the chosen gene and then insert it to the genome again.
 *
 * @time
 * O(N)
 *
 * @name{InsertMutation}
 */

InsertMutation::InsertMutation() {
}

InsertMutation::~InsertMutation() {
}

void InsertMutation::DoMutate(vector<uint>& genes, uint lowerIndex, uint upperIndex) {
	if (Random::Bool())
		std::swap(lowerIndex, upperIndex);

	uint temp = genes[lowerIndex];
	genes.erase(genes.begin() + lowerIndex);
	genes.insert(genes.begin() + upperIndex, temp);
}

} /* namespace ea */
