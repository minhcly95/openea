/*
 * Recombinator.h
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../../EA/Type/Core.h"
#include "../../rtoc/Constructible.h"
#include "Variator.h"

namespace ea {

using namespace std;

class Recombinator: public Variator {
public:
	inline virtual ~Recombinator() {
	}

	GenomePtr Combine(vector<GenomePtr>& pParents);
	virtual uint GetParentCount() = 0;

protected:
	virtual GenomePtr DoCombine(vector<GenomePtr>& pParents) = 0;
	virtual GenomePoolPtr DoVariate(const GenomePoolPtr& pPool) override;
};

} /* namespace ea */
