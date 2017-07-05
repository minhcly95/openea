/*
 * TournamentSelection.cpp
 *
 *  Created on: Mar 29, 2017
 *      Author: Bui Quang Minh
 */

#include "../pch.h"
#include "TournamentSelection.h"

#include "../core/pool/OrganismPool.h"
#include "UniformSelection.h"

namespace ea {

EA_TYPEINFO_CUSTOM_IMPL(TournamentSelection) {
	return *ea::TypeInfo("TournamentSelection")
		.Add("probability", &TournamentSelection::mProb)
		->SetConstructor<TournamentSelection, uint>("size");
}

/**
 * @class TournamentSelection
 * Implementation of **Tournament selection** operator.
 * This Selector will choose the best Organism from a random subset of the Pool.
 * If multiple Organism is required, that process is repeated.
 * The probability of choosing the best Organism can also be configured.
 * If the best Organism wasn't chosen, the second best will have the same probability
 * of choosing, and this process is repreated to the last Organism in the pool.
 *
 * @note
 * It is required that the input Pool size must be greater than or equal to the tournament size.
 * The same Organism could be selected more than once, because it can be chosen in different tournaments.
 *
 * @method
 * Use UniformSelection without replacement to select the tournament Pool,
 * then sort the Pool and choose the Organism with the given probability.
 * Repeat the above process until full.
 *
 * @time
 * O(N * klogk) with N is the input Pool size and k is the tournament size.
 *
 * @name{TournamentSelection}
 *
 * @eaml
 * @attr{size,
 * uint - Required - The tournament size.}
 * @attr{probability,
 * double - Optional - The probability of choosing the best Organism.}
 * @endeaml
 */

/**
 * Create a TournamentSelection with given tournament size and probability.
 * @param pSize The tournament size.
 * @param pProb The probability of choosing the best Organism.
 */
TournamentSelection::TournamentSelection(uint pSize, double pProb) :
		mSize(pSize), mProb(pProb) {
}

TournamentSelection::~TournamentSelection() {
}

/**
 * Get the size of the tournament.
 * @return The size of the tournament.
 */
uint TournamentSelection::GetSize() const {
	return mSize;
}

/**
 * Get the probability of choosing the best Organism.
 * @return The probability (from 0.0 to 1.0 inclusively).
 */
double TournamentSelection::GetProbability() const {
	return mProb;
}

OrganismPoolPtr TournamentSelection::DoSelect(
		const OrganismPoolPtr& pPool, uint pTargetSize) {
	static uniform_real_distribution<double> dist(0, 1);
	static UniformSelection uniSelection(false);

	if (pPool->size() < mSize)
		throw invalid_argument(
				"TournamentSelection requires pool size larger than tournament size.");

	OrganismPoolPtr newPool = make_shared<OrganismPool>();

	for (uint i = 0; i < pTargetSize; i++) {
		OrganismPoolPtr tournament = uniSelection.Select(pPool, mSize);
		tournament->Sort();

		for (auto org = tournament->begin(); org != tournament->end(); org++)

			if (mProb == 1.0 || dist(Random::generator) < mProb
					|| org == tournament->end() - 1) {

				newPool->push_back(*org);
				break;
			}
	}

	return newPool;
}

} /* namespace ea */
