/*
 * Strategy.h
 *
 *  Created on: May 18, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../Common.h"
#include "../../EA/Type/Core.h"
#include "../../core/Population.h"
#include "../../core/Session.h"
#include "../../core/Operator.h"
#include "../../core/OperatorGroup.h"
#include "../../core/SeriesOperatorGroup.h"
#include "../../core/interface/Hook.h"
#include "../../rtoc/Constructible.h"

namespace ea {

using namespace std;

class Strategy : public Constructible {
public:
	Strategy();
	virtual ~Strategy();

	SessionPtr Evolve(PopulationPtr pPopulation = nullptr);
	void Evolve(SessionPtr& pSession, PopulationPtr pPopulation = nullptr);

	virtual bool IsReady() { return true; }

	OperatorGroup<Hook> hooks;

protected:
	virtual void Setup() = 0;
	virtual void Begin() { }
	virtual void Loop() = 0;
	virtual void End() { }

	virtual vector<string> GetTimeRecordOrder() const {
		return vector<string>();
	}

	const PopulationPtr& GetPopulation() const;
	const SessionPtr& GetSession() const;

	// Set and get pool functions
	void SetPool(uint pIndex, const PoolPtr& pPool);
	template <class T>
	void SetPool(uint pIndex, const Ptr<T>& pPool) {
		SetPool(pIndex, static_pointer_cast<Pool>(pPool));
	}
	void SetGenomePool(uint pIndex, const GenomePoolPtr& pPool);
	void SetOrganismPool(uint pIndex, const OrganismPoolPtr& pPool);

	PoolPtr GetPool(uint pIndex);
	GenomePoolPtr GetGenomePool(uint pIndex);
	OrganismPoolPtr GetOrganismPool(uint pIndex);

	template<class ReturnT, class... Args>
	ReturnT Execute(string pId, function<ReturnT(Args&&...)> pFunc, Args&&... args) {
		EA_LOG_TRACE << "Gen " << GetPopulation()->GetGeneration()
				<< ", function \"" << pId << "\"" << flush;
		return mSession->Measure(pId, pFunc, forward<Args>(args)...);
	}
	template<class OpT, class... Args>
	typename OpT::OutputType Execute(string pId, const Operator<OpT>& pOp, Args&&... args) {
		EA_LOG_TRACE << "Gen " << GetPopulation()->GetGeneration()
				<< ", operator \"" << pId << "\"" << flush;

		return mSession->Measure(pId, function<typename OpT::OutputType(void)>([&] () {
			return pOp(mSession, forward<Args>(args)...);
		}));
	}

	template<class OpT, class... Args>
	typename OperatorGroup<OpT>::ReturnType ExecuteInParallel(string pId,
			const OperatorGroup<OpT>& pGroup, Args&&... args) {

		EA_LOG_TRACE << "Gen " << GetPopulation()->GetGeneration()
				<< ", operator group \"" << pId << "\" in parallel" << flush;

		return mSession->Measure(pId, function<typename OperatorGroup<OpT>::ReturnType(void)>([&] () {
			return pGroup.InParallel(mSession, forward<Args>(args)...);
		}));
	}

	template<class OpT, class... Args>
	typename OpT::OutputType ExecuteInSeries(string pId, const SeriesOperatorGroup<OpT>& pGroup,
			typename SeriesOperatorGroup<OpT>::ProcessingType pData, Args&&... args) {

		EA_LOG_TRACE << "Gen " << GetPopulation()->GetGeneration()
				<< ", operator group \"" << pId << "\" in series" << flush;

		return mSession->Measure(pId, function<typename OpT::OutputType(void)>([&] () {
 			return pGroup.InSeries(mSession, pData, forward<Args>(args)...);
		}));
	}

private:
	PopulationPtr mPopulation;
	SessionPtr mSession;

	static void SignalHandler(int signum);
	static Strategy* sCurrent;
};

} /* namespace ea */
