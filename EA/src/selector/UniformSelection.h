/*
 * UniformSelection.h
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

class UniformSelection: public ResizableSelector {
public:
	EA_TYPEINFO_CUSTOM_DECL

	UniformSelection(bool pWithReplacement = true);
	virtual ~UniformSelection();

	bool IsWithReplacement() const;

private:
	bool mWithReplacement;

protected:
	virtual OrganismPoolPtr DoSelect(OrganismPoolPtr const& pPool,
			uint pTargetSize) override;
};

} /* namespace ea */


