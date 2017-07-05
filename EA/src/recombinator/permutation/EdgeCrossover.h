/*
 * EdgeCrossover.h
 *
 *  Created on: Apr 7, 2017
 *      Author: Kiet Vo
 */

#pragma once

#include "../../EA/Type/Permutation.h"
#include "../../genome/PermutationGenome.h"
#include "../TypedRecombinator.h"

namespace ea {

class EdgeCrossover: public TypedRecombinator<PermutationGenome> {
public:
	EA_TYPEINFO_DEFAULT(EdgeCrossover)

	EdgeCrossover();
	virtual ~EdgeCrossover();

	inline virtual uint GetParentCount() override {
		return 2;
	}

private:
	virtual PermutationGenomePtr DoCombine(vector<PermutationGenomePtr>& pParents) override;
};

} /* namespace ea */


