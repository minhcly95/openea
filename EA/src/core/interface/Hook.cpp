/*
 * Hook.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: Bui Quang Minh
 */

#include "../../pch.h"
#include "Hook.h"

#include "../pool/OrganismPool.h"
#include "../Population.h"
#include "../Session.h"

namespace ea {

/**
 * @class Hook
 * The interface for custom code execution.
 * Hook is a mechanism allowing custom code can be added to the evolution process.
 * Hook is executed at specific points such as after initialization, before evolution,
 * each generation or at the end of evolution; and provides information of the Population
 * (which is normally private and cannot be accessed easily). Example of using Hook is to provide
 * output on each generation, generate backup or even termination condition.
 *
 * To use Hook, users simply need to override DoGenerational(), DoInitial(), DoStart() or DoEnd() methods.
 * It is not required to override all of them. For details, check the description of each function.
 *
 * Hook is compatible with Operator and OperatorGroup. It takes no input and produces no output.
 */

Hook::Hook() : mPopulation(nullptr) {
}

Hook::~Hook() {
}

#define HOOK_FUNC(NAME)\
	void Hook::NAME() {\
		try {\
			Do##NAME();\
		} catch (exception& e) {\
			throw EA_EXCEPTION(EAException, HOOK_ERROR,\
					"Hook::" #NAME ": Exception caught when executing Hook.", current_exception());\
		}\
	}

/**
 * @fn void Hook::Generational()
 * Public method of DoGenerational().
 * This function handles the exception and calls DoGenerational() internally.
 */
/**
 * @fn void Hook::DoGenerational()
 * Custom code implementation to be run on each generation.
 * If child class overrides this function, that segment of code will be run each generation,
 * after the filtering stage (survival Selector is applied).
 *
 * Useful applications are printing best fitness value, termination condition and saving backup.
 * @see Population::Evolve()
 * @see StandardOutputHook
 * @see FitnessTerminationHook
 */
HOOK_FUNC(Generational)
/**
 * @fn void Hook::Initial()
 * Public method of DoInitial().
 * This function handles the exception and calls DoInitial() internally.
 */
/**
 * @fn void Hook::DoInitial()
 * Custom code implementation to be run when the Population is initialized.
 * If child class overrides this function, that segment of code will be run when the Population is initialized,
 * specifically after Population::Initialize() is called (or implicitly called by Population::Evolve()).
 *
 * Useful applications are preparing file system and initializing Hook data.
 * @see Population::Initialize()
 * @see Population::Evolve()
 * @see BackupHook
 */
HOOK_FUNC(Initial)
/**
 * @fn void Hook::Start()
 * Public method of DoStart().
 * This function handles the exception and calls DoStart() internally.
 */
/**
 * @fn void Hook::DoStart()
 * Custom code implementation to be run before the evolution process.
 * If child class overrides this function, that segment of code will be run right after Population::Evolve() is invoked
 * and before any evolution process.
 *
 * Useful applications are saving the begin generation and evaluation number or the begin time of evolution process.
 * @see Population::Evolve()
 * @see InformedTerminationHook
 */
HOOK_FUNC(Start)
/**
 * @fn void Hook::End()
 * Public method of DoEnd().
 * This function handles the exception and calls DoEnd() internally.
 */
/**
 * @fn void Hook::DoEnd()
 * Custom code implementation to be run after the evolution process.
 * If child class overrides this function, that segment of code will be run right before Population::Evolve() is returned,
 * after the evolution process has finished or been terminated.
 *
 * Useful applications are saving last generation backup and printing out statistics.
 * @see Population::Evolve()
 * @see BackupHook
 */
HOOK_FUNC(End)

/**
 * Get the population size of the OrganismPool #0.
 * Error will be thrown if the Pool #0 is not an OrganismPool.
 * @return The pool size of OrganismPool #0.
 */
uint Hook::GetSize() const {
	return GetMainPool()->size();
}
/**
 * Get the current generation number of the Population.
 * @return The current generation number.
 */
ullong Hook::GetGeneration() const {
	return mPopulation->GetGeneration();
}
/**
 * Get the current evaluation number of the Population.
 * @return The current evaluation number.
 */
ullong Hook::GetEvaluation() const {
	return mPopulation->GetEvaluation();
}
/**
 * Get the OrganismPool #0 of the Population.
 * Error will be thrown if the Pool #0 is not an OrganismPool.
 * @return The OrganismPool #0.
 */
OrganismPoolPtr Hook::GetMainPool() const {
	return mPopulation->GetOrganismPool(0);
}
/**
 * Sort the OrganismPool #0 of the Population.
 * If querying the best Organism is required, the OrganismPool must be sorted by using this function.
 * The order of the Organism is determined by the type of Fitness. This function is equivalent to:
 *
 * @code
 * Hook::GetMainPool()->Sort()
 * @endcode
 *
 * Error will be thrown if the Pool #0 is not an OrganismPool.
 *
 * @see Fitness::Compare()
 */
void Hook::SortMainPool() {
	GetMainPool()->Sort();
}

/**
 * Get the best Organism of the OrganismPool #0.
 * This function will sort the OrganismPool #0 and then query the first Organism from it. After calling
 * this function, the OrganismPool is guaranteed to be sorted.
 * The order of the Organism is determined by the type of Fitness.
 *
 * Error will be thrown if the Pool #0 is not an OrganismPool.
 *
 * @return The best Organism.
 */
const OrganismPtr& Hook::GetBestOrganism() {
	SortMainPool();
	return GetMainPool()->at(0);
}

/**
 * Terminate the Session.
 * This function will call Session::Terminate() internally.
 *
 * @see Session::Terminate()
 *
 */
void Hook::Terminate() {
	mSession->Terminate();
}

/**
 * Whether the current Session is running or is terminated.
 * If Terminate() has been called, this function will return false even if the last generation
 * is not yet finished.
 *
 * @return true if the Session was started and Terminate() has not been called yet.
 */
bool Hook::IsRunning() {
	return mSession->IsRunning();
}

/**
 * Invocation entry for Operator and OperatorGroup.
 * This function will call the hook function which is identified by the given function pointer.
 * @param pSession The current running Session (given automatically by Operator).
 * @param pFunc The function te be executed (Initial(), Start(), Generational() or End()).
 */
void Hook::operator ()(const SessionPtr& pSession, void (Hook::*pFunc)()) {
	mSession = pSession;
	mPopulation = pSession->GetPopulation();

	(this->*pFunc)();

	mSession = nullptr;
	mPopulation = nullptr;
}

/**
 * Get the Population object set by the context Session.
 * This function is equivalent to @tt{GetSession()->GetPopulation()}.
 * @return The Population object of the current running Session.
 */
const PopulationPtr& Hook::GetPopulation() {
	return mPopulation;
}

/**
 * Get the Session running context.
 * This Session object is only available when Hook is invoked via an Operator of OperatorGroup.
 * Otherwise, this function returns nullptr.
 * @return The current Session running context.
 */
const SessionPtr& Hook::GetSession() {
	return mSession;
}

} /* namespace ea */
