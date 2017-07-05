/*
 * StandardOutputHook.h
 *
 *  Created on: Apr 3, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../EA/Type/Core.h"
#include "../core/interface/Hook.h"

namespace ea {

class StandardOutputHook: public Hook {
public:
	EA_TYPEINFO_CUSTOM_DECL

	StandardOutputHook(bool pPrintGenome = true);
	virtual ~StandardOutputHook();

private:
	bool mPrintGenome;

protected:
	virtual void DoGenerational() override;
};

} /* namespace ea */


