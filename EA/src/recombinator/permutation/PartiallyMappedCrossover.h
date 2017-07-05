/*
 * PartiallyMappedCrossover.h
 *
 *  Created on: Apr 6, 2017
 *      Author: Kiet Vo
 */

#pragma once

#include "../../EA/Type/Permutation.h"
#include "../../genome/PermutationGenome.h"
#include "../TypedRecombinator.h"

namespace ea {

class PartiallyMappedCrossover: public TypedRecombinator<PermutationGenome> {
public:
	EA_TYPEINFO_DEFAULT(PartiallyMappedCrossover)

	PartiallyMappedCrossover();
	virtual ~PartiallyMappedCrossover();

	inline virtual uint GetParentCount() {
		return 2;
	}

private:
	virtual PermutationGenomePtr DoCombine(vector<PermutationGenomePtr>& pParents) override;
};

} /* namespace ea */


