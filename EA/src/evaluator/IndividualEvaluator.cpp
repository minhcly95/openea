/*
 * IndividualEvaluator.cpp
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../pch.h"

#include "IndividualEvaluator.h"
#include "../EA/Core.h"
#include "../misc/ClusterComputableImpl.h"

namespace ea {

/**
 * @class IndividualEvaluator
 * The interface for evaluation method at individual level.
 * This class provides the interface to implement custom evaluation method at individual level.
 * Users should override DoEvaluate(const GenomePtr&) in child class to implement their customized code.
 *
 * This interface requires a general Fitness value (which is not useful in most case).
 * Please take a look on ScalarEvaluator which only requires a fitness value of type double.
 *
 * This interface works at the individual level, which takes in a Genome and produces a Organism.
 * The evaluation of each individual will be independent from each other.
 * If the evaluation method requires interaction between individuals, see the more general interface: Evaluator.
 *
 * The interface supports multi-threading and cluster computation by default.
 *
 * @see ScalarEvaluator
 * @see Evaluator
 */

/**
 * @fn FitnessPtr IndividualEvaluator::DoEvaluate(const GenomePtr& pGenome)
 * Implementation of the evaluation method.
 *
 * IndividualEvaluator requires a general Fitness value as the return.
 * If users want to return only a fitness value of type double, see ScalarEvaluator.
 *
 * IndividualEvaluator interface works at the individual level.
 * For the interfaces which work at the Pool level, see Evaluator.
 *
 * @param pGenome The input Genome to be evaluated.
 * @return The Fitness value corresponding to the input Genome.
 */

/**
 * The public method of DoEvaluate().
 * This function handles the exception thrown when calling DoEvaluate() internally.
 * The returned Fitness will be wrapped with the corresponding Genome into an Organism.
 * This function also calls IncreaseEvaluationCount() to increase the evaluation counter.
 *
 * @param pGenome The input Genome to be evaluated.
 * @return The Organism object, which encapsulates the Genome and its Fitness value.
 */
OrganismPtr IndividualEvaluator::Evaluate(const GenomePtr& pGenome) {
	auto rValue = make_shared<Organism>(pGenome, EvaluateFitness(pGenome));
	IncreaseEvaluationCount();
	return rValue;
}

/**
 * The public method of DoEvaluate() which returns only Fitness value.
 * This function won't increase the evaluation counter.
 * @param pGenome The input Genome to be evaluated.
 * @return The Fitness value corresponding to the input Genome.
 */
FitnessPtr IndividualEvaluator::EvaluateFitness(const GenomePtr& pGenome) {
	try {
		return this->DoEvaluate(pGenome);
	} catch (exception& e) {
		throw EA_EXCEPTION(EAException, EVALUATOR_ERROR,
				"Exception caught when executing IndividualEvaluator.",
				current_exception());
	}
}

/**
 * Specialized implementation to be compatible with Evaluator.
 * Each Genome in the given GenomePool will be processed by IndividialEvaluator::Evaluate() function.
 * The process supports multi-threading and computation on cluster.
 */
OrganismPoolPtr IndividualEvaluator::DoEvaluate(const GenomePoolPtr& pGenomePool) {
	GenomePoolPtr inputPool = pGenomePool;
	OrganismPoolPtr outputPool = make_shared<OrganismPool>(inputPool->size());

	vector<FitnessPtr> clusterResult = ExecuteInCluster(vector<GenomePtr>(*inputPool), true,
			[&] (uint, const GenomePtr&, const FitnessPtr&) {
		IncreaseEvaluationCount();
	});

	MultiThreading::For(0, inputPool->size(), [&] (int i) {
		(*outputPool)[i] = make_shared<Organism>((*inputPool)[i], clusterResult[i]);
	});

	return outputPool;
}

FitnessPtr IndividualEvaluator::ProcessOnRemote(const GenomePtr& pInput) {
	return EvaluateFitness(pInput);
}

} /* namespace ea */
