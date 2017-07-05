/*
 * CycleCrossover.h
 *
 *  Created on: Apr 11, 2017
 *      Author: Kiet Vo
 */

#pragma once

#include "../../EA/Type/Permutation.h"
#include "../../genome/PermutationGenome.h"
#include "../TypedRecombinator.h"

namespace ea {

class CycleCrossover: public TypedRecombinator<PermutationGenome> {
public:
	EA_TYPEINFO_DEFAULT(CycleCrossover)

	CycleCrossover();
	virtual ~CycleCrossover();

	inline virtual uint GetParentCount() {
		return 2;
	}

private:
	virtual PermutationGenomePtr DoCombine(vector<PermutationGenomePtr>& pParents) override;
};

} /* namespace ea */


