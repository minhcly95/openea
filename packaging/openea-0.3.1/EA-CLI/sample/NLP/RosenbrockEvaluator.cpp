/*
 * RosenbrockEvaluator.cpp
 *
 *  Created on: Jun 07, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "RosenbrockEvaluator.h"

double ea::addon::RosenbrockEvaluator::DoScalarEvaluate(const DoubleArrayGenomePtr& pGenome) {
	vector<double>& x = pGenome->GetGenes();
	double f = 0;
	for (uint i = 0; i < x.size() - 1; i++)
		f += 100 * pow(x[i+1] - x[i]*x[i], 2) + pow(1-x[i], 2);
	return f;
}
