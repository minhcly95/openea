/*
 * FunctionalEvaluator.h
 *
 *  Created on: Apr 15, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../EA/Type/Core.h"
#include <functional>
#include "ScalarEvaluator.h"

namespace ea {

class FunctionalEvaluator : public ScalarEvaluator {
public:
	FunctionalEvaluator(FunctionType pFunc, bool pMaximizer = true);
	virtual ~FunctionalEvaluator();

	virtual bool IsMaximizer() override;

private:
	bool mMaximizer;
	FunctionType mFunc;

protected:
	virtual double DoScalarEvaluate(const GenomePtr& pGenome) override;
};

} /* namespace ea */
