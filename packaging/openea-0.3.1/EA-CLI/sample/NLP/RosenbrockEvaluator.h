/*
 * RosenbrockEvaluator.h
 *
 *  Created on: Jun 07, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include <openea/EA.h>

namespace ea {
namespace addon {

class RosenbrockEvaluator: public TypedScalarEvaluator<DoubleArrayGenome> {
public:
	EA_TYPEINFO_DEFAULT(RosenbrockEvaluator)

	virtual inline bool IsMaximizer() override {
		return false;
	}

private:
	virtual double DoScalarEvaluate(const DoubleArrayGenomePtr& pGenome) override;
};
REGISTER_ADDON(RosenbrockEvaluator)

}
}


