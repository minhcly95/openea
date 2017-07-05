/*
 * ScalarEvaluator.h
 *
 *  Created on: Apr 25, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../EA/Type/Core.h"
#include "IndividualEvaluator.h"

namespace ea {

class ScalarEvaluator: public IndividualEvaluator {
public:
	using FunctionType = function<double(const GenomePtr&)>;

	virtual ~ScalarEvaluator();

	virtual inline bool IsMaximizer() {
		return true;
	}

protected:
	virtual double DoScalarEvaluate(const GenomePtr& pGenome) = 0;
	virtual FitnessPtr DoEvaluate(const GenomePtr& pGenome) override final;
};

} /* namespace ea */
