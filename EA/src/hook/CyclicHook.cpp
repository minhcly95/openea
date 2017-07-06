/*
 * CyclicHook.cpp
 *
 *  Created on: Jul 3, 2017
 *      Author: Bui Quang Minh
 */

#include "../pch.h"
#include "CyclicHook.h"

namespace ea {

EA_TYPEINFO_CUSTOM_IMPL(CyclicHook) {
	return *ea::TypeInfo("CyclicHook").
		SetConstructor<CyclicHook, HookPtr, uint>("inner", "frequency");
}

CyclicHook::CyclicHook(HookPtr innerHook, uint pFrequency) :
	mInner(innerHook), mFrequency(pFrequency) {
}

CyclicHook::~CyclicHook() {
}

void CyclicHook::DoGenerational() {
	if (mFrequency > 1 && GetGeneration() % mFrequency > 0)
		return;

	(*mInner)(GetSession(), &Hook::Generational);
}

void CyclicHook::DoEnd() {
	if (mFrequency > 1 && GetGeneration() % mFrequency > 0)
		(*mInner)(GetSession(), &Hook::Generational);

	(*mInner)(GetSession(), &Hook::End);
}

} /* namespace ea */
