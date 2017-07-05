/*
 * UniformCrossover.h
 *
 *  Created on: Mar 29, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../EA/Type/Array.h"
#include "../TypedRecombinator.h"

namespace ea {

using namespace std;

/**
 * Implementation of **Uniform crossover** operator for ArrayGenome.
 * This Recombinator is only applicable on ArrayGenome of the same type of genes T.
 *
 * When apply, each gene in the child genome will have 50% to inherit from the first parent
 * and 50% from the second. If one parent is longer than the other (in length), the exceed part
 * will be transfered to the child.
 *
 * @tparam T Type of genes.
 *
 * @method
 * Iterate through the array of genes, then apply probability test.
 *
 * @time
 * O(N)
 *
 * @name{BoolUniformCrossover, IntUniformCrossover, DoubleUniformCrossover}
 */
template<class T>
class UniformCrossover: public TypedRecombinator<ArrayGenome<T>> {
public:
	EA_TYPEINFO_CUSTOM_DECL

	inline UniformCrossover() {
	}
	inline virtual ~UniformCrossover() {
	}
	inline virtual uint GetParentCount() override {
		return 2;
	}

protected:
	virtual ArrayGenomePtr<T> DoCombine(vector<ArrayGenomePtr<T>>& pParents) override;
};

#ifndef DOXYGEN_IGNORE
EA_TYPEINFO_SPECIALIZED_DEFAULT(BoolUniformCrossover)
EA_TYPEINFO_SPECIALIZED_DEFAULT(IntUniformCrossover)
EA_TYPEINFO_SPECIALIZED_DEFAULT(DoubleUniformCrossover)
#endif

template<class T>
ArrayGenomePtr<T> UniformCrossover<T>::DoCombine(vector<ArrayGenomePtr<T>>& pParents) {
	vector<T>* genomes[2] = { &pParents[0]->GetGenes(),
			&pParents[1]->GetGenes() };

	uint longerParent = ((*genomes[0]).size() > (*genomes[1]).size()) ? 0 : 1;
	uint minSize = (*genomes[longerParent ^ 1]).size();

	ArrayGenomePtr<T> newGenome = make_shared<ArrayGenome<T>>();
	vector<T>& newGenes = newGenome->GetGenes();
	for (uint i = 0; i < minSize; i++) {
		newGenes.push_back((*genomes[Random::generator() & 1])[i]);
	}
	newGenes.insert(newGenes.end(),
			(*genomes[longerParent]).begin() + minSize,
			(*genomes[longerParent]).end());

	return newGenome;
}

} /* namespace ea */

