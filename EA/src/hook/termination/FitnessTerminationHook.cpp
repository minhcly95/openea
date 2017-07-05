/*
 * TerminationHook.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: Bui Quang Minh
 */

#include "../../pch.h"
#include "FitnessTerminationHook.h"
#include "../../core/Organism.h"
#include "../../core/pool/OrganismPool.h"

namespace ea {

using namespace std;

#ifndef DOXYGEN_IGNORE
EA_DEFINE_CUSTOM_SERIALIZER(FitnessTerminationHook::Condition, data, ss) {
	static HashMap<string, FitnessTerminationHook::Condition> strToCond = {
		{"eq",  FitnessTerminationHook::EQUAL_TO},
		{"equ",  FitnessTerminationHook::EQUAL_TO},
		{"equal",  FitnessTerminationHook::EQUAL_TO},
		{"equal-to",  FitnessTerminationHook::EQUAL_TO},
		{"=",  FitnessTerminationHook::EQUAL_TO},
		{"==",  FitnessTerminationHook::EQUAL_TO},

		{"lt",  FitnessTerminationHook::LESS_THAN},
		{"lss",  FitnessTerminationHook::LESS_THAN},
		{"less",  FitnessTerminationHook::LESS_THAN},
		{"less-than",  FitnessTerminationHook::LESS_THAN},
		{"<",  FitnessTerminationHook::LESS_THAN},

		{"le",  FitnessTerminationHook::LESS_THAN_OR_EQUAL_TO},
		{"leq",  FitnessTerminationHook::LESS_THAN_OR_EQUAL_TO},
		{"less-equal",  FitnessTerminationHook::LESS_THAN_OR_EQUAL_TO},
		{"less-or-equal",  FitnessTerminationHook::LESS_THAN_OR_EQUAL_TO},
		{"less-than-or-equal-to",  FitnessTerminationHook::LESS_THAN_OR_EQUAL_TO},
		{"<=",  FitnessTerminationHook::LESS_THAN_OR_EQUAL_TO},

		{"gt",  FitnessTerminationHook::GREATER_THAN},
		{"gtr",  FitnessTerminationHook::GREATER_THAN},
		{"greater",  FitnessTerminationHook::GREATER_THAN},
		{"greater-than",  FitnessTerminationHook::GREATER_THAN},
		{">",  FitnessTerminationHook::GREATER_THAN},

		{"ge",  FitnessTerminationHook::GREATER_OR_EQUAL_TO},
		{"geq",  FitnessTerminationHook::GREATER_OR_EQUAL_TO},
		{"greater-equal",  FitnessTerminationHook::GREATER_OR_EQUAL_TO},
		{"greater-or-equal",  FitnessTerminationHook::GREATER_OR_EQUAL_TO},
		{"greater-than-or-equal-to",  FitnessTerminationHook::GREATER_OR_EQUAL_TO},
		{">=",  FitnessTerminationHook::GREATER_OR_EQUAL_TO},
	};

	string str;
	if (!bool(ss >> str))
		return false;

	auto itr = strToCond.find(str);
	if (itr == strToCond.end())
		return false;

	data = itr->second;
	return true;
}
#endif

EA_TYPEINFO_CUSTOM_IMPL(FitnessTerminationHook) {
	return *ea::TypeInfo("FitnessTerminationHook")
		.Add("re-sort", &FitnessTerminationHook::mResort)
		->SetConstructor<FitnessTerminationHook, Condition, double>("condition", "threshold");
}

/**
 * @class FitnessTerminationHook
 * A Hook which will terminate the Population based on a fitness condition.
 * The Hook will check the best fitness value of the population with the given condition. If it is satisfied,
 * the evolution process will be terminated.
 *
 * See FitnessTerminationHook::Condition for available conditions.
 *
 * @name{FitnessTerminationHook}
 *
 * @eaml
 * @attr{condition, FitnessTerminationHook::Condition - Required - The condition to terminate.}
 * @attr{threshold, double - Required - The target fitness.}
 * @attr{re-sort, bool - Optional - If true\, the Population will be sorted (default).}
 * @endeaml
 */

/**
 * Create a FitnessTerminationHook with the given condition and target fitness.
 * @param pCondition The condition to terminate.
 * @param pThreshold The target fitness.
 * @param pResort If true, the Population will be sorted. Users should set this to false if they know
 * the Population has been sorted before (e.g. StandardOutputHook has been executed).
 */
FitnessTerminationHook::FitnessTerminationHook(Condition pCondition,
		double pThreshold, bool pResort) :
		mCondition(pCondition), mThreshold(pThreshold), mResort(pResort) {
}

FitnessTerminationHook::~FitnessTerminationHook() {
}

void FitnessTerminationHook::DoGenerational() {
	if (mResort)
		SortMainPool();
	double maxFitness = GetMainPool()->at(0)->GetFitnessValue();

#define DEBUG_MES(SIGN)\
		EA_LOG_DEBUG << "FitnessTerminationHook: Reached fitness value = "\
				<< maxFitness << " " SIGN " " << mThreshold << ". Terminating..." << flush

	if ((int(mCondition) & 1) && maxFitness == mThreshold) {
		DEBUG_MES("==");
		Terminate();
	}

	if ((int(mCondition) & 2) && maxFitness < mThreshold) {
		DEBUG_MES("<");
		Terminate();
	}

	if ((int(mCondition) & 4) && maxFitness > mThreshold) {
		DEBUG_MES(">");
		Terminate();
	}
}

} /* namespace ea */
