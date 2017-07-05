/*
 * Mutator.h
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../../EA/Type/Core.h"
#include "Recombinator.h"

namespace ea {

using namespace std;

class Mutator: public Recombinator {
public:
	inline virtual ~Mutator() {
	}

	GenomePtr Apply(const GenomePtr& pTarget);
	inline virtual uint GetParentCount() override final {
		return 1;
	}

protected:
	virtual GenomePtr DoCombine(vector<GenomePtr>& pParents) override final;
	virtual GenomePtr DoApply(const GenomePtr& pTarget) = 0;
};

}

/* namespace ea */
