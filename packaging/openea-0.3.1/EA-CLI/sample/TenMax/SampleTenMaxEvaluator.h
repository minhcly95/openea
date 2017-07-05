/*
 * TenMaxEvaluator.h
 *
 *  Created on: Mar 23, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include <openea/EA.h>

namespace ea {
namespace addon {

class SampleTenMaxEvaluator: public ScalarEvaluator {
public:
	EA_TYPEINFO_DEFAULT(SampleTenMaxEvaluator)

	inline SampleTenMaxEvaluator() {
	}
	inline ~SampleTenMaxEvaluator() {
	}

private:
	virtual double DoScalarEvaluate(const GenomePtr& pGenome) override;
};

REGISTER_ADDON(SampleTenMaxEvaluator)

}
}


