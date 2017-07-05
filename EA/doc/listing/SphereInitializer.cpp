#include "SphereInitializer.h"
#include "SphereOptVector.h"
#include <cmath>

using namespace std;

#ifndef DOXYGEN_IGNORE

namespace ea {
namespace so {

//! [TypeInfo]
EA_TYPEINFO_CUSTOM_IMPL(SphereInitializer) {
	return *ea::TypeInfo("SphereInitializer").
			SetConstructor<SphereInitializer, uint>("length");
}
//! [TypeInfo]

SphereInitializer::SphereInitializer(uint pLength) :
	mLength(pLength) { }

GenomePoolPtr SphereInitializer::DoInitialize(uint pSize) {
	GenomePoolPtr pool = make_shared<GenomePool>();

	for (uint i = 0; i < pSize; i++) {
		DoubleArrayGenomePtr newGenome = make_shared<DoubleArrayGenome>();
		vector<double>& x = newGenome->GetGenes();

		for (uint i = 0; i < mLength; i++)
			x.push_back(Random::Rate());
		
		SphereOptVector::Normalize(x);

		pool->push_back(static_pointer_cast<Genome>(newGenome));
	}

	return pool;
}

}
}

#endif
