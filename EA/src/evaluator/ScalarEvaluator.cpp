/*
 * ScalarEvaluator.cpp
 *
 *  Created on: Apr 25, 2017
 *      Author: Bui Quang Minh
 */

#include "ScalarEvaluator.h"
#include "../fitness/ScalarFitness.h"
#include "../Common.h"

namespace ea {

using namespace std;

/**
 * @class ScalarEvaluator
 * Specialization of IndividualEvaluator for ScalarFitness.
 * The interface IndividualEvaluator requires a general Fitness object as the return value of
 * IndividualEvaluator::DoEvaluate(). This class defines a new function DoScalarEvaluate()
 * which only requires a fitness value of type double to be returned, and redirects DoEvaluate()
 * to the new function so it is compatible with IndividualEvaluator.
 *
 * If users know in advance which type of Genome will be processed, see TypedScalarEvaluator.
 * If users want to create a ScalarEvaluator in-place without writing a new child class, see FunctionalEvaluator
 * and TypedFunctionalEvaluator.
 *
 * @see DoScalarEvaluate()
 * @see TypedScalarEvaluator
 * @see FunctionalEvaluator
 * @see TypedFunctionalEvaluator
 */

/**
 * @fn bool ScalarEvaluator::IsMaximizer()
 * Whether this ScalarEvaluator is a maximizer or a minimizer.
 * The default value is true (maximizer). If user-defined class is a minimizer,
 * this function must be overriden and return false. The value of this function
 * will determine the value of ScalarFitness::IsMaximizer().
 * @return true (default) if this ScalarEvaluator is a maximizer, false if it is a minimizer.
 */

/**
 * @fn double ScalarEvaluator::DoScalarEvaluate(const GenomePtr& pGenome)
 * Implementation of the evaluation method.
 * Instead of requiring a Fitness object as the return value, this function requires only
 * a scalar fitness value of type double. The ScalarFitness object will be created internally
 * when Evaluate() is called.
 * @param pGenome The input Genome to be evaluated.
 * @return A scalar value of type double representing the fitness value.
 */

ScalarEvaluator::~ScalarEvaluator() {
}

/**
 * Specialized implementation to be compatible with IndividualEvaluator.
 * This function will call DoScalarEvaluate() internally.
 * @see Evaluator::DoEvaluate()
 */
FitnessPtr ScalarEvaluator::DoEvaluate(const GenomePtr& pGenome) {
	return make_shared<ScalarFitness>(DoScalarEvaluate(pGenome), IsMaximizer());
}

} /* namespace ea */
