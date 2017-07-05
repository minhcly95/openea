/*
 * ResizableSelector.h
 *
 *  Created on: May 18, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../core/interface/Selector.h"

namespace ea {

class ResizableSelector : public Selector {
public:
	ResizableSelector(uint pTargetSize = 0);
	virtual ~ResizableSelector();

	void Resize(uint pTargetSize);

	void SetSize(uint pTargetSize);
	uint GetSize();

	using Selector::Select;
	OrganismPoolPtr Select(OrganismPoolPtr const& pPool, uint pTargetSize);

	OrganismPoolPtr operator() (const SessionPtr& session, OrganismPoolPtr pPool, uint pTargetSize);

protected:
	virtual OrganismPoolPtr DoSelect(OrganismPoolPtr const& pPool) override;
	virtual OrganismPoolPtr DoSelect(OrganismPoolPtr const& pPool, uint pTargetSize) = 0;

private:
	uint mTargetSize;
};

} /* namespace ea */
