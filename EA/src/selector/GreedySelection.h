/*
 * GreedySelection.h
 *
 *  Created on: Mar 18, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../EA/Type/Core.h"
#include "../rtoc/Constructible.h"
#include "ResizableSelector.h"

namespace ea {

class GreedySelection: public ResizableSelector {
public:
	EA_TYPEINFO_DEFAULT(GreedySelection)

	GreedySelection();
	virtual ~GreedySelection();

protected:
	virtual OrganismPoolPtr DoSelect(OrganismPoolPtr const& pPool,
			uint pTargetSize) override;
};

} /* namespace ea */


