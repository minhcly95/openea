#include "SphereOptVector.h"
#include <cmath>

using namespace std;

#ifndef DOXYGEN_IGNORE

namespace ea {
namespace so {

void SphereOptVector::Normalize(vector<double>& x) {
	double norm = 0;
	for (uint i = 0; i < x.size(); i++)
		norm += x[i] * x[i];
	norm = sqrt(norm);

	for (uint i = 0; i < x.size(); i++)
		x[i] /= norm;
}

}
}

#endif
