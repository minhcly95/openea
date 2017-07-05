/*
 * EdgeCrossover.cpp
 *
 *  Created on: Apr 7, 2017
 *      Author: Kiet Vo
 */

#include "../../pch.h"
#include "EdgeCrossover.h"
#include "../../genome/PermutationGenome.h"

using namespace std;

namespace ea {

/**
 * @class EdgeCrossover
 * Implementation of **Edge crossover** operator for PermutationGenome.
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
 * @name{EdgeCrossover}
 */

EdgeCrossover::EdgeCrossover() {
}

EdgeCrossover::~EdgeCrossover() {
}

#ifndef DOXYGEN_IGNORE
using TableType = unordered_map<uint, unordered_map<uint, bool>>;

TableType ConstructEdgeTable(vector<uint>* genomes[2]);

template<class T>
int PickRandomElement(unordered_map<uint, T> & map);
void RemoveAllReferences(uint element, TableType & edgeTable);
int PickNextElement(uint currentElement, TableType & edgeTable);
#endif

/**
 * Edge-3 Crossover
 */
PermutationGenomePtr EdgeCrossover::DoCombine(vector<PermutationGenomePtr>& pParents) {
	vector<uint>* genomes[2] = { &pParents[0]->GetGenes(),
			&pParents[1]->GetGenes() };

	uint genomeLength = (*genomes[0]).size();
	if (genomeLength < 2)
		throw invalid_argument("Genome length must be greater than 1 for PermutationGenome");
	if ((*genomes[1]).size() != genomeLength)
		throw invalid_argument("Two parents with different genome lengths!");

	PermutationGenomePtr newGenome = make_shared<PermutationGenome>();
	vector<uint>& newGenes = newGenome->GetGenes();

	TableType edgeTable = ConstructEdgeTable(genomes);

	uint element = PickRandomElement(edgeTable);
	newGenes.push_back(element);

	for (uint i = 1; i < genomeLength; i++) {
		RemoveAllReferences(element, edgeTable);
		element = PickNextElement(element, edgeTable);
		newGenes.push_back(element);
	}

	return newGenome;
}

TableType ConstructEdgeTable(vector<uint>* genomes[2]) {

	TableType edgeTable;
	for (uint i = 0; i < 2; i++) {
		auto first = (*genomes[i]).begin();
		auto last = (*genomes[i]).end() - 1;

		for (auto element = first; element != last + 1; element++) {
			unordered_map<uint, bool> *row = &edgeTable[*element];

			auto neighbour1 = element + 1;
			auto neighbour2 = element - 1;

			if (element == first) {
				neighbour1 = first + 1;
				neighbour2 = last;
			} else if (element == last) {
				neighbour1 = last - 1;
				neighbour2 = first;
			}

			bool found = row->find(*neighbour1) != row->end();
			(*row)[*neighbour1] = found;

			found = row->find(*neighbour2) != row->end();
			(*row)[*neighbour2] = found;
		}
	}
	return edgeTable;
}

template<class T>
int PickRandomElement(unordered_map<uint, T> & map) {
	// random generator
	uniform_int_distribution<uint> dist(0, map.size() - 1);

	// get random element
	auto it = map.begin();
	advance(it, dist(Random::generator));

	return it->first;	// important! only pick, no removal (make it reusable)
}

void RemoveAllReferences(uint element, TableType & edgeTable) {
	for (auto it = edgeTable.begin(); it != edgeTable.end(); it++) {
		if (it->first == element)
			continue;

		it->second.erase(element);
	}
}

int PickNextElement(uint currentElement, TableType & edgeTable) {
	// extract the current row
	unordered_map<uint, bool> row = edgeTable[currentElement];
	edgeTable.erase(currentElement);

	// pick another row if the current row is empty
	if (row.size() == 0) {
		return PickRandomElement(edgeTable);
	}

	// find common edge
	for (auto it = row.begin(); it != row.end(); it++)
		if (it->second)
			return it->first;

	// find element that has shortest list
	// start with a random element in the row
	uint newElement = PickRandomElement(row);
	uint shortest = edgeTable[newElement].size();

	for (auto it = row.begin(); it != row.end(); it++) {
		uint temp = edgeTable[it->first].size();
		if (temp < shortest) {
			shortest = temp;
			newElement = it->first;
		}
	}
	return newElement;
}

} /* namespace ea */
