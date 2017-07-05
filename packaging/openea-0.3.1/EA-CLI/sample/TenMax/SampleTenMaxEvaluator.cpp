/*
 * TenMaxEvaluator.cpp
 *
 *  Created on: Mar 23, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "SampleTenMaxEvaluator.h"

double ea::addon::SampleTenMaxEvaluator::DoScalarEvaluate(const GenomePtr& pGenome) {
	IntArrayGenomePtr arrayGenome = dynamic_pointer_cast<IntArrayGenome>(
			pGenome);
	if (!arrayGenome)
		throw invalid_argument(
				"TenMaxEvaluator requires IntArrayGenome input.");

	vector<int>& genes = arrayGenome->GetGenes();

	return std::accumulate(genes.begin(), genes.end(), 0);
}
