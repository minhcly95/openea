#include "SphereMutation.h"
#include "SphereOptVector.h"
#include <cmath>

using namespace std;

#ifndef DOXYGEN_IGNORE

namespace ea {
namespace so {

DoubleArrayGenomePtr SphereMutation::DoApply(const DoubleArrayGenomePtr& pTarget) {
	DoubleArrayGenomePtr newGenome = pTarget->Clone();
	pair<uint, uint> pair = Random::Pair<uint>(0, pTarget->GetSize() - 1);

	vector<double>& x = newGenome->GetGenes();
	uint i = pair.first, j = pair.second;
	double p = Random::Rate();
	double q = sqrt(pow(x[i] / x[j], 2) * (1 - p * p) + 1);
	x[i] *= p;
	x[j] *= q;

	SphereOptVector::Normalize(x);
	
	return newGenome;
}

}
}

#endif
