/*
 * PermutationGenome.h
 *
 *  Created on: Apr 5, 2017
 *      Author: Kiet Vo
 */

#pragma once

#include "../EA/Type/Core.h"
#include "ArrayGenome.h"

namespace ea {

class PermutationGenome: public GenomeClonable<PermutationGenome>, public ArrayGenomeBase<uint> {
public:
	EA_TYPEINFO_DEFAULT(PermutationGenome)

	PermutationGenome(vector<uint>& pGenes);
	PermutationGenome();
	virtual ~PermutationGenome();

	bool IsValid() const;

	virtual ostream& Print(ostream& os) const override;
};

} /* namespace ea */
