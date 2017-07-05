/*
 * GenerationTerminationHook.h
 *
 *  Created on: Apr 4, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../EA/Type/Core.h"
#include "InformedTerminationHook.h"

namespace ea {

class GenerationTerminationHook: public InformedTerminationHook {
public:
	EA_TYPEINFO_CUSTOM_DECL

	GenerationTerminationHook(ullong pTarget = 0, bool pInform = true);
	virtual ~GenerationTerminationHook();

protected:
	virtual ullong GetValue() override;
};

} /* namespace ea */


