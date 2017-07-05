/*
 * PhenomePool.h
 *
 *  Created on: Jun 29, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../../EA/Type/Core.h"
#include "../interface/Phenome.h"
#include "Pool.h"
#include "../../rtoc/Constructible.h"

namespace ea {

using namespace std;

class PhenomePool: public vector<PhenomePtr>, public Pool {
public:
	EA_TYPEINFO_DEFAULT(PhenomePool)

	inline PhenomePool() :
			vector<PhenomePtr>() {
	}
	inline PhenomePool(uint count) :
			vector<PhenomePtr>(count) {
	}
	template<class InputIt>
	inline PhenomePool(InputIt first, InputIt last) :
			vector<PhenomePtr>(first, last) {
	}

	inline virtual ~PhenomePool() {
	}

	void Shuffle();

	static PhenomePoolPtr Join(vector<PhenomePoolPtr> pPools);

protected:
	virtual void DoSerialize(ostream& pStream) const override;
	virtual void DoDeserialize(istream& pStream) override;
};

} /* namespace ea */
