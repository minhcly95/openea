/*
 * H1BenchmarkEvaluator.cpp
 *
 *  Created on: Jun 08, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "H1BenchmarkEvaluator.h"

double ea::addon::H1BenchmarkEvaluator::DoScalarEvaluate(const DoubleArrayGenomePtr& pGenome) {
	vector<double>& x = pGenome->GetGenes();
	if (x.size() != 2)
		throw EA_EXCEPTION(EAException, OTHERS, "H1 benchmark function requires Genome of size 2.");
	return (pow(sin(x[0]-x[1]/8), 2) + pow(sin(x[1]+x[0]/8), 2)) /
			(sqrt(pow(x[0]-8.6998, 2) + pow(x[1]-6.7665, 2)) + 1);
}
