/*
 * PermutationInitializer.h
 *
 *  Created on: Apr 5, 2017
 *      Author: Kiet Vo
 */

#pragma once

#include "../EA/Type/Permutation.h"
#include "../core/interface/Initializer.h"

namespace ea {

class PermutationInitializer: public Initializer {
public:
	EA_TYPEINFO_CUSTOM_DECL

	PermutationInitializer	(uint pLength);

	virtual ~PermutationInitializer();

private:
	uint mLength;

protected:
	virtual GenomePoolPtr DoInitialize(uint pSize) override;
};

}
/* namespace ea */

