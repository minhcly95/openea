/*
 * H1BenchmarkEvaluator.h
 *
 *  Created on: Jun 08, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include <openea/EA.h>

namespace ea {
namespace addon {

class H1BenchmarkEvaluator: public TypedScalarEvaluator<DoubleArrayGenome> {
public:
	EA_TYPEINFO_DEFAULT(H1BenchmarkEvaluator)

	virtual inline bool IsMaximizer() override {
		return true;
	}

private:
	virtual double DoScalarEvaluate(const DoubleArrayGenomePtr& pGenome) override;
};
REGISTER_ADDON(H1BenchmarkEvaluator)

}
}


