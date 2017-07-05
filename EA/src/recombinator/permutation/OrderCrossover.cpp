/*
 * OrderCrossover.cpp
 *
 *  Created on: Apr 10, 2017
 *      Author: Kiet Vo, Bui Quang Minh
 */

#include "../../pch.h"
#include "OrderCrossover.h"
#include "../../genome/PermutationGenome.h"

namespace ea {

/**
 * @class OrderCrossover
 * Implementation of **Order crossover** operator for PermutationGenome.
 * This Recombinator is only applicable on PermutationGenome.
 *
 * This is the implementation of the operator with the same name described in the book
 * **Introduction to Evolutionary Computing**, 2nd Edition, 2015 by *A.E. Eiben* and *J.E. Smith*
 *
 * @method
 * Copy a segment from one parent, then iterate through the array from the end of the segment.
 * A table of existence is maintained to detect existed genes.
 *
 * @time
 * O(N)
 *
 * @name{OrderCrossover}
 */

OrderCrossover::OrderCrossover() {
}

OrderCrossover::~OrderCrossover() {
}

PermutationGenomePtr OrderCrossover::DoCombine(
		vector<PermutationGenomePtr>& pParents) {
	vector<uint>* genomes[2] = { &pParents[0]->GetGenes(),
			&pParents[1]->GetGenes() };

	uint genomeLength = (*genomes[0]).size();
	if (genomeLength == 0)
		throw invalid_argument(
				"OrderCrossover: Genome length must be greater than 0");
	if ((*genomes[1]).size() != genomeLength)
		throw invalid_argument("OrderCrossover: Two parents must have same genome length!");

	auto interval = Random::OrderedPair<uint>(0, genomeLength);
	uint lowerIndex = interval.first, upperIndex = interval.second;

	// Create the child's genome from genome 0 (segment [lower, upper))
	PermutationGenomePtr newGenome = make_shared<PermutationGenome>();
	vector<uint>& newGenes = newGenome->GetGenes();
	newGenes.resize(genomeLength);

	auto segBegin = newGenes.begin() + lowerIndex;
	auto segEnd = newGenes.begin() + upperIndex;

	std::copy((*genomes[0]).begin() + lowerIndex,
			(*genomes[0]).begin() + upperIndex, segBegin);

	// Bit mask
	vector<bool> exist(genomeLength, false);
	for (auto i = segBegin; i != segEnd; i++)
		exist[*i] = true;

	// Copy the remaining genes from genome 1
	uint childPosition = upperIndex == genomeLength ? 0 : upperIndex;

	bool wrapped = false;
	for (uint i = upperIndex; !wrapped || i != upperIndex; i++) {
		if (i >= genomeLength) {
			wrapped = true;
			i = 0;
		}

		if (exist[(*genomes[1])[i]])
			continue;

		newGenes[childPosition++] = (*genomes[1])[i];

		if (childPosition >= genomeLength)
			childPosition = 0;
	}

	return newGenome;
}

} /* namespace ea */
