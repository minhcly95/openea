/*
 * ScalarFitness.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "ScalarFitness.h"
#include "../Common.h"

namespace ea {

/**
 * @class ScalarFitness
 * Fitness implementation for single scalar fitness value.
 * The generic Fitness class only provides the template for compare operators.
 * This class provides instead an implementation for the case of a single fitness value of type double.
 * Users should use the ScalarEvaluator to create this kind of Fitness indirectly.
 *
 * @name{ScalarFitness}
 *
 * @see ScalarEvaluator.
 */

/**
 * Create a ScalarFitness with the given value.
 * A boolean value must also be provided to decide the compare direction. Usually this will be provided
 * automatically by ScalarEvaluator::IsMaximizer().
 * @param pValue The scalar fitness value.
 * @param pMaximizer Whether the corresponding ScalarEvaluator is a maximizer (true) or minimizer (false).
 */
ScalarFitness::ScalarFitness(double pValue, bool pMaximizer) :
		mValue(pValue), mMaximizer(pMaximizer) {
}

/**
 * @fn ScalarFitness::ScalarFitness()
 * The default constructor used by BinarySerializer (users should not use this).
 * The default constructor is required by every Storable class to enable deserialization.
 * @see Storable
 */

ScalarFitness::~ScalarFitness() {
}

/**
 * Get the fitness value of this ScalarFitness.
 * @return The fitness value.
 */
double ScalarFitness::GetValue() const {
	return mValue;
}

/**
 * Whether the corresponding ScalarEvaluator is a maximizer or a minimizer.
 * This is usually set automatically by ScalarEvaluator::IsMaximizer().
 * @return true if it is a maximizer, false for minimizer.
 * @see ScalarEvaluator::IsMaximizer()
 */
bool ScalarFitness::IsMaximizer() const {
	return mMaximizer;
}

int ScalarFitness::Compare(const Fitness& other) const {
	const ScalarFitness* casted = dynamic_cast<const ScalarFitness*>(&other);
	if (!casted)
		throw EA_EXCEPTION(EAException, FITNESS_BAD_CAST,
				"ScalarFitness can only be compared with other ScalarFitness.");
	if (this->mMaximizer != casted->mMaximizer)
		throw EA_EXCEPTION(EAException, FITNESS_INCOMPATIBLE,
				"Maximizing ScalarFitness cannot be compared with minimizing ScalarFitness.");

	if (this->mValue == casted->mValue)
		return 0;
	else
		return ((this->mValue > casted->mValue) ^ mMaximizer) ? -1 : 1;
}

inline void ScalarFitness::DoSerialize(ostream& pStream) const {
	Write<bool>(pStream, mMaximizer);
	Write<double>(pStream, mValue);
}

inline void ScalarFitness::DoDeserialize(istream& pStream) {
	mMaximizer = Read<bool>(pStream);
	mValue = Read<double>(pStream);
}

} /* namespace ea */
