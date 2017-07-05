#include "SphereOptEvaluator.h"

using namespace std;

#ifndef DOXYGEN_IGNORE

namespace ea {
namespace so {

//! [Definition]
double SphereOptEvaluator::DoScalarEvaluate(const DoubleArrayGenomePtr& pGenome) {
	vector<double>& x = pGenome->GetGenes();
	uint n = x.size();
	return std::accumulate(x.begin(), x.end(), pow(n, n / 2.0), std::multiplies<double>());
}
//! [Definition]

}
}

#endif
