/*
 * Population.h
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../EA/Type/Core.h"
#include "../rtoc/Storable.h"

namespace ea {

using namespace std;

class Population final: public Storable {
public:
	Population();
	virtual ~Population();

	// Counter managenment
	ullong GetCounter(string pId) const;
	void SetCounter(string pId, ullong pValue);
	ullong IncreaseCounter(string pId);

	using CounterMap = map<string, atomic<ullong>>;
	const CounterMap& GetCounters();

	static const string GENERATION_COUNTER;
	static const string EVALUATION_COUNTER;

	ullong GetGeneration() const;
	ullong GetEvaluation() const;

	ullong IncreaseGeneration();
	ullong IncreaseEvaluation();

	// Pool management
	PoolPtr GetPool(uint pIndex) const;
	void SetPool(uint pIndex, const PoolPtr& pPool);
	GenomePoolPtr GetGenomePool(uint pIndex) const;
	OrganismPoolPtr GetOrganismPool(uint pIndex) const;

	template<class P>
	void SetPool(uint pIndex, const Ptr<P>& pPool) {
		static_assert(is_base_of<Pool, P>::value,
				"Population::SetPool<P>() : P must be a child of Pool.");
		SetPool(pIndex, static_pointer_cast<Pool>(pPool));
	}

protected:
	virtual void DoSerialize(ostream& pStream) const override;
	virtual void DoDeserialize(istream& pStream) override;

private:
	map<uint, PoolPtr> mPools;
	CounterMap mCounters;
};

} /* namespace ea */
