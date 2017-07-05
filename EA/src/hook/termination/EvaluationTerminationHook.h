/*
 * EvaluationTerminationHook.h
 *
 *  Created on: Apr 4, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../EA/Type/Core.h"
#include "InformedTerminationHook.h"

namespace ea {

using namespace std;

class EvaluationTerminationHook: public InformedTerminationHook {
public:
	EA_TYPEINFO_CUSTOM_DECL

	EvaluationTerminationHook(ullong pTarget, bool pInform = true);
	virtual ~EvaluationTerminationHook();
protected:
	virtual ullong GetValue() override;
};

} /* namespace ea */


