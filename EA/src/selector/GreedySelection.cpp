/*
 * GreedySelection.cpp
 *
 *  Created on: Mar 18, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../pch.h"
#include "GreedySelection.h"
#include "../core/Organism.h"
#include "../core/pool/OrganismPool.h"

namespace ea {

using namespace std;

/**
 * @class GreedySelection
 * Implementation of **Greedy selection** operator.
 * This Selector will only choose the best Organism deterministically.
 *
 * @note
 * It requires the input Pool size to be larger then or equal to the target pool size.
 *
 * @method
 * Sort the Pool then erase the tail.
 *
 * @time
 * O(NlogN) because of the sort.
 *
 * @name{GreedySelection}
 */

GreedySelection::GreedySelection() {
}

GreedySelection::~GreedySelection() {
}

OrganismPoolPtr GreedySelection::DoSelect(
		const OrganismPoolPtr& pPool, uint pTargetSize) {
	if (pPool->size() < pTargetSize)
		throw invalid_argument(
				"GreedySelection requires pool size larger than target size.");

	OrganismPoolPtr newPool = make_shared<OrganismPool>(*pPool);

	newPool->Sort();

	newPool->erase(newPool->begin() + pTargetSize, newPool->end());

	return newPool;
}

} /* namespace ea */
