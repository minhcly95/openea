/*
 * UniformSelection.cpp
 *
 *  Created on: Mar 18, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../pch.h"
#include "UniformSelection.h"
#include <forward_list>

#include "../core/pool/OrganismPool.h"

namespace ea {

using namespace std;

EA_TYPEINFO_CUSTOM_IMPL(UniformSelection) {
	return *ea::TypeInfo("UniformSelection")
		.Add("with-replacement", &UniformSelection::mWithReplacement)
		->SetConstructor<UniformSelection>();
}

/**
 * @class UniformSelection
 * Implementation of **Uniform selection** operator.
 * This Selector will choose Organism randomly without any preference.
 * The input Pool could be selected with or without replacement.
 *
 * If selecting with replacement, Organism might be repeated. This option will run very fast
 * and places no restriction on the input Pool size.
 *
 * If selecting without replacement, the input Pool size must be greater than or equal to the target size.
 * Organism will only appear once, and it would have a little overhead.
 *
 * @method
 * For select with replacement, simply pick randomly N times. For select without replacement,
 * there are 2 cases. First case is when the target size is smaller than half the input Pool size,
 * then an existence table will be maintained. Otherwise, the Pool will be shuffled and erase the tail.
 *
 * @time
 * O(N) in both options.
 *
 * @name{UniformSelection}
 *
 * @eaml
 * @attr{with-replacement,
 * bool - Optional - Whether selection process is with replacement or without replacement}
 * @endeaml
 */

/**
 * Create an UniformSelection operator.
 * @param pWithReplacement Whether selection process is with replacement or without replacement.
 */
UniformSelection::UniformSelection(bool pWithReplacement) :
		mWithReplacement(pWithReplacement) {
}

UniformSelection::~UniformSelection() {
}

OrganismPoolPtr UniformSelection::DoSelect(const OrganismPoolPtr& pPool, uint pTargetSize) {
	uniform_int_distribution<int> dist(0, pPool->size() - 1);
	OrganismPoolPtr newPool = make_shared<OrganismPool>();

	if (mWithReplacement) {
		for (uint i = 0; i < pTargetSize; i++) {
			int chosenPos = dist(Random::generator);
			newPool->push_back((*pPool)[chosenPos]);
		}
		return newPool;

	} else { // without replacement
		if (pPool->size() < pTargetSize)
			throw invalid_argument(
					"UniformSelection without replacement requires pool size larger than target size.");

		if (pTargetSize < pPool->size() / 2) {
			vector<bool> chose(pPool->size(), false);
			for (uint i = 0; i < pTargetSize; i++) {
				int chosenPos;
				do
					chosenPos = dist(Random::generator);
				while (chose[chosenPos]);
				newPool->push_back((*pPool)[chosenPos]);
				chose[chosenPos] = true;
			}
			return newPool;
		} else {
			newPool = make_shared<OrganismPool>(*pPool);
			newPool->Shuffle();
			newPool->erase(newPool->begin() + pTargetSize, newPool->end());
			return newPool;
		}
	}
}

bool UniformSelection::IsWithReplacement() const {
	return mWithReplacement;
}

} /* namespace ea */
