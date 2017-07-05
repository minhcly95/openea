/*
 * GenomePool.h
 *
 *  Created on: May 18, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../../EA/Type/Core.h"
#include "../interface/Genome.h"
#include "Pool.h"
#include "../../rtoc/Constructible.h"

namespace ea {

using namespace std;

class GenomePool: public vector<GenomePtr>, public Pool {
public:
	EA_TYPEINFO_DEFAULT(GenomePool)

	inline GenomePool() :
			vector<GenomePtr>() {
	}
	inline GenomePool(uint count) :
			vector<GenomePtr>(count) {
	}
	template<class InputIt>
	inline GenomePool(InputIt first, InputIt last) :
			vector<GenomePtr>(first, last) {
	}

	inline virtual ~GenomePool() {
	}

	void Shuffle();

	static GenomePoolPtr Join(vector<GenomePoolPtr> pPools);

protected:
	virtual void DoSerialize(ostream& pStream) const override;
	virtual void DoDeserialize(istream& pStream) override;
};

} /* namespace ea */
