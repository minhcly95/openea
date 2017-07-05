/*
 * MuPlusLambdaStrategy.cpp
 *
 *  Created on: May 18, 2017
 *      Author: Bui Quang Minh
 */

#include "EvolutionStrategy.h"

#include "../../pch.h"
#include "../../EA/Core.h"

namespace ea {

/**
 * @class EvolutionStrategy
 * A trivial implementation of <b>Evolution %Strategy</b>.
 * This class implements a trivial \f$(\mu + \lambda)\f$-ES or \f$(\mu,\lambda)\f$-ES with basic operators
 * including recombination, mutation, parent and survival selection.
 * The execution followed this order:
 * - [S] Spawning phase: Recombinator in recombination list is applied on the main pool
 * to spawn new Genome and put into the spawn pool.
 * - [M] Mutation phase: Mutator in mutation list is applied on newly created Genome.
 * - [E] Evaluation phase: Evaluator is applied to each Genome, creates a Organism wrapper for each Genome.
 * - [F] Filtering phase: Survival Selector is applied to truncate the main pool to the population size.
 * - Global Hook execution.
 *
 * The numbering of Pool of this Strategy is:
 * - Main pool: OrganismPool #0 containing evaluated and filtered Organism.
 * - Spawn pool: GenomePool #1 containing newly spawned and mutated Genome.
 *
 * To add operators into the DefaultStrategy, use these class fields:
 * - #initializer
 * - #evaluator
 * - #recombinators
 * - #mutators
 * - #survivalSelector
 * - #hooks
 *
 * To switch between \f$(\mu + \lambda)\f$ (which merges new Organism with the old main pool) and \f$(\mu,\lambda)\f$
 * (which discard the old main pool and keep only new Organism), see SetSelectionMode().
 *
 * @name{EvolutionStrategy}
 *
 * @eaml
 * @attr{size,
 * uint - Required - The size of the main pool}
 * @attr{initializer,
 * Initializer - Required - The initialization method}
 * @attr{evaluator,
 * IndividualEvaluator - Required - The evaluation method}
 * @attr{recombinators,
 * MetaRecombinator - List\, Required - List of recombination operators
 * wrapped with their parent selection methods and the offspring ratio}
 * @attr{mutators,
 * MetaMutator - List\, Optional - List of mutation operators wrapped with their mutation rate}
 * @attr{survival-selector,
 * ResizableSelector - Required - The survival selection operator}
 * @attr{hooks,
 * Hook - List\, Optional - List of global Hook}
 * @attr{selection-mode,
 * SelectionMode - Optional - Whether the main pool will be discarded}
 * @endeaml
 */

#ifndef DOXYGEN_IGNORE
EA_DEFINE_CUSTOM_SERIALIZER(EvolutionStrategy::SelectionMode, data, ss) {
	static HashMap<string, EvolutionStrategy::SelectionMode> strToCond = {
		{"+",  EvolutionStrategy::PLUS},
		{"plus",  EvolutionStrategy::PLUS},

		{",",  EvolutionStrategy::COMMA},
		{"comma",  EvolutionStrategy::COMMA}
	};

	string str;
	if (!bool(ss >> str))
		return false;

	auto itr = strToCond.find(str);
	if (itr == strToCond.end())
		return false;

	data = itr->second;
	return true;
}
#endif

EA_TYPEINFO_CUSTOM_IMPL(EvolutionStrategy) {
	return *ea::TypeInfo("EvolutionStrategy").
			Add("initializer", &EvolutionStrategy::initializer)
			->Add("evaluator", &EvolutionStrategy::evaluator)
			->Add("recombinators", &EvolutionStrategy::recombinators)
			->Add("mutators", &EvolutionStrategy::mutators)
			->Add("survival-selector", &EvolutionStrategy::survivalSelector)
			->Add("hooks", &EvolutionStrategy::hooks)
			->Add("selection-mode", &EvolutionStrategy::mSelectionMode)
			->SetConstructor<EvolutionStrategy, uint>("size");
}

EvolutionStrategy::EvolutionStrategy(uint popSize) :
		initializer(), recombinators(), mutators(),
		evaluator(), survivalSelector(),
		mPopSize(popSize), mSelectionMode(PLUS) { }

EvolutionStrategy::~EvolutionStrategy() {
}

void EvolutionStrategy::Setup() {
	GenomePoolPtr initPool = Execute("I", initializer, mPopSize);
	OrganismPoolPtr evalPool = Execute("IE", evaluator, initPool);

	GetPopulation()->SetPool(0, evalPool);
	GetPopulation()->SetPool(1, initPool);
}

void EvolutionStrategy::Begin() {
}

void EvolutionStrategy::Loop() {
	OrganismPoolPtr mainPool = GetPopulation()->GetOrganismPool(0);

	vector<GenomePoolPtr> spawnPools = ExecuteInParallel("S", recombinators, mainPool);

	GenomePoolPtr mutatedPool = ExecuteInSeries("M", mutators, GenomePool::Join(spawnPools));

	OrganismPoolPtr evaluatedPool = Execute("E", evaluator, mutatedPool);

	OrganismPoolPtr prefilterPool;
	if (mSelectionMode == PLUS)
		prefilterPool = OrganismPool::Join({mainPool, evaluatedPool});
	else
		prefilterPool = evaluatedPool;

	OrganismPoolPtr filteredPool = Execute("F", survivalSelector, prefilterPool, mPopSize);

	GetPopulation()->SetPool(0, filteredPool);
	GetPopulation()->SetPool(1, mutatedPool);
}

bool EvolutionStrategy::IsReady() {
	return bool(initializer) && evaluator && survivalSelector && recombinators.GetSize();
}

/**
 * Set the selection mode (plus or comma) of the algorithm.
 * This function allows users to switch between the \f$(\mu+\lambda)\f$-ES and \f$(\mu,\lambda)\f$-ES algorithm:
 * - If \f$(\mu+\lambda)\f$-ES is used (#PLUS), the old pool will be kept and merge with new Organism.
 * - If \f$(\mu,\lambda)\f$-ES is used (#COMMA), the old pool will be discarded, which requires the total offspring ratio must
 * be greater than 1.
 * @param pMode The selection mode (#PLUS or #COMMA).
 */
void EvolutionStrategy::SetSelectionMode(SelectionMode pMode) {
	mSelectionMode = pMode;
}

/**
 * Get the selection mode (plus or comma) of the algorithm.
 * @return The selection mode (#PLUS or #COMMA).
 * @see SetSelectionMode()
 */
EvolutionStrategy::SelectionMode EvolutionStrategy::GetSelectionMode() const {
	return mSelectionMode;
}

vector<string> EvolutionStrategy::GetTimeRecordOrder() const {
	return { "S", "M", "E", "F" };
}

} /* namespace ea */
