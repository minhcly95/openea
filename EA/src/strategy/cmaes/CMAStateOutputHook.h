/*
 * CMAStateOutputHook.h
 *
 *  Created on: Jun 8, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../EA/Type/Core.h"
#include "../../core/interface/Hook.h"

namespace ea {

class CMAStateOutputHook: public Hook {
public:
	EA_TYPEINFO_CUSTOM_DECL

	CMAStateOutputHook(bool pPrintGenome = true);
	virtual ~CMAStateOutputHook();

private:
	bool mPrintGenome;

protected:
	virtual void DoGenerational() override;
};

} /* namespace ea */


