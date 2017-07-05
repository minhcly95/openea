#pragma once

#include <openea/EA.h>

#ifndef DOXYGEN_IGNORE

namespace ea {
namespace so {

using namespace std;

//! [Declaration]
class SphereCrossover: public TypedRecombinator<DoubleArrayGenome> {
public:
	EA_TYPEINFO_DEFAULT(SphereCrossover)

protected:
	virtual DoubleArrayGenomePtr DoCombine(vector<DoubleArrayGenomePtr>& pParents) override;
	virtual uint GetParentCount() override;
};

REGISTER_ADDON(SphereCrossover)
//! [Declaration]

}
}

#endif
