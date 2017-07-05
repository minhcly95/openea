#pragma once

#include <openea/EA.h>

#ifndef DOXYGEN_IGNORE

namespace ea {
namespace so {

//! [Declaration]
class SphereOptEvaluator: public TypedScalarEvaluator<DoubleArrayGenome> {
public:
	EA_TYPEINFO_DEFAULT(SphereOptEvaluator)

protected:
	virtual double DoScalarEvaluate(const DoubleArrayGenomePtr& pGenome) override;
};

REGISTER_ADDON(SphereOptEvaluator)
//! [Declaration]

}
}

#endif
