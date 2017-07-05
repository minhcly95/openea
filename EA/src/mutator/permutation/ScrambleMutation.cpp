/*
 * ScrambleMutation.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: Kiet Vo
 */

#include "../../pch.h"
#include "ScrambleMutation.h"

namespace ea {

/**
 * @class ScrambleMutation
 * Implementation of **Scramble mutation** operator for PermutationGenome.
 * This Mutator is only applicable on PermutationGenome.
 *
 * When apply, a random segment of genes will be shuffled.
 *
 * This operator guarantees that the segment length is always at least 2.
 * However, that doesn't mean the new genome will always be a different one because
 * there is still a chance that shuffling will return the same order.
 *
 * @method
 * Use std::shuffle to shuffle the segment.
 *
 * @time
 * O(N)
 *
 * @name{ScrambleMutation}
 */

ScrambleMutation::ScrambleMutation() {
}

ScrambleMutation::~ScrambleMutation() {
}

void ScrambleMutation::DoMutate(vector<uint>& genes, uint lowerIndex, uint upperIndex) {
	shuffle(genes.begin() + lowerIndex, genes.begin() + upperIndex + 1, Random::generator);
}

} /* namespace ea */
