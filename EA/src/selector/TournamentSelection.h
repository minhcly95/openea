/*
 * TournamentSelection.h
 *
 *  Created on: Mar 29, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../EA/Type/Core.h"
#include "../rtoc/Constructible.h"
#include "ResizableSelector.h"

namespace ea {

class TournamentSelection: public ResizableSelector {
public:
	EA_TYPEINFO_CUSTOM_DECL

	TournamentSelection(uint pSize, double pProb = 1);
	virtual ~TournamentSelection();

	uint GetSize() const;
	double GetProbability() const;

private:
	uint mSize;
	double mProb;

protected:
	virtual OrganismPoolPtr DoSelect(OrganismPoolPtr const& pPool,
			uint pTargetSize) override;
};

} /* namespace ea */


