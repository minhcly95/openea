/*
 * EightQueenEvaluator.cpp
 *
 *  Created on: April 12, 2017
 *      Author: Kiet Vo
 */

#include <pch.h>
#include "SampleEightQueenEvaluator.h"

using namespace ea;
using namespace ea::addon;

bool IsDiagonallyChecked(vector<uint>& genes, uint index1, uint index2) {
	// cast uint to int
	int diff1 = index1 - index2;
	int diff2 = genes[index1] - genes[index2];

	return abs(diff1) == abs(diff2);
}

double OldVersion(SampleEightQueenEvaluator::InputType pGenome) {
	vector<uint>& genes = pGenome->GetGenes();

	double count = 0;
	for (uint i = 0; i < genes.size() - 1; i++)
	for (uint j = i + 1; j < genes.size(); j++)
	count += IsDiagonallyChecked(genes, i, j);

	return count;
}

double NewVersion(SampleEightQueenEvaluator::InputType pGenome) {
	vector<uint>& genes = pGenome->GetGenes();
	uint size = genes.size();

	// Bucket method, inspired by bucket sort = linear complexity

	// Buckets
	vector<uint> mainDiagonal(size * 2 - 1, 0);		// Indexed by i + j
	vector<uint> offDiagonal(size * 2 - 1, 0);		// Indexed by (size - 1 - i) + j

	// Put to buckets
	for (uint i = 0; i < size; i++) {
		uint j = genes[i];
		mainDiagonal[i + j]++;
		offDiagonal[size - 1 - i + j]++;
	}

	// Count the buckets
	uint count = 0;
	for (uint b = 0; b < 2 * size - 1; b++) {
		count += mainDiagonal[b] * (mainDiagonal[b] - 1) / 2;
		count += offDiagonal[b] * (offDiagonal[b] - 1) / 2;
	}

	return count;
}

double ea::addon::SampleEightQueenEvaluator::DoScalarEvaluate(InputType pGenome) {
	if (mOldVersion)
		return OldVersion(pGenome);
	else
		return NewVersion(pGenome);
}
