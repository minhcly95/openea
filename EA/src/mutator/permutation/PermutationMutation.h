/*
 * PermutationMutation.h
 *
 *  Created on: Apr 6, 2017
 *      Author: Kiet Vo
 */

#pragma once

#include "../../EA/Type/Permutation.h"
#include "../../genome/PermutationGenome.h"
#include "../TypedMutator.h"

namespace ea {

class PermutationMutation: public TypedMutator<PermutationGenome> {
protected:
	virtual PermutationGenomePtr DoApply(const PermutationGenomePtr& pTarget) override final;
	virtual void DoMutate(vector<uint>& genes, uint lowerIndex, uint upperIndex) = 0;	// for permutations only

public:
	virtual ~PermutationMutation();
};

} /* namespace ea */


