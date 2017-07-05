#include "SphereCrossover.h"
#include "SphereOptVector.h"
#include <cmath>

using namespace std;

#ifndef DOXYGEN_IGNORE

namespace ea {
namespace so {

DoubleArrayGenomePtr SphereCrossover::DoCombine(vector<DoubleArrayGenomePtr>& pParents) {
	DoubleArrayGenomePtr newGenome = make_shared<DoubleArrayGenome>();

	vector<double>& x = pParents[0]->GetGenes();
	vector<double>& y = pParents[1]->GetGenes();
	vector<double>& z = newGenome->GetGenes();

	double alpha = Random::Rate();
	for (uint i = 0; i < x.size(); i++)
		z.push_back(alpha * x[i] + (1 - alpha) * y[i]);
	
	SphereOptVector::Normalize(z);
	
	return newGenome;
}

uint SphereCrossover::GetParentCount() {
	return 2;
}

}
}

#endif
