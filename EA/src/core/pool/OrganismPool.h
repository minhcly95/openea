/*
 * OrganismPool.h
 *
 *  Created on: Mar 31, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../../EA/Type/Core.h"
#include "../Organism.h"
#include "Pool.h"

namespace ea {

using namespace std;

class OrganismPool: public vector<OrganismPtr>, public Pool {
public:
	EA_TYPEINFO_DEFAULT(OrganismPool)

	inline OrganismPool() :
			vector<OrganismPtr>() {
	}
	inline OrganismPool(uint count) :
			vector<OrganismPtr>(count) {
	}
	template<class InputIt>
	inline OrganismPool(InputIt first, InputIt last) :
			vector<OrganismPtr>(first, last) {
	}

	inline virtual ~OrganismPool() {
	}

	void Sort();
	void Shuffle();

	GenomePoolPtr Extract();

	static OrganismPoolPtr Join(vector<OrganismPoolPtr> pPools);

protected:
	virtual void DoSerialize(ostream& pStream) const override;
	virtual void DoDeserialize(istream& pStream) override;
};

} /* namespace ea */
