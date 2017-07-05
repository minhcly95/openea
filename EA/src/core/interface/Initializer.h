/*
 * Initializer.h
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

class Initializer: public Recombinator {
public:
	using InputType = void;
	using OutputType = GenomePoolPtr;

	inline virtual ~Initializer() {
	}

	GenomePoolPtr Initialize(uint pSize);
	inline virtual uint GetParentCount() override final {
		return 0;
	}

	GenomePoolPtr operator() (const SessionPtr& pSession, uint pSize);

protected:
	virtual GenomePtr DoCombine(vector<GenomePtr>& pParents) override final;
	virtual GenomePoolPtr DoInitialize(uint pSize) = 0;
};

} /* namespace ea */
