#pragma once

#include <openea/EA.h>

namespace ea {
namespace addon {

class OneMaxEvaluator: public TypedScalarEvaluator<BoolArrayGenome> {
public:
	EA_TYPEINFO_DEFAULT(OneMaxEvaluator)

protected:
	virtual double DoScalarEvaluate(const BoolArrayGenomePtr& pGenome) override;
};

REGISTER_ADDON(OneMaxEvaluator)

}
}
