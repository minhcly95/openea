#pragma once

#include <openea/EA.h>

#ifndef DOXYGEN_IGNORE

namespace ea {
namespace so {

//! [Declaration]
class SphereMutation: public TypedMutator<DoubleArrayGenome> {
public:
	EA_TYPEINFO_DEFAULT(SphereMutation)

protected:
	virtual DoubleArrayGenomePtr DoApply(const DoubleArrayGenomePtr& pTarget) override;
};

REGISTER_ADDON(SphereMutation)
//! [Declaration]

}
}

#endif
