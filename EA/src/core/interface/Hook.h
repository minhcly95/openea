/*
 * Hook.h
 *
 *  Created on: Apr 3, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../EA/Type/Core.h"
#include "../../rtoc/Constructible.h"

namespace ea {

using namespace std;

class Hook : public Constructible {
public:
	using OutputType = void;

	Hook();
	virtual ~Hook();

	void Generational();
	void Initial();
	void Start();
	void End();

	void operator() (const SessionPtr& pSession, void (Hook::*pFunc)());

protected:
	uint GetSize() const;
	ullong GetGeneration() const;
	ullong GetEvaluation() const;

	OrganismPoolPtr GetMainPool() const;
	void SortMainPool();
	const OrganismPtr& GetBestOrganism();

	void Terminate();
	bool IsRunning();

	const PopulationPtr& GetPopulation();
	const SessionPtr& GetSession();

	inline virtual void DoGenerational() {};
	inline virtual void DoInitial() {};
	inline virtual void DoStart() {};
	inline virtual void DoEnd() {};

private:
	SessionPtr mSession;
	PopulationPtr mPopulation;
};

} /* namespace ea */
