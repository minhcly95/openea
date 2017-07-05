/*
 * Phenome.h
 *
 *  Created on: Jun 29, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include <iostream>
#include "../../EA/Type/Core.h"
#include "../../rtoc/Storable.h"

namespace ea {

using namespace std;

class Phenome: public virtual Storable {
public:
	inline Phenome(const GenomePtr& pGenome) :
		mGenome(pGenome) { }

	inline virtual ~Phenome() {	}

	virtual ostream& Print(ostream& os) const;
	const GenomePtr& GetGenome() const;

protected:
	virtual void DoSerialize(ostream& pStream) const override;
	virtual void DoDeserialize(istream& pStream) override;

private:
	GenomePtr mGenome;
};

inline ostream& operator <<(ostream& os, const Phenome& phenome) {
	return phenome.Print(os);
}

template <class G>
class TypedPhenome: public Phenome {
	static_assert(is_base_of<Genome, G>::value,
			"TypedPhenome<G> : G must be a child of Genome.");

public:
	inline TypedPhenome(const Ptr<G>& pGenome) : Genome(static_pointer_cast<Genome>(pGenome)) { }
	inline virtual ~TypedPhenome() { }

	inline const Ptr<G>& GetGenome() const {
		return Phenome::GetGenome()->To<G>();
	}
};

} /* namespace ea */
