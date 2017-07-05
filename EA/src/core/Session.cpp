/*
 * Session.cpp
 *
 *  Created on: May 27, 2017
 *      Author: Bui Quang Minh
 */

#include "../pch.h"
#include "Session.h"
#include "../Common.h"

namespace ea {

/**
 * @class Session
 * The class representing a running instance of an EA.
 * When calling Strategy::Evolve(), a Session object will be created. This object represents
 * a running instance of the algorithm described in the Strategy run on the given Population
 * (or a newly created one if not given). The Strategy and the target Population can be queried
 * by using functions as GetStrategy() and GetPopulation().
 *
 * Session object stores the context of the run. It is propagated throughout the evolution process
 * as the first argument of Operator::operator()(), OperatorGroup::InParallel() or
 * SeriesOperatorGroup::InSeries(). Session object is responsible in measuring time of execution
 * (which can be queried by GetTimeRecords() and GetTotalTime()),
 * and also providing running status such as IsRunning() and Terminate().
 *
 * Users shouldn't create a Session instance by themselves, the algorithm won't run.
 * The only way to start an EA is to call the function Strategy::Evolve().
 *
 * @see Strategy
 * @see Population
 * @see Operator
 */

/**
 * Create a Session object (shouldn't be called by users).
 * This function is only used internally by Strategy class.
 * Creating a standalone Session object will not start the algorithm.
 * The only way to start an EA is to call the function Strategy::Evolve().
 *
 * @param pPopulation The target Population.
 * @param pStrategy The algorithm.
 */
Session::Session(PopulationPtr pPopulation, StrategyPtr pStrategy)
	: mPopulation(pPopulation), mStrategy(pStrategy), mTime(), mTotalTime(), mRunning(true) {

	// System report
	if (MultiThreading::GetRealNumThreads() != 1)
		EA_LOG_DEBUG<< "Parallel processing is enabled, number of threads = " << MultiThreading::GetRealNumThreads() << flush;
	else
		EA_LOG_DEBUG << "Parallel processing is disabled" << flush;
}

/**
 * Get the target Population of the run.
 * @return The target Population.
 */
const PopulationPtr& Session::GetPopulation() const {
	return mPopulation;
}
/**
 * Get the Strategy which is applying in this run.
 * @return The applying Strategy.
 */
const StrategyPtr& Session::GetStrategy() const {
	return mStrategy;
}
/**
 * Get the time records measured in this run.
 * This function returns a list of records, which are pairs of operator ID and the average execution time.
 * If an empty order is received, the function will return all the records existed.
 * Otherwise, the order of the returned list will be identical to the given order.
 * Non-existing records are ignored.
 * The order list can be get from Strategy::GetTimeRecordOrder().
 *
 * @param pOrder A list of IDs representing the wanted order of the output.
 * @return A list of records, each is a pair of operator ID and average execution time.
 */
vector<pair<string, float>> Session::GetTimeRecords(vector<string> pOrder) const {
	vector<pair<string, float>> result;
	if (pOrder.size() == 0) {
		for (auto pair : mTime)
			result.push_back({pair.first, pair.second.Average()});
	}
	else {
		for (auto id : pOrder)
			if (mTime.find(id) != mTime.end())
				result.push_back({id, mTime.at(id).Average()});
	}
	return result;
}

/**
 * Get the average execution time of a generation.
 * @return The generational average execution time.
 */
float Session::GetTotalTime() const {
	return mTotalTime.Average();
}

void Session::MeasureTotalTime(function<void(void)> pFunc) {
	chrono::time_point<chrono::high_resolution_clock> t(chrono::high_resolution_clock::now());
	pFunc();
    mTotalTime.Add(MilisFloat(chrono::high_resolution_clock::now() - t));
}

/**
 * Whether this Session is running or is terminated.
 * If Terminate() has been called, this function will return false even if the last generation
 * is not yet finished.
 *
 * @return true if Terminate() has not been called yet.
 */
bool ea::Session::IsRunning() const {
	return mRunning;
}
/**
 * Terminate the process.
 * This function will mark the stop flag of the run, causing the evolution process
 * stops on the next generation. This won't terminate the run immediately, but rather
 * wait until the current generation is finished. However, after calling this function,
 * IsRunning() will always return false regardless the running status.
 */
void ea::Session::Terminate() {
	mRunning = false;
}

}

