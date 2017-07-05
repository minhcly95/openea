/*
 * NPointCrossover.h
 *
 *  Created on: Mar 29, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../EA/Type/Array.h"
#include "../../core/interface/Recombinator.h"
#include "../../genome/ArrayGenome.h"

namespace ea {

using namespace std;

#ifndef DOXYGEN_IGNORE
template<class T, uint ... N> class NPointCrossover;
#endif

/**
 * Implementation of **N-point crossover** operator for ArrayGenome.
 * This Recombinator is only applicable on ArrayGenome of the same type of genes T.
 *
 * When apply, split points will be drawn randomly. Then the child genome will copy the genes from one parent.
 * When it encounter a split point, it switches the target parent until the end.
 * If one parent is longer than the other (in length), the exceed part will be transfered to the child.
 *
 * Specialization is @ref OnePointCrossover and @ref TwoPointCrossover. For more details, see NPointCrossover<T, N>.
 *
 * @tparam T Type of genes.
 *
 * @method
 * Draw split points randomly. Then iterate through the array of genes, switch parent if encounter a split point.
 *
 * @time
 * O(L + NlogN) with L is the length of input genomes and N is the number of split points.
 *
 * @name{BoolNPointCrossover, IntNPointCrossover, DoubleNPointCrossover}
 *
 * @eaml
 * @attr{cross-count, uint - Required - The number of split points.}
 * @endeaml
 *
 * @see NPointCrossover<T, N>
 */
template<class T>
class NPointCrossover<T> : public Recombinator {
private:
	uint mCrossCount;

protected:
	virtual GenomePtr DoCombine(vector<GenomePtr>& pParents) override;

public:
	EA_TYPEINFO_CUSTOM_DECL

	/**
	 * Create an NPointCrossover with the given number of split points.
	 * @param pCrossCount The number of split points.
	 */
	inline NPointCrossover(uint pCrossCount) :
			mCrossCount(pCrossCount) {
	}
	inline virtual ~NPointCrossover() {
	}
	inline virtual uint GetParentCount() override {
		return 2;
	}
	/**
	 * Get the number of split points.
	 * @return The number of split points.
	 */
	uint GetCrossCount() const {
		return mCrossCount;
	}
};

#ifndef DOXYGEN_IGNORE
EA_TYPEINFO_SPECIALIZED_INLINE(NPointCrossover, bool) {
	return *ea::TypeInfo("BoolNPointCrossover")
		.SetConstructor<BoolNPointCrossover, uint>("cross-count");
}
EA_TYPEINFO_SPECIALIZED_INLINE(NPointCrossover, int) {
	return *ea::TypeInfo("IntNPointCrossover")
		.SetConstructor<IntNPointCrossover, uint>("cross-count");
}
EA_TYPEINFO_SPECIALIZED_INLINE(NPointCrossover, double) {
	return *ea::TypeInfo("DoubleNPointCrossover")
		.SetConstructor<DoubleNPointCrossover, uint>("cross-count");
}
#endif

template<class T>
GenomePtr NPointCrossover<T>::DoCombine(vector<GenomePtr>& pParents) {
	vector<vector<T>*> genomes(pParents.size());
	transform(pParents.begin(), pParents.end(), genomes.begin(),
			[this](GenomePtr parent) {
				ArrayGenomePtr<T> genome = dynamic_pointer_cast<ArrayGenome<T>>(parent);
				if (!genome)
				throw invalid_argument(
						"All input parents for NPointCrossover must be ArrayGenome-s.");
				if (genome->GetSize() <= mCrossCount)
				throw invalid_argument(
						"All input parents for " + to_string(mCrossCount) +
						"-PointCrossover must have at least " + to_string(mCrossCount + 1) + " genes.");
				return &genome->GetGenes();
			});

	uint minSize = min((*genomes[0]).size(), (*genomes[1]).size());
	vector<uint> crossPoints;
	if (mCrossCount == 1) {
		uniform_int_distribution<uint> dist(1, minSize - 1);
		crossPoints = {0, dist(Random::generator)};
	}
	else {
		crossPoints = vector<uint>(minSize - 1);
		iota(crossPoints.begin(), crossPoints.end(), 1);
		shuffle(crossPoints.begin(), crossPoints.end(), Random::generator);
		crossPoints.erase(crossPoints.begin() + mCrossCount, crossPoints.end());
		sort(crossPoints.begin(), crossPoints.end());
		crossPoints.insert(crossPoints.begin(), 0);
	}

	ArrayGenomePtr<T> newGenome = make_shared<ArrayGenome<T>>();
	vector<T>& newGenes = newGenome->GetGenes();
	uint parent = 0;
	for (uint i = 0; i < mCrossCount; i++) {
		newGenes.insert(newGenes.end(),
				(*genomes[parent]).begin() + crossPoints[i],
				(*genomes[parent]).begin() + crossPoints[i + 1]);

		parent ^= 1;
	}
	newGenes.insert(newGenes.end(),
			(*genomes[parent]).begin() + crossPoints[mCrossCount],
			(*genomes[parent]).end());

	return newGenome;
}

/**
 * Implementation of **N-point crossover** operator for ArrayGenome with specified number of split points.
 * This Recombinator is only applicable on ArrayGenome of the same type of genes T.
 *
 * This is a specialization of NPointCrossover<T> with a specified number of split points.
 * For the details of implementation, see NPointCrossover<T>.
 *
 * There are two aliases for this class, \ref OnePointCrossover for N = 1 and \ref TwoPointCrossover for N = 2.
 *
 * @tparam T Type of genes.
 *
 * @method
 * Draw split points randomly. Then iterate through the array of genes, switch parent if encounter a split point.
 *
 * @time
 * O(L + NlogN) with L is the length of input genomes and N is the number of split points.
 *
 * @name{BoolOnePointCrossover, IntOnePointCrossover, DoubleOnePointCrossover,
 * BoolTwoPointCrossover, IntTwoPointCrossover, DoubleTwoPointCrossover}
 *
 * @note
 * NPointCrossover<T, N> doesn't have any EAML attribute because the number of split points
 * is already specified.
 *
 * @see NPointCrossover<T>
 */
template<class T, uint N>
class NPointCrossover<T, N> : public NPointCrossover<T> {
public:
	EA_TYPEINFO_CUSTOM_DECL

	inline NPointCrossover() :
			NPointCrossover<T>(N) {
	}
	inline virtual ~NPointCrossover() {
	}
};

#ifndef DOXYGEN_IGNORE
EA_TYPEINFO_SPECIALIZED_DEFAULT(BoolOnePointCrossover)
EA_TYPEINFO_SPECIALIZED_DEFAULT(IntOnePointCrossover)
EA_TYPEINFO_SPECIALIZED_DEFAULT(DoubleOnePointCrossover)

EA_TYPEINFO_SPECIALIZED_DEFAULT(BoolTwoPointCrossover)
EA_TYPEINFO_SPECIALIZED_DEFAULT(IntTwoPointCrossover)
EA_TYPEINFO_SPECIALIZED_DEFAULT(DoubleTwoPointCrossover)
#endif

} /* namespace ea */


