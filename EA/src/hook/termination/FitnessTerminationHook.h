/*
 * TerminationHook.h
 *
 *  Created on: Apr 3, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../EA/Type/Core.h"
#include "../../core/interface/Hook.h"
#include "../../rtoc/Serializer.h"
#include "../../rtoc/Constructible.h"

namespace ea {

class FitnessTerminationHook: public Hook {
public:
	enum Condition {
		EQUAL_TO = 1,			///< Terminate if the current fitness is **equal to** the given one. EAML: \tt{"=="}.
		LESS_THAN,				///< Terminate if the current fitness is **less than** the given one. EAML: \tt{"<"}.
		LESS_THAN_OR_EQUAL_TO,	///< Terminate if the current fitness is **less than or equal to** the given one. EAML: \tt{"<="}.
		GREATER_THAN,			///< Terminate if the current fitness is **greater than** the given one. EAML: \tt{">"}.
		GREATER_OR_EQUAL_TO		///< Terminate if the current fitness is **greater than or equal to** the given one. EAML: \tt{">="}.
	};

	EA_TYPEINFO_CUSTOM_DECL

	FitnessTerminationHook(Condition pCondition, double pThreshold,
			bool pResort = true);
	virtual ~FitnessTerminationHook();
private:
	Condition mCondition;
	double mThreshold;
	bool mResort;

protected:
	virtual void DoGenerational() override;
};

} /* namespace ea */


