/*
 * Strategy.cpp
 *
 *  Created on: May 18, 2017
 *      Author: Bui Quang Minh
 */

#include "../../pch.h"
#include "Strategy.h"
#include "../../EA/Core.h"

namespace ea {

/**
 * @class Strategy
 * The interface for execution order (algorithm).
 *
 * This class provides the interface to implement custom call order or algorithm.
 * To start an EA, users need to create an instance of Strategy implementation
 * and call the Evolve() function.
 * If users want to use an implemented trivial Strategy, please take a look at DefaultStrategy.
 *
 * To define a custom strategy, first, operators must be defined by using the wrapper as
 * Operator, OperatorGroup and SeriesOperatorGroup.
 * Then, users need to override four functions:
 * - Setup(): Run when no Population is provided when Evolve() is called.
 * - Begin(): Run before the evolution starts.
 * - Loop(): Run each generation.
 * - End(): Run after the evolution ends.
 *
 * In those functions, users manipulate the Pool get by GetPool(), using the
 * defined Operator and OperatorGroup via Execute function. Finally, set the Pool back to
 * the Population by using SetPool().
 *
 * Every Strategy already has an OperatorGroup of Hook named @ref hooks. Users can re-use this group
 * or define for themselves a new group of Hook.
 *
 * Strategy interface is Constructible, and is the root element of an EAML file.
 * This means users can define their own EAML template corresponding to their custom-defined
 * Strategy, including the names of the operators and their types.
 *
 * @see DefaultStrategy
 *
 * @see Operator
 * @see OperatorGroup
 * @see SeriesOperatorGroup
 */

/**
 * @var OperatorGroup<Hook> Strategy::hooks
 * The global OperatorGroup of Hook.
 * Functions of the Hook added to this group will be called after the corresponding Strategy methods are called:
 * - Hook::Initial(): after Setup()
 * - Hook::Start(): after Begin()
 * - Hook::Generational(): after Loop()
 * - Hook::End(): after End()
 */

/**
 * @fn bool Strategy::IsReady()
 * Check the readiness of the Strategy.
 * If users want to perform some checks before strating the evolution process, they can override this function
 * and return a predicate whether the Strategy is ready or not. The default implementation is always true.
 * @return Whether the Strategy is ready. Default implementation is always true.
 */

/**
 * @fn void Strategy::Setup()
 * Procedure to initialize a new Population.
 * This function needs to be overridden in child classes. This function will be invoked when Evolve() is called
 * with no Population object. In that case, a new Population will be created and this function will initialize
 * that Population.
 */

/**
 * @fn void Strategy::Begin()
 * Procedure executed before the evolution starts.
 * This function doesn't need to be overridden in child classes. This function will be called once when users call Evolve(),
 * before the loop starts, no matter if a Population is given or not.
 * If no Population is given, Begin() will be executed after Setup().
 */

/**
 * @fn void Strategy::Loop()
 * Procedure executed each generation.
 * This function needs to be overridden in child classes. This function is the main procedure of the Strategy and
 * will be invoked each generation. Users should implement the regular evolutionary process here.
 */

/**
 * @fn void Strategy::End()
 * Procedure executed after the evolution ends.
 * This function doesn't need to be overridden in child classes. This function is the counterpart of Begin() and
 * will be called once before Evolve() is returned.
 */

Strategy::Strategy() : hooks(), mPopulation(nullptr) {
}

Strategy::~Strategy() { }

/**
 * Use this Strategy to evolve the given Population.
 * This function is similar to Evolve(PopulationPtr) but the Session object is returned to
 * the given reference pointer. This function is used when users need to access to the current running Session
 * immediately without waiting until the end of the evolution
 * (by using another thread for example, because Evolve() functions are blocking).
 *
 * @param pSession The reference to a pointer of Session. This will contain the returned Session.
 * @param pPopulation The Population will be evolved. If it is not provided, a new one will be created and initialized.
 */
void Strategy::Evolve(SessionPtr& pSession, PopulationPtr pPopulation) {
	// Signal attachment
	sCurrent = this;

	struct sigaction sigs;
	sigs.sa_handler = Strategy::SignalHandler;

	sigaction(SIGINT, &sigs, NULL);
	sigaction(SIGTERM, &sigs, NULL);

	// Initializtion
	Cluster::Deploy();

	if (pPopulation == nullptr) {
		mPopulation = make_shared<Population>();
		pSession = mSession = make_shared<Session>(mPopulation, static_pointer_cast<Strategy>(shared_from_this()));

		Setup();

		EA_LOG_TRACE << "Global hooks Initial" << flush;
		hooks.InParallel(mSession, &Hook::Initial);

		EA_LOG_DEBUG<< "Population initialized, evaluation " << GetPopulation()->GetEvaluation() << flush;
	}
	else {
		mPopulation = pPopulation;
		pSession = mSession = make_shared<Session>(mPopulation, static_pointer_cast<Strategy>(shared_from_this()));
	}

	// Evolution process
	Begin();

	EA_LOG_TRACE << "Global hooks Start" << flush;
	hooks.InParallel(mSession, &Hook::Start);

	EA_LOG_DEBUG<< "Evolution started from generation " << GetPopulation()->GetGeneration()
			<< ", evaluation " << GetPopulation()->GetEvaluation() << flush;

	while (mSession->IsRunning()) {
		GetPopulation()->IncreaseGeneration();
		mSession->MeasureTotalTime(bind(&Strategy::Loop, this));

		EA_LOG_TRACE << "Gen " << GetPopulation()->GetGeneration() << ", global hooks Generational" << flush;
		ExecuteInParallel("H", hooks, &Hook::Generational);
	}

	End();

	EA_LOG_TRACE << "Global hooks End" << flush;
	hooks.InParallel(mSession, &Hook::End);

	EA_LOG_DEBUG<< "Evolution stopped at generation " << GetPopulation()->GetGeneration()
			<< ", evaluation " << GetPopulation()->GetEvaluation() << flush;

	// Time report
	LogStream timeStr(Log::DEBUG);
	timeStr << "Time Report:";
	auto records = GetSession()->GetTimeRecords(GetTimeRecordOrder());
	for (auto record : records)
		timeStr << " [" << record.first << "] " << record.second;
	timeStr << " [Total] " << GetSession()->GetTotalTime() << " ms " << flush;

	// Prevent cyclic reference
	mPopulation = nullptr;
	mSession = nullptr;

	sCurrent = nullptr;
}
/**
 * Use this Strategy to evolve the given Population.
 * A Session object corresponding to the run will be returned.
 * If no Population is given, a new one will be created and Setup() will be called.
 *
 * @param pPopulation The Population will be evolved. If it is not provided, a new one will be created and initialized.
 * @return The Session object of the run.
 */
SessionPtr Strategy::Evolve(PopulationPtr pPopulation) {
	SessionPtr session;
	Evolve(session, pPopulation);
	return session;
}

/**
 * Store the given Pool to the processing Population.
 * @param pIndex The index of the Pool.
 * @param pPool The Pool to be set.
 */
void Strategy::SetPool(uint pIndex, const PoolPtr& pPool) {
	mPopulation->SetPool(pIndex, pPool);
}
/**
 * Store the given GenomePool to the processing Population.
 * This function is similar to SetPool() with automatic casting to GenomePool.
 * @param pIndex The index of the Pool.
 * @param pPool The GenomePool to be set.
 */
void Strategy::SetGenomePool(uint pIndex, const GenomePoolPtr& pPool) {
	SetPool(pIndex, static_pointer_cast<Pool>(pPool));
}
/**
 * Store the given OrganismPool to the processing Population.
 * This function is similar to SetPool() with automatic casting to OrganismPool.
 * @param pIndex The index of the Pool.
 * @param pPool The OrganismPool to be set.
 */
void Strategy::SetOrganismPool(uint pIndex, const OrganismPoolPtr& pPool) {
	SetPool(pIndex, static_pointer_cast<Pool>(pPool));
}

/**
 * Get a Pool stored in the processing Population.
 * @param pIndex The index of the Pool.
 * @return The wanted Pool. If there's no Pool at that index, nullptr is retuned.
 */
PoolPtr Strategy::GetPool(uint pIndex) {
	return mPopulation->GetPool(pIndex);
}
/**
 * Get a GenomePool stored in the processing Population.
 * This function is similar to GetPool() with automatic casting to GenomePool.
 * @param pIndex The index of the Pool.
 * @return The wanted GenomePool. If there's no Pool at that index, nullptr is retuned.
 * If the Pool exists but is not a GenomePool, exception will be thrown.
 */
GenomePoolPtr Strategy::GetGenomePool(uint pIndex) {
	return mPopulation->GetGenomePool(pIndex);
}
/**
 * Get a OrganismPool stored in the processing Population.
 * This function is similar to GetPool() with automatic casting to OrganismPool.
 * @param pIndex The index of the Pool.
 * @return The wanted OrganismPool. If there's no Pool at that index, nullptr is retuned.
 * If the Pool exists but is not a OrganismPool, exception will be thrown.
 */
OrganismPoolPtr Strategy::GetOrganismPool(uint pIndex) {
	return mPopulation->GetOrganismPool(pIndex);
}

/**
 * Get the processing Population object.
 * @return The processing Population.
 */
const PopulationPtr& Strategy::GetPopulation() const {
	return mPopulation;
}

/**
 * Get the current running Session.
 * @return The current Session.
 */
const SessionPtr& Strategy::GetSession() const {
	return mSession;
}

/**
 * @fn vector<string> Strategy::GetTimeRecordOrder()
 * Get the order of the time records displayed at the end of the Session.
 * By default, the order follows the alphabet order. All operator IDs will be printed out.
 * If users want to have a different time records order or omit some IDs,
 * they can override this function and implement a custom one.
 * This order will be used in the Time Records log, printed at the end of the Session.
 * @return A list of operator IDs representing the order of the records. Empty list means using the default setting.
 */

/**
 * @fn ReturnT Strategy::Execute(string pId, function<ReturnT(Args&&...)> pFunc, Args&&... args)
 * Execute the given function with the corresponding arguments under the given operator ID.
 * This function will execute the function and measure the time of the execution.
 * This is an alias version of Session::Measure().
 *
 * @tparam ReturnT The return type of the function (auto-deduced).
 * @tparam Args The type of the arguments (auto-deduced).
 * @param pId The operator ID, which is used to track the identity of the operator.
 * @param pFunc The function to be executed.
 * @param args The arguments to be forwarded to the function.
 * @return The return value of the given function.
 */

/**
 * @fn OpT::OutputType Strategy::Execute(string pId, const Operator<OpT>& pOp, Args&&... args)
 * Execute the given Operator with the corresponding arguments under the given operator ID.
 * This function will execute the Operator and measure the time of the execution.
 * The current Session object will be included to Operator::operator() as the first arguments, then
 * the given arguments will be forwarded.
 *
 * @tparam OpT The type of the Operator (auto-deduced).
 * @tparam Args The type of the arguments (auto-deduced).
 * @param pId The operator ID, which is used to track the identity of the operator.
 * @param pOp The Operator to be executed.
 * @param args The arguments to be forwarded to the function.
 * @return The return value of the given Operator.
 */

/**
 * @fn OperatorGroup< OpT >::ReturnType Strategy::ExecuteInParallel(string pId, const OperatorGroup<OpT>& pGroup, Args&&... args)
 * Execute the given OperatorGroup in parallel with the corresponding arguments under the given operator ID.
 * This function will execute the OperatorGroup and measure the time of the execution.
 * The current Session object will be included to OperatorGroup::InParallel() as the first arguments, then
 * the given arguments will be forwarded.
 *
 * @tparam OpT The type of the Operator (auto-deduced).
 * @tparam Args The type of the arguments (auto-deduced).
 * @param pId The operator ID, which is used to track the identity of the operator.
 * @param pGroup The OperatorGroup to be executed.
 * @param args The arguments to be forwarded to the function.
 * @return The return value of the given OperatorGroup.
 */

/**
 * @fn OpT::OutputType Strategy::ExecuteInSeries(string pId, const SeriesOperatorGroup<OpT>& pGroup,
 * typename SeriesOperatorGroup<OpT>::ProcessingType pData, Args&&... args)
 * Execute the given SeriesOperatorGroup in series with the corresponding arguments under the given operator ID.
 * This function will execute the SeriesOperatorGroup and measure the time of the execution.
 * The current Session object will be included to SeriesOperatorGroup::InSeries() as the first arguments, then
 * the given arguments will be forwarded.
 *
 * @tparam OpT The type of the Operator (auto-deduced).
 * @tparam Args The type of the arguments (auto-deduced).
 * @param pId The operator ID, which is used to track the identity of the operator.
 * @param pGroup The SeriesOperatorGroup to be executed.
 * @param pData The input data to be processed.
 * @param args The arguments to be forwarded to the function.
 * @return The return value of the given SeriesOperatorGroup.
 */

Strategy* Strategy::sCurrent = nullptr;

void Strategy::SignalHandler(int signum) {
	if (!sCurrent)
		exit(-1);

	sCurrent->GetSession()->Terminate();
	string sigStr = signum == SIGINT ? "SIGINT" : "SIGTERM";
	EA_LOG_ERROR << sigStr + " received. Terminating..." << flush;
}

} /* namespace ea */
