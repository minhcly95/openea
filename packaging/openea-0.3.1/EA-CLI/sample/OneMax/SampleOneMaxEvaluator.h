/*
 * OneMaxEvaluator.h
 *
 *  Created on: Mar 23, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include <openea/EA.h>

namespace ea {
namespace addon {

class SampleOneMaxEvaluator: public ScalarEvaluator {
public:
	EA_TYPEINFO_DEFAULT(SampleOneMaxEvaluator)

	inline SampleOneMaxEvaluator() {
	}
	inline ~SampleOneMaxEvaluator() {
	}

private:
	virtual double DoScalarEvaluate(const GenomePtr& pGenome) override;
};

REGISTER_ADDON(SampleOneMaxEvaluator)

}
}


