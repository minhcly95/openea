/*
 * EvaluationTerminationHook.cpp
 *
 *  Created on: Apr 4, 2017
 *      Author: Bui Quang Minh
 */

#include "../../pch.h"
#include "EvaluationTerminationHook.h"

namespace ea {

EA_TYPEINFO_CUSTOM_IMPL(EvaluationTerminationHook){
	return *ea::TypeInfo("EvaluationTerminationHook")
		.Add("inform", &EvaluationTerminationHook::mInform)
		->SetConstructor<EvaluationTerminationHook, ullong>("target");
}

/**
 * @class EvaluationTerminationHook
 * A Hook which terminates when the target evaluation number has been reached.
 * This Hook also provides a tracker to show the current speed and the estimated remaining time.
 * These functionality is inherited from InformedTerminationHook.
 *
 * @name{EvaluationTerminationHook}
 *
 * @eaml
 * @attr{target, ullong - Required - The target evaluation number.}
 * @attr{inform, bool - Optional - Whether the tracker will be shown or not (default is true).}
 * @endeaml
 */

/**
 * Create an EvaluationTerminationHook with the target evaluation number.
 * @param pTarget The target evaluation number.
 * @param pInform Whether the tracker will be shown or not.
 */
EvaluationTerminationHook::EvaluationTerminationHook(ullong pTarget,
		bool pInform) :
		InformedTerminationHook(pTarget, pInform, "evaluation", "Evl", "e") {
}
EvaluationTerminationHook::~EvaluationTerminationHook() {
}

/**
 * Return the evaluation number.
 * This is the implementation of InformedTerminationHook::GetValue() with the attribute to be the evaluation number.
 * @return The evaluation number.
 */
ullong EvaluationTerminationHook::GetValue() {
	return GetEvaluation();
}

}
/* namespace ea */
