/*
 * Evaluator.cpp
 *
 *  Created on: May 27, 2017
 *      Author: Bui Quang Minh
 */

#include "../../pch.h"
#include "Evaluator.h"
#include "../../Common.h"
#include "../Session.h"
#include "../Population.h"

namespace ea {

/**
 * @class Evaluator
 * The interface for evaluation method.
 * This class provides the interface to implement custom evaluation method at pool level.
 * Users should override DoEvaluate() in child class to implement their customized code.
 *
 * This interface works at the Pool level, which takes in a GenomePool and produces a OrganismPool.
 * For the interfaces which work at the individual level, see IndividualEvaluator or ScalarEvaluator.
 *
 * Evaluator is compatible with Operator and OperatorGroup.
 * It takes a GenomePool as input and produces an OrganismPool.
 *
 * @see IndividualEvaluator
 * @see ScalarEvaluator
 */

/**
 * @fn OrganismPoolPtr Evaluator::DoEvaluate(const GenomePoolPtr& pGenomePool)
 * Implementation of the evaluation method.
 *
 * Evaluator interface works at the Pool level, which takes in a GenomePool and produces a OrganismPool.
 * For the interfaces which work at the individual level, see IndividualEvaluator or ScalarEvaluator.
 *
 * @param pGenomePool The input GenomePool to be evaluated.
 * @return The OrganismPool containing evaluated Organism.
 */

Evaluator::~Evaluator() {
}

/**
 * The public method of DoEvaluate().
 * This function handles the exception thrown when calling DoEvaluate() internally.
 * @param pGenomePool The input GenomePool to be evaluated.
 * @return The OrganismPool containing evaluated Organism.
 */
OrganismPoolPtr Evaluator::Evaluate(const GenomePoolPtr& pGenomePool) {
	try {
		return this->DoEvaluate(pGenomePool);
	} catch (exception& e) {
		throw EA_EXCEPTION(EAException, EVALUATOR_ERROR,
				"Exception caught when executing Evaluator.",
				current_exception());
	}
}

/**
 * Get the Session running context.
 * This Session object is only available when Evaluator is invoked via an Operator of OperatorGroup.
 * Otherwise, this function throws an exception.
 * @return The current Session running context.
 */
const SessionPtr& Evaluator::GetSession() const {
	if (!mSession)
	throw EA_EXCEPTION(EAException, SESSION_DEPENDENT,
			"This Evaluator is Session-dependent. Evaluate() cannot be called.");
	return mSession;
}

/**
 * Set the Session running context.
 * Use this function if users want to invoke the Evaluate() method within a specific Session context.
 * It is only required if the implementation depends on Session (see GetSession()).
 * @param pSession The Session running context.
 */
void Evaluator::SetSession(const SessionPtr& pSession) {
	mSession = pSession;
}

/**
 * Helper function for child classes to increase the evaluation counter of Population.
 * This function only increases the counter if the operator is invoked with a Session
 * (which means operator()() is called instead of Evaluate()).
 */
void Evaluator::IncreaseEvaluationCount() {
	if (mSession != nullptr)
		mSession->GetPopulation()->IncreaseEvaluation();
}

/**
 * Invocation entry for Operator and OperatorGroup.
 * This function will call Evaluate() internally and set the Session context for IncreaseEvaluationCount().
 * @param pSession The current running Session (given automatically by Operator).
 * @param pGenomePool The input GenomePool to be evaluated.
 * @return The OrganismPool containing evaluated Organism.
 */
OrganismPoolPtr Evaluator::operator ()(const SessionPtr& pSession, const GenomePoolPtr& pGenomePool) {
	mSession = pSession;
	OrganismPoolPtr rValue = Evaluate(pGenomePool);
	mSession = nullptr;
	return rValue;
}

} /* namespace ea */
