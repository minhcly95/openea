/*
 * IndividualEvaluator.h
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../core/interface/Evaluator.h"
#include "../misc/ClusterComputable.h"
#include "../core/interface/Genome.h"
#include "../core/interface/Fitness.h"

namespace ea {

class IndividualEvaluator: public Evaluator,
		public ClusterComputable<GenomePtr, FitnessPtr> {
public:
	using FunctionType = function<FitnessPtr(const GenomePtr&)>;

	inline virtual ~IndividualEvaluator() {
	}

	OrganismPtr Evaluate(const GenomePtr& pOrganism);
	FitnessPtr EvaluateFitness(const GenomePtr& pOrganism);

protected:
	virtual FitnessPtr DoEvaluate(const GenomePtr& pGenome) = 0;
	virtual OrganismPoolPtr DoEvaluate(const GenomePoolPtr& pGenomePool) override;

	virtual FitnessPtr ProcessOnRemote(const GenomePtr& pInput) override;
};

} /* namespace ea */
