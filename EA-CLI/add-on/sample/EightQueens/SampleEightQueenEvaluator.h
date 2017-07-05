/*
 * EightQueenEvaluator.h
 *
 *  Created on: April 12, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include <EA.h>

namespace ea {
namespace addon {

class SampleEightQueenEvaluator: public TypedScalarEvaluator<PermutationGenome> {
private:
	bool mOldVersion;

public:
	inline SampleEightQueenEvaluator(bool oldVersion = false) :
			mOldVersion(oldVersion) {
	}
	inline ~SampleEightQueenEvaluator() {
	}
	virtual inline bool IsMaximizer() override {
		return false;
	}

	EA_TYPEINFO_CUSTOM_INLINE {
		return *ea::TypeInfo("SampleEightQueenEvaluator")
			.SetConstructor<SampleEightQueenEvaluator, bool>("old-version");
	}
	EA_TYPEINFO_GET

private:
	virtual double DoScalarEvaluate(InputType pGenome) override;
};

REGISTER_ADDON(SampleEightQueenEvaluator)

}
}

