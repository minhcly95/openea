/*
 * Selector.h
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../../EA/Type/Core.h"
#include "../../rtoc/Constructible.h"

namespace ea {

using namespace std;

class Selector: public Constructible {
public:
	using InputType = OrganismPoolPtr;
	using OutputType = OrganismPoolPtr;

	inline virtual ~Selector() {
	}

	OrganismPoolPtr Select(OrganismPoolPtr const& pPool);

	OrganismPoolPtr operator() (const SessionPtr& pSession, OrganismPoolPtr pPool);

protected:
	virtual OrganismPoolPtr DoSelect(OrganismPoolPtr const& pPool) = 0;
};

} /* namespace ea */
