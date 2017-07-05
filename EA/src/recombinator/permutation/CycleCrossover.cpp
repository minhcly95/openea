/*
 * CycleCrossover.cpp
 *
 *  Created on: Apr 11, 2017
 *      Author: Kiet Vo
 */

#include "../../pch.h"
#include "CycleCrossover.h"
#include "../../genome/PermutationGenome.h"

namespace ea {

/**
 * @class CycleCrossover
 * Implementation of **Cycle crossover** operator for PermutationGenome.
 * This Recombinator is only applicable on PermutationGenome.
 *
 * This is the implementation of the operator with the same name described in the book
 * **Introduction to Evolutionary Computing**, 2nd Edition, 2015 by *A.E. Eiben* and *J.E. Smith*
 *
 * @method
 * Unknown
 *
 * @time
 * Unknown
 *
 * @name{CycleCrossover}
 */

CycleCrossover::CycleCrossover() {
}

CycleCrossover::~CycleCrossover() {
}

PermutationGenomePtr CycleCrossover::DoCombine(vector<PermutationGenomePtr>& pParents) {
	vector<uint>* genomes[2] = { &pParents[0]->GetGenes(),
			&pParents[1]->GetGenes() };

	uint genomeLength = (*genomes[0]).size();
	if (genomeLength < 2)
		throw invalid_argument("PermutationGenome requries genome length greater than 1.");
	if ((*genomes[1]).size() != genomeLength)
		throw invalid_argument("Two parents must have the same genome length.");

	// make the child a copy of p1 (including genes of cycle1 from p1)
	vector<uint>& p1Genome = (*genomes[0]), p2Genome = (*genomes[1]);
	PermutationGenomePtr newGenome = make_shared<PermutationGenome>(p1Genome);
	vector<uint>& newGenes = newGenome->GetGenes();

	uint currentIndex = 0;
	vector<uint> cycle1;
	cycle1.push_back(currentIndex);

	do {
		// where is this (p2's) gene in p1?
		auto foundGene = find(p1Genome.begin(), p1Genome.end(), p2Genome[currentIndex]);

		currentIndex = distance(p1Genome.begin(), foundGene);
		cycle1.push_back(currentIndex);
	} while (currentIndex != 0);

	// copy genes of cycle2 from p2 to child
	for (uint i = 0; i < genomeLength; i++) {
		bool found = find(cycle1.begin(), cycle1.end(), i) != cycle1.end();
		if (found)
			continue;

		newGenes[i] = p2Genome[i];
	}

	return newGenome;
}

} /* namespace ea */
