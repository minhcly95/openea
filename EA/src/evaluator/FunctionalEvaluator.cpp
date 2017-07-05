/*
 * FunctionalEvaluator.cpp
 *
 *  Created on: Apr 15, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "FunctionalEvaluator.h"

namespace ea {

/**
 * @class FunctionalEvaluator
 * A ScalarEvaluator which accepts an std::function as the evaluation function.
 * FunctionalEvaluator allows users creating an in-place ScalarEvaluator without write a new child class.
 * This class contributes mainly for usability, where simple evaluation function doesn't need complex structures
 * and therefore users don't need to write a new class for it.
 *
 * If users know which type of Genome will be processed, they can use TypedFunctionalEvaluator which provides
 * similar functionalities with auto-casted input in addition.
 *
 * @see TypedFunctionalEvaluator.
 */

/**
 * Create a ScalarEvaluator with the given function as the evaluation function.
 * The given function will be invoked when Evaluate() is called.
 * @param pFunc The evaluation function, which accepts a const GenomePtr& and returns a double.
 * @param pMaximizer Whether this Evaluator is a maximizer (true) or a minimizer (false).
 */
FunctionalEvaluator::FunctionalEvaluator(FunctionType pFunc, bool pMaximizer) :
		mMaximizer(pMaximizer), mFunc(pFunc) {
}

FunctionalEvaluator::~FunctionalEvaluator() {
}

bool FunctionalEvaluator::IsMaximizer() {
	return mMaximizer;
}

/**
 * Implementation of ScalarEvaluator::DoScalarEvaluate().
 * This function will invoke the given function at the constructor with the given Genome.
 */
double FunctionalEvaluator::DoScalarEvaluate(const GenomePtr& pGenome) {
	return mFunc(pGenome);
}

} /* namespace ea */
