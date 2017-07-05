/*
 * OneMaxEvaluator.cpp
 *
 *  Created on: Mar 23, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "SampleOneMaxEvaluator.h"

double ea::addon::SampleOneMaxEvaluator::DoScalarEvaluate(const GenomePtr& pGenome) {
	BoolArrayGenomePtr arrayGenome = dynamic_pointer_cast<BoolArrayGenome>(
			pGenome);
	if (!arrayGenome)
		throw invalid_argument(
				"OneMaxEvaluator requires BoolArrayGenome input.");

	vector<bool>& genes = arrayGenome->GetGenes();

	return std::count(genes.begin(), genes.end(), true);
}
