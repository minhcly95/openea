/*
 * NameService.cpp
 *
 *  Created on: Mar 23, 2017
 *      Author: Bui Quang Minh
 */

#include "../pch.h"
#include "NameService.h"
#include "../EA.h"

namespace ea {

/**
 * @class NameService
 * Static class which provides a service to translate class name into TypeInfo.
 * This class is the entry point to construct any object during run-time.
 * It provides a dictionary to translate class name of type string to TypeInfo,
 * and from there a real object can be constructed. All functions of this class
 * are static, so the NameService can be accessed everywhere.
 *
 * Before translating any name, users must add the class to the NameService first
 * by using Add() function. Then simply calling Get(), users can get the TypeInfo
 * of a given class name. Built-in types in the library are added by default for convenience.
 */

NameService::NameMap NameService::sMap;
NameService::InitStruct NameService::__init;

/**
 * Translate the given class name into TypeInfo.
 * Note that the class name must be added to the NameService before by calling Add() function.
 * Otherwise an exception will be thrown.
 * @param pName The class name to be translated.
 * @return The translated TypeInfo.
 */
const TypeInfo& NameService::Get(string pName) {
	auto entry = sMap.find(pName);
	if (entry == sMap.end())
		throw EA_EXCEPTION(RTOCException, CLASS_NAME_NOT_EXIST,
				pName + " class does not exist in Name service.");

	return entry->second;
}

/**
 * @fn void NameService::Add<T>()
 * Add the type T to the NameService.
 * This function automatically gets the TypeInfo by calling Constructible::GetTypeInfo() and adds it using Add(TypeInfo) function.
 * @tparam T Must be a child of Constructible and Constructible::GetTypeInfo() has been overriden (or using type info macros).
 */

/**
 * Add a TypeInfo to the NameService.
 * Add the given TypeInfo to the NameService. The class name is automatically get from the TypeInfo.
 *
 * An alternative is Add<T>() function which only requires only the type as template parameter and no TypeInfo.
 * @param pTypeInfo The TypeInfo to be added.
 */
void NameService::Add(const TypeInfo pTypeInfo) {
	auto entry = sMap.find(pTypeInfo.GetTypeName());
	if (entry == sMap.end())
		sMap.emplace(pTypeInfo.GetTypeName(), pTypeInfo);
	else
		throw EA_EXCEPTION(RTOCException, CLASS_NAME_REGISTERED,
				pTypeInfo.GetTypeName() + " class already exists in Name service.");
}

#define ADD(CLASS) NameService::Add<CLASS>()
#define ADD_PACK(CLASS)\
	ADD(Bool ## CLASS);\
	ADD(Int ## CLASS);\
	ADD(Double ## CLASS)

NameService::InitStruct::InitStruct() {
	// Core
	ADD(OrganismPool);
	ADD(GenomePool);
	ADD(PhenomePool);
	ADD(MetaPool);

	// Initializer
	ADD_PACK(RandomArrayInitializer);
	ADD(PermutationInitializer);

	// Randomizer
	ADD_PACK(Randomizer);

	// Mutation
	ADD_PACK(PointResetMutation);
	ADD(InsertMutation);
	ADD(InversionMutation);
	ADD(ScrambleMutation);
	ADD(SwapMutation);

	// Recombinator
	ADD_PACK(OnePointCrossover);
	ADD_PACK(TwoPointCrossover);
	ADD_PACK(NPointCrossover);
	ADD_PACK(UniformCrossover);
	ADD(CycleCrossover);
	ADD(EdgeCrossover);
	ADD(OrderCrossover);
	ADD(PartiallyMappedCrossover);

	// Selection
	ADD(GreedySelection);
	ADD(UniformSelection);
	ADD(TournamentSelection);

	// Hooks
	ADD(StandardOutputHook);
	ADD(FitnessTerminationHook);
	ADD(GenerationTerminationHook);
	ADD(EvaluationTerminationHook);
	ADD(BackupHook);
	ADD(RealTimeInfoHook);
	ADD(FitnessReportHook);
	ADD(CyclicHook);

	// Genome
	ADD(BoolArrayGenome);
	ADD(IntArrayGenome);
	ADD(DoubleArrayGenome);
	ADD(PermutationGenome);

	// Fitness
	ADD(ScalarFitness);
	ADD(PoweredFitness);

	// Strategy
	ADD(EvolutionStrategy);
	ADD(CMAEvolutionStrategy);
	ADD(CMAStatePool);
	ADD(CMAStateOutputHook);

	// Utility
	ADD(MetaRecombinator);
	ADD(MetaMutator);
}

/**
 * Get all class names stored in the NameService.
 * This function is useful when debugging and also for supporting the GUI.
 * @return A list of class names currently stored in the NameService.
 */
vector<string> NameService::GetAllNames() {
	vector<string> allNames;
	for (auto entry : sMap)
		allNames.push_back(entry.first);
	return allNames;
}

} /* namespace ea */

