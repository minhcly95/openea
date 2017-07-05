/*
 * GenerationTerminationHook.cpp
 *
 *  Created on: Apr 4, 2017
 *      Author: Bui Quang Minh
 */

#include "../../pch.h"
#include "GenerationTerminationHook.h"

namespace ea {

EA_TYPEINFO_CUSTOM_IMPL(GenerationTerminationHook){
	return *ea::TypeInfo("GenerationTerminationHook")
		.Add("inform", &GenerationTerminationHook::mInform)
		->SetConstructor<GenerationTerminationHook, ullong>("target");
}

/**
 * @class GenerationTerminationHook
 * A Hook which terminates when the target generation number has been reached.
 * This Hook also provides a tracker to show the current speed and the estimated remaining time.
 * These functionality is inherited from InformedTerminationHook.
 *
 * @name{GenerationTerminationHook}
 *
 * @eaml
 * @attr{target, ullong - Required - The target generation number.}
 * @attr{inform, bool - Optional - Whether the tracker will be shown or not (default is true).}
 * @endeaml
 */

/**
 * Create an GenerationTerminationHook with the target generation number.
 * @param pTarget The target generation number.
 * @param pInform Whether the tracker will be shown or not.
 */
GenerationTerminationHook::GenerationTerminationHook(ullong pTarget,
		bool pInform) :
		InformedTerminationHook(pTarget, pInform, "generation", "Gen", "g") {
}
GenerationTerminationHook::~GenerationTerminationHook() {
}

/**
 * Return the generation number.
 * This is the implementation of InformedTerminationHook::GetValue() with the attribute to be the generation number.
 * @return The generation number.
 */
ullong GenerationTerminationHook::GetValue() {
	return GetGeneration();
}

}
/* namespace ea */
