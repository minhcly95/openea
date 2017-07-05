/*
 * PartiallyMappedCrossover.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: Kiet Vo
 */

#include "../../pch.h"
#include "PartiallyMappedCrossover.h"
#include "../../genome/PermutationGenome.h"

namespace ea {

/**
 * @class PartiallyMappedCrossover
 * Implementation of **Partially mapped crossover** operator (or **PMX**) for PermutationGenome.
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
 * @name{PartiallyMappedCrossover}
 */

PartiallyMappedCrossover::PartiallyMappedCrossover() {
}

PartiallyMappedCrossover::~PartiallyMappedCrossover() {
}

PermutationGenomePtr PartiallyMappedCrossover::DoCombine(vector<PermutationGenomePtr>& pParents) {
	vector<uint>* genomes[2] = { &pParents[0]->GetGenes(),
			&pParents[1]->GetGenes() };

	uint genomeLength = (*genomes[0]).size();
	if (genomeLength < 2)
		throw invalid_argument("Genome length must be greater than 1 for PermutationGenome");
	if ((*genomes[1]).size() != genomeLength)
		throw invalid_argument("Two parents with different genome lengths!");

	// random generator
	uniform_int_distribution<uint> dist(0, (*genomes[0]).size() - 1);

	// generate 2 random indices
	uint lowerIndex = dist(Random::generator), upperIndex;
	do {
		upperIndex = dist(Random::generator);
	} while (lowerIndex == upperIndex);

	// swap 2 indices
	if (lowerIndex > upperIndex) {
		lowerIndex += upperIndex;
		upperIndex = lowerIndex - upperIndex;
		lowerIndex -= upperIndex;
	}

	// create the child's genome from the p1's genome (segment [lower, upper])
	vector<uint>& p1Genome = (*genomes[0]), &p2Genome = (*genomes[1]);
	PermutationGenomePtr newGenome = make_shared<PermutationGenome>(p1Genome);
	vector<uint>& newGenes = newGenome->GetGenes();

	// keep track of vacancies in child's genome (outside the "segment")
	vector<uint> vacancies;
	for (uint i = 0; i < lowerIndex; i++)
		vacancies.push_back(i);
	for (uint i = upperIndex + 1; i < genomeLength; i++)
		vacancies.push_back(i);

	vector<uint>::iterator p1First = p1Genome.begin() + lowerIndex;
	vector<uint>::iterator p1Last = p1Genome.begin() + upperIndex + 1;

	vector<uint>::iterator p2First = p2Genome.begin() + lowerIndex;
	vector<uint>::iterator p2Last = p2Genome.begin() + upperIndex + 1;

	// copy genes in "segment" of p2 into child (only if they are not there already)
	vector<uint>::iterator p2Gene;
	for (p2Gene = p2First; p2Gene != p2Last; p2Gene++) {

		// does this gene in p2 also exist in copied segment?
		vector<uint>::iterator foundGene = find(p1First, p1Last, *p2Gene);
		bool found = foundGene != p1Last;

		// if not then
		if (!found) {
			uint tempIndex = distance(p2Genome.begin(), p2Gene);	// index of this gene in p2

			bool occupied = true;
			while (occupied) {
				uint tempGene = newGenes[tempIndex];	// gene in child of the same position

				// where is this value in p2?
				foundGene = find(p2Genome.begin(), p2Genome.end(), tempGene);
				tempIndex = distance(p2Genome.begin(), foundGene);

				// is tempIndex in child occupied?
				occupied = find(vacancies.begin(), vacancies.end(), tempIndex)
						== vacancies.end();
			}

			// assign new gene to child
			newGenes[tempIndex] = *p2Gene;

			vacancies.erase(remove(vacancies.begin(), vacancies.end(), tempIndex),
					vacancies.end());
		}
	}

	// copy the rest from p2 to child
	for (uint i = 0; i < genomeLength; i++) {

		// skip over the "segment" in p2
		if (lowerIndex <= i && i <= upperIndex)
			continue;

		uint newGene = p2Genome[i];

		// ignore genes in p2 that already exist in copied segment
		bool found = find(p1First, p1Last, newGene) != p1Last;
		if (found)
			continue;

		// assign new gene to child
		newGenes[vacancies[0]] = newGene;
		vacancies.erase(vacancies.begin());
	}

	return newGenome;
}

} /* namespace ea */
