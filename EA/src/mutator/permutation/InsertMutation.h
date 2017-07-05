/*
 * InsertMutation.h
 *
 *  Created on: Apr 6, 2017
 *      Author: Kiet Vo
 */

#pragma once

#include "../../Type.h"
#include "PermutationMutation.h"

namespace ea {

class InsertMutation: public PermutationMutation {
protected:
	virtual void DoMutate(vector<uint>& genes, uint lowerIndex, uint upperIndex) override;

public:
	EA_TYPEINFO_DEFAULT(InsertMutation)

	InsertMutation();
	virtual ~InsertMutation();
};

} /* namespace ea */


