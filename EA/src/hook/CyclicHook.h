/*
 * CyclicHook.h
 *
 *  Created on: Jul 3, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../EA/Type/Core.h"
#include "../core/interface/Hook.h"
#include "../rtoc/Constructible.h"

namespace ea {

class CyclicHook : public Hook {
public:
	EA_TYPEINFO_CUSTOM_DECL

	CyclicHook(HookPtr innerHook, uint pFrequency);
	virtual ~CyclicHook();

protected:
	virtual void DoGenerational() override;
	virtual void DoEnd() override;

private:
	HookPtr mInner;
	uint mFrequency;
};

} /* namespace ea */
