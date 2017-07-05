#pragma once

#include <openea/EA.h>

#ifndef DOXYGEN_IGNORE

namespace ea {
namespace so {

//! [Declaration]
class SphereInitializer: public Initializer {
public:
	EA_TYPEINFO_CUSTOM_DECL

	SphereInitializer(uint pLength);

protected:
	virtual GenomePoolPtr DoInitialize(uint pSize) override;

private:
	uint mLength;
};

REGISTER_ADDON(SphereInitializer)
//! [Declaration]

}
}

#endif
